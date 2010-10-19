// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/logging.h"

#if defined(OS_WIN)
#include <io.h>
#include <windows.h>
typedef HANDLE FileHandle;
typedef HANDLE MutexHandle;
// Windows warns on using write().  It prefers _write().
#define write(fd, buf, count) _write(fd, buf, static_cast<unsigned int>(count))
// Windows doesn't define STDERR_FILENO.  Define it here.
#define STDERR_FILENO 2
#elif defined(OS_MACOSX)
#include <CoreFoundation/CoreFoundation.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <mach-o/dyld.h>
#elif defined(OS_POSIX)
#include <sys/syscall.h>
#include <time.h>
#endif

#if defined(OS_POSIX)
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_PATH PATH_MAX
typedef FILE* FileHandle;
#include "pthread.h"
typedef pthread_mutex_t* MutexHandle;
#endif

#include <ctime>
#include <iomanip>
#include <cstring>
#include <algorithm>

#include "base/base_switches.h"
#include "base/command_line.h"
#include "base/debug_util.h"
#include "base/eintr_wrapper.h"
#include "base/lock_impl.h"
#if defined(OS_POSIX)
#include "base/safe_strerror_posix.h"
#endif
#include "base/process_util.h"
#include "base/string_piece.h"
#include "base/utf_string_conversions.h"
#include "base/vlog.h"

namespace logging {

bool g_enable_dcheck = false;
VlogInfo* g_vlog_info = NULL;

const char* const log_severity_names[LOG_NUM_SEVERITIES] = {
  "INFO", "WARNING", "ERROR", "ERROR_REPORT", "FATAL" };

int min_log_level = 0;

// The default set here for logging_destination will only be used if
// InitLogging is not called.  On Windows, use a file next to the exe;
// on POSIX platforms, where it may not even be possible to locate the
// executable on disk, use stderr.
#if defined(OS_WIN)
LoggingDestination logging_destination = LOG_ONLY_TO_FILE;
#elif defined(OS_POSIX)
LoggingDestination logging_destination = LOG_ONLY_TO_SYSTEM_DEBUG_LOG;
#endif

const int kMaxFilteredLogLevel = LOG_WARNING;
std::string* log_filter_prefix;

// For LOG_ERROR and above, always print to stderr.
const int kAlwaysPrintErrorLevel = LOG_ERROR;

// Which log file to use? This is initialized by InitLogging or
// will be lazily initialized to the default value when it is
// first needed.
#if defined(OS_WIN)
typedef std::wstring PathString;
#else
typedef std::string PathString;
#endif
PathString* log_file_name = NULL;

// this file is lazily opened and the handle may be NULL
FileHandle log_file = NULL;

// what should be prepended to each message?
bool log_process_id = false;
bool log_thread_id = false;
bool log_timestamp = true;
bool log_tickcount = false;

// Should we pop up fatal debug messages in a dialog?
bool show_error_dialogs = false;

// An assert handler override specified by the client to be called instead of
// the debug message dialog and process termination.
LogAssertHandlerFunction log_assert_handler = NULL;
// An report handler override specified by the client to be called instead of
// the debug message dialog.
LogReportHandlerFunction log_report_handler = NULL;
// A log message handler that gets notified of every log message we process.
LogMessageHandlerFunction log_message_handler = NULL;

// Helper functions to wrap platform differences.

int32 CurrentProcessId() {
#if defined(OS_WIN)
  return GetCurrentProcessId();
#elif defined(OS_POSIX)
  return getpid();
#endif
}

int32 CurrentThreadId() {
#if defined(OS_WIN)
  return GetCurrentThreadId();
#elif defined(OS_MACOSX)
  return mach_thread_self();
#elif defined(OS_LINUX)
  return syscall(__NR_gettid);
#elif defined(OS_FREEBSD)
  // TODO(BSD): find a better thread ID
  return reinterpret_cast<int64>(pthread_self());
#endif
}

uint64 TickCount() {
#if defined(OS_WIN)
  return GetTickCount();
#elif defined(OS_MACOSX)
  return mach_absolute_time();
#elif defined(OS_POSIX)
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  uint64 absolute_micro =
    static_cast<int64>(ts.tv_sec) * 1000000 +
    static_cast<int64>(ts.tv_nsec) / 1000;

  return absolute_micro;
#endif
}

void CloseFile(FileHandle log) {
#if defined(OS_WIN)
  CloseHandle(log);
#else
  fclose(log);
#endif
}

void DeleteFilePath(const PathString& log_name) {
#if defined(OS_WIN)
  DeleteFile(log_name.c_str());
#else
  unlink(log_name.c_str());
#endif
}

void GetDefaultLogFile(PathString default_log_file) {
#if defined(OS_WIN)
  // On Windows we use the same path as the exe.
  wchar_t module_name[MAX_PATH];
  GetModuleFileName(NULL, module_name, MAX_PATH);
  default_log_file = module_name;
  std::wstring::size_type last_backslash =
      default_log_file.rfind('\\', default_log_file.size());
  if (last_backslash != std::wstring::npos)
    default_log_file.erase(last_backslash + 1);
  default_log_file += L"debug.log";
#elif defined(OS_POSIX)
  // On other platforms we just use the current directory.
  default_log_file = "debug.log";
#endif
}

// This class acts as a wrapper for locking the logging files.
// LoggingLock::Init() should be called from the main thread before any logging
// is done. Then whenever logging, be sure to have a local LoggingLock
// instance on the stack. This will ensure that the lock is unlocked upon
// exiting the frame.
// LoggingLocks can not be nested.
class LoggingLock {
 public:
  LoggingLock() {
    LockLogging();
  }

  ~LoggingLock() {
    UnlockLogging();
  }

  static void Init(LogLockingState lock_log, const PathChar* new_log_file) {
    if (initialized)
      return;
    lock_log_file = lock_log;
    if (lock_log_file == LOCK_LOG_FILE) {
#if defined(OS_WIN)
      if (!log_mutex) {
        std::wstring safe_name;
        if (new_log_file)
          safe_name = new_log_file;
        else
          GetDefaultLogFile(safe_name);
        // \ is not a legal character in mutex names so we replace \ with /
        std::replace(safe_name.begin(), safe_name.end(), '\\', '/');
        std::wstring t(L"Global\\");
        t.append(safe_name);
        log_mutex = ::CreateMutex(NULL, FALSE, t.c_str());
      }
#endif
    } else {
      log_lock = new LockImpl();
    }
    initialized = true;
  }

 private:
  static void LockLogging() {
    if (lock_log_file == LOCK_LOG_FILE) {
#if defined(OS_WIN)
      ::WaitForSingleObject(log_mutex, INFINITE);
      // WaitForSingleObject could have returned WAIT_ABANDONED. We don't
      // abort the process here. UI tests might be crashy sometimes,
      // and aborting the test binary only makes the problem worse.
      // We also don't use LOG macros because that might lead to an infinite
      // loop. For more info see http://crbug.com/18028.
#elif defined(OS_POSIX)
      pthread_mutex_lock(&log_mutex);
#endif
    } else {
      // use the lock
      log_lock->Lock();
    }
  }

  static void UnlockLogging() {
    if (lock_log_file == LOCK_LOG_FILE) {
#if defined(OS_WIN)
      ReleaseMutex(log_mutex);
#elif defined(OS_POSIX)
      pthread_mutex_unlock(&log_mutex);
#endif
    } else {
      log_lock->Unlock();
    }
  }

  // The lock is used if log file locking is false. It helps us avoid problems
  // with multiple threads writing to the log file at the same time.  Use
  // LockImpl directly instead of using Lock, because Lock makes logging calls.
  static LockImpl* log_lock;

  // When we don't use a lock, we are using a global mutex. We need to do this
  // because LockFileEx is not thread safe.
#if defined(OS_WIN)
  static MutexHandle log_mutex;
#elif defined(OS_POSIX)
  static pthread_mutex_t log_mutex;
#endif

  static bool initialized;
  static LogLockingState lock_log_file;
};

// static
bool LoggingLock::initialized = false;
// static
LockImpl* LoggingLock::log_lock = NULL;
// static
LogLockingState LoggingLock::lock_log_file = LOCK_LOG_FILE;

#if defined(OS_WIN)
// static
MutexHandle LoggingLock::log_mutex = NULL;
#elif defined(OS_POSIX)
pthread_mutex_t LoggingLock::log_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

// Called by logging functions to ensure that debug_file is initialized
// and can be used for writing. Returns false if the file could not be
// initialized. debug_file will be NULL in this case.
bool InitializeLogFileHandle() {
  if (log_file)
    return true;

  if (!log_file_name) {
    // Nobody has called InitLogging to specify a debug log file, so here we
    // initialize the log file name to a default.
    log_file_name = new PathString();
    GetDefaultLogFile(*log_file_name);
  }

  if (logging_destination == LOG_ONLY_TO_FILE ||
      logging_destination == LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG) {
#if defined(OS_WIN)
    log_file = CreateFile(log_file_name->c_str(), GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                          OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (log_file == INVALID_HANDLE_VALUE || log_file == NULL) {
      // try the current directory
      log_file = CreateFile(L".\\debug.log", GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      if (log_file == INVALID_HANDLE_VALUE || log_file == NULL) {
        log_file = NULL;
        return false;
      }
    }
    SetFilePointer(log_file, 0, 0, FILE_END);
#elif defined(OS_POSIX)
    log_file = fopen(log_file_name->c_str(), "a");
    if (log_file == NULL)
      return false;
#endif
  }

  return true;
}

void BaseInitLoggingImpl(const PathChar* new_log_file,
                         LoggingDestination logging_dest,
                         LogLockingState lock_log,
                         OldFileDeletionState delete_old) {
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  g_enable_dcheck =
      command_line->HasSwitch(switches::kEnableDCHECK);
  delete g_vlog_info;
  g_vlog_info = NULL;
  // Don't bother initializing g_vlog_info unless we use one of the
  // vlog switches.
  if (command_line->HasSwitch(switches::kV) ||
      command_line->HasSwitch(switches::kVModule)) {
    g_vlog_info =
        new VlogInfo(command_line->GetSwitchValueASCII(switches::kV),
                     command_line->GetSwitchValueASCII(switches::kVModule));
  }

<<<<<<< HEAD
void InitLogging(const PathChar* new_log_file, LoggingDestination logging_dest,
                 LogLockingState lock_log, OldFileDeletionState delete_old) {
  g_enable_dcheck =
#ifdef ANDROID
      false;
#else
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableDCHECK);
#endif // ANDROID
=======
  LoggingLock::Init(lock_log, new_log_file);

  LoggingLock logging_lock;
>>>>>>> Chromium at release 7.0.540.0

  if (log_file) {
    // calling InitLogging twice or after some log call has already opened the
    // default log file will re-initialize to the new options
    CloseFile(log_file);
    log_file = NULL;
  }

  logging_destination = logging_dest;

  // ignore file options if logging is disabled or only to system
  if (logging_destination == LOG_NONE ||
      logging_destination == LOG_ONLY_TO_SYSTEM_DEBUG_LOG)
    return;

  if (!log_file_name)
    log_file_name = new PathString();
  *log_file_name = new_log_file;
  if (delete_old == DELETE_OLD_LOG_FILE)
    DeleteFilePath(*log_file_name);

  InitializeLogFileHandle();

}

void SetMinLogLevel(int level) {
  min_log_level = level;
}

int GetMinLogLevel() {
  return min_log_level;
}

int GetVlogLevelHelper(const char* file, size_t N) {
  DCHECK_GT(N, 0U);
  return g_vlog_info ?
      g_vlog_info->GetVlogLevel(base::StringPiece(file, N - 1)) :
      VlogInfo::kDefaultVlogLevel;
}

void SetLogFilterPrefix(const char* filter)  {
  if (log_filter_prefix) {
    delete log_filter_prefix;
    log_filter_prefix = NULL;
  }

  if (filter)
    log_filter_prefix = new std::string(filter);
}

void SetLogItems(bool enable_process_id, bool enable_thread_id,
                 bool enable_timestamp, bool enable_tickcount) {
  log_process_id = enable_process_id;
  log_thread_id = enable_thread_id;
  log_timestamp = enable_timestamp;
  log_tickcount = enable_tickcount;
}

void SetShowErrorDialogs(bool enable_dialogs) {
  show_error_dialogs = enable_dialogs;
}

void SetLogAssertHandler(LogAssertHandlerFunction handler) {
  log_assert_handler = handler;
}

void SetLogReportHandler(LogReportHandlerFunction handler) {
  log_report_handler = handler;
}

void SetLogMessageHandler(LogMessageHandlerFunction handler) {
  log_message_handler = handler;
}


// Displays a message box to the user with the error message in it.
// Used for fatal messages, where we close the app simultaneously.
void DisplayDebugMessageInDialog(const std::string& str) {
  if (str.empty())
    return;

  if (!show_error_dialogs)
    return;

#if defined(OS_WIN)
  // For Windows programs, it's possible that the message loop is
  // messed up on a fatal error, and creating a MessageBox will cause
  // that message loop to be run. Instead, we try to spawn another
  // process that displays its command line. We look for "Debug
  // Message.exe" in the same directory as the application. If it
  // exists, we use it, otherwise, we use a regular message box.
  wchar_t prog_name[MAX_PATH];
  GetModuleFileNameW(NULL, prog_name, MAX_PATH);
  wchar_t* backslash = wcsrchr(prog_name, '\\');
  if (backslash)
    backslash[1] = 0;
  wcscat_s(prog_name, MAX_PATH, L"debug_message.exe");

  std::wstring cmdline = UTF8ToWide(str);
  if (cmdline.empty())
    return;

  STARTUPINFO startup_info;
  memset(&startup_info, 0, sizeof(startup_info));
  startup_info.cb = sizeof(startup_info);

  PROCESS_INFORMATION process_info;
  if (CreateProcessW(prog_name, &cmdline[0], NULL, NULL, false, 0, NULL,
                     NULL, &startup_info, &process_info)) {
    WaitForSingleObject(process_info.hProcess, INFINITE);
    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);
  } else {
    // debug process broken, let's just do a message box
    MessageBoxW(NULL, &cmdline[0], L"Fatal error",
                MB_OK | MB_ICONHAND | MB_TOPMOST);
  }
#elif defined(USE_X11) && !defined(OS_CHROMEOS)
  // Shell out to xmessage, which behaves like debug_message.exe, but is
  // way more retro.  We could use zenity/kdialog but then we're starting
  // to get into needing to check the desktop env and this dialog should
  // only be coming up in Very Bad situations.
  std::vector<std::string> argv;
  argv.push_back("xmessage");
  argv.push_back(str);
  base::LaunchApp(argv, base::file_handle_mapping_vector(), true /* wait */,
                  NULL);
#else
  // http://code.google.com/p/chromium/issues/detail?id=37026
  NOTIMPLEMENTED();
#endif
}

#if defined(OS_WIN)
LogMessage::SaveLastError::SaveLastError() : last_error_(::GetLastError()) {
}

LogMessage::SaveLastError::~SaveLastError() {
  ::SetLastError(last_error_);
}
#endif  // defined(OS_WIN)

LogMessage::LogMessage(const char* file, int line, LogSeverity severity,
                       int ctr)
    : severity_(severity) {
  Init(file, line);
}

LogMessage::LogMessage(const char* file, int line, const CheckOpString& result)
    : severity_(LOG_FATAL) {
  Init(file, line);
  stream_ << "Check failed: " << (*result.str_);
}

LogMessage::LogMessage(const char* file, int line, LogSeverity severity,
                       const CheckOpString& result)
    : severity_(severity) {
  Init(file, line);
  stream_ << "Check failed: " << (*result.str_);
}

LogMessage::LogMessage(const char* file, int line)
     : severity_(LOG_INFO) {
  Init(file, line);
}

LogMessage::LogMessage(const char* file, int line, LogSeverity severity)
    : severity_(severity) {
  Init(file, line);
}

// writes the common header info to the stream
void LogMessage::Init(const char* file, int line) {
  // log only the filename
  const char* last_slash = strrchr(file, '\\');
  if (last_slash)
    file = last_slash + 1;

  // TODO(darin): It might be nice if the columns were fixed width.

  stream_ <<  '[';
  if (log_process_id)
    stream_ << CurrentProcessId() << ':';
  if (log_thread_id)
    stream_ << CurrentThreadId() << ':';
  if (log_timestamp) {
    time_t t = time(NULL);
    struct tm local_time = {0};
#if _MSC_VER >= 1400
    localtime_s(&local_time, &t);
#else
    localtime_r(&t, &local_time);
#endif
    struct tm* tm_time = &local_time;
    stream_ << std::setfill('0')
            << std::setw(2) << 1 + tm_time->tm_mon
            << std::setw(2) << tm_time->tm_mday
            << '/'
            << std::setw(2) << tm_time->tm_hour
            << std::setw(2) << tm_time->tm_min
            << std::setw(2) << tm_time->tm_sec
            << ':';
  }
  if (log_tickcount)
    stream_ << TickCount() << ':';
  stream_ << log_severity_names[severity_] << ":" << file <<
             "(" << line << ")] ";

  message_start_ = stream_.tellp();
}

LogMessage::~LogMessage() {
  // TODO(brettw) modify the macros so that nothing is executed when the log
  // level is too high.
  if (severity_ < min_log_level)
    return;

#ifndef NDEBUG
  if (severity_ == LOG_FATAL) {
    // Include a stack trace on a fatal.
    StackTrace trace;
    stream_ << std::endl;  // Newline to separate from log message.
    trace.OutputToStream(&stream_);
  }
#endif
  stream_ << std::endl;
  std::string str_newline(stream_.str());

  // Give any log message handler first dibs on the message.
  if (log_message_handler && log_message_handler(severity_, str_newline))
    return;

  if (log_filter_prefix && severity_ <= kMaxFilteredLogLevel &&
      str_newline.compare(message_start_, log_filter_prefix->size(),
                          log_filter_prefix->data()) != 0) {
    return;
  }

  if (logging_destination == LOG_ONLY_TO_SYSTEM_DEBUG_LOG ||
      logging_destination == LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG) {
#if defined(OS_WIN)
    OutputDebugStringA(str_newline.c_str());
#endif
    fprintf(stderr, "%s", str_newline.c_str());
    fflush(stderr);
  } else if (severity_ >= kAlwaysPrintErrorLevel) {
    // When we're only outputting to a log file, above a certain log level, we
    // should still output to stderr so that we can better detect and diagnose
    // problems with unit tests, especially on the buildbots.
    fprintf(stderr, "%s", str_newline.c_str());
    fflush(stderr);
  }

  // We can have multiple threads and/or processes, so try to prevent them
  // from clobbering each other's writes.
  // If the client app did not call InitLogging, and the lock has not
  // been created do it now. We do this on demand, but if two threads try
  // to do this at the same time, there will be a race condition to create
  // the lock. This is why InitLogging should be called from the main
  // thread at the beginning of execution.
  LoggingLock::Init(LOCK_LOG_FILE, NULL);
  // write to log file
  if (logging_destination != LOG_NONE &&
      logging_destination != LOG_ONLY_TO_SYSTEM_DEBUG_LOG) {
    LoggingLock logging_lock;
    if (InitializeLogFileHandle()) {
#if defined(OS_WIN)
      SetFilePointer(log_file, 0, 0, SEEK_END);
      DWORD num_written;
      WriteFile(log_file,
                static_cast<const void*>(str_newline.c_str()),
                static_cast<DWORD>(str_newline.length()),
                &num_written,
                NULL);
#else
      fprintf(log_file, "%s", str_newline.c_str());
      fflush(log_file);
#endif
    }
  }

  if (severity_ == LOG_FATAL) {
    // display a message or break into the debugger on a fatal error
    if (DebugUtil::BeingDebugged()) {
      DebugUtil::BreakDebugger();
    } else {
      if (log_assert_handler) {
        // make a copy of the string for the handler out of paranoia
        log_assert_handler(std::string(stream_.str()));
      } else {
        // Don't use the string with the newline, get a fresh version to send to
        // the debug message process. We also don't display assertions to the
        // user in release mode. The enduser can't do anything with this
        // information, and displaying message boxes when the application is
        // hosed can cause additional problems.
#ifndef NDEBUG
        DisplayDebugMessageInDialog(stream_.str());
#endif
        // Crash the process to generate a dump.
        DebugUtil::BreakDebugger();
      }
    }
  } else if (severity_ == LOG_ERROR_REPORT) {
    // We are here only if the user runs with --enable-dcheck in release mode.
    if (log_report_handler) {
      log_report_handler(std::string(stream_.str()));
    } else {
      DisplayDebugMessageInDialog(stream_.str());
    }
  }
}

#if defined(OS_WIN)
// This has already been defined in the header, but defining it again as DWORD
// ensures that the type used in the header is equivalent to DWORD. If not,
// the redefinition is a compile error.
typedef DWORD SystemErrorCode;
#endif

SystemErrorCode GetLastSystemErrorCode() {
#if defined(OS_WIN)
  return ::GetLastError();
#elif defined(OS_POSIX)
  return errno;
#else
#error Not implemented
#endif
}

#if defined(OS_WIN)
Win32ErrorLogMessage::Win32ErrorLogMessage(const char* file,
                                           int line,
                                           LogSeverity severity,
                                           SystemErrorCode err,
                                           const char* module)
    : err_(err),
      module_(module),
      log_message_(file, line, severity) {
}

Win32ErrorLogMessage::Win32ErrorLogMessage(const char* file,
                                           int line,
                                           LogSeverity severity,
                                           SystemErrorCode err)
    : err_(err),
      module_(NULL),
      log_message_(file, line, severity) {
}

Win32ErrorLogMessage::~Win32ErrorLogMessage() {
  const int error_message_buffer_size = 256;
  char msgbuf[error_message_buffer_size];
  DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM;
  HMODULE hmod;
  if (module_) {
    hmod = GetModuleHandleA(module_);
    if (hmod) {
      flags |= FORMAT_MESSAGE_FROM_HMODULE;
    } else {
      // This makes a nested Win32ErrorLogMessage. It will have module_ of NULL
      // so it will not call GetModuleHandle, so recursive errors are
      // impossible.
      DPLOG(WARNING) << "Couldn't open module " << module_
          << " for error message query";
    }
  } else {
    hmod = NULL;
  }
  DWORD len = FormatMessageA(flags,
                             hmod,
                             err_,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             msgbuf,
                             sizeof(msgbuf) / sizeof(msgbuf[0]),
                             NULL);
  if (len) {
    while ((len > 0) &&
           isspace(static_cast<unsigned char>(msgbuf[len - 1]))) {
      msgbuf[--len] = 0;
    }
    stream() << ": " << msgbuf;
  } else {
    stream() << ": Error " << GetLastError() << " while retrieving error "
        << err_;
  }
}
#elif defined(OS_POSIX)
ErrnoLogMessage::ErrnoLogMessage(const char* file,
                                 int line,
                                 LogSeverity severity,
                                 SystemErrorCode err)
    : err_(err),
      log_message_(file, line, severity) {
}

ErrnoLogMessage::~ErrnoLogMessage() {
  stream() << ": " << safe_strerror(err_);
}
#endif  // OS_WIN

void CloseLogFile() {
  LoggingLock logging_lock;

  if (!log_file)
    return;

  CloseFile(log_file);
  log_file = NULL;
}

void RawLog(int level, const char* message) {
  if (level >= min_log_level) {
    size_t bytes_written = 0;
    const size_t message_len = strlen(message);
    int rv;
    while (bytes_written < message_len) {
      rv = HANDLE_EINTR(
          write(STDERR_FILENO, message + bytes_written,
                message_len - bytes_written));
      if (rv < 0) {
        // Give up, nothing we can do now.
        break;
      }
      bytes_written += rv;
    }

    if (message_len > 0 && message[message_len - 1] != '\n') {
      do {
        rv = HANDLE_EINTR(write(STDERR_FILENO, "\n", 1));
        if (rv < 0) {
          // Give up, nothing we can do now.
          break;
        }
      } while (rv != 1);
    }
  }

  if (level == LOG_FATAL)
    DebugUtil::BreakDebugger();
}

}  // namespace logging

std::ostream& operator<<(std::ostream& out, const wchar_t* wstr) {
  return out << WideToUTF8(std::wstring(wstr));
}
