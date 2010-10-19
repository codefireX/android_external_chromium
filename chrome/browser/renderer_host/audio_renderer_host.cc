// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/renderer_host/audio_renderer_host.h"

#include "base/histogram.h"
#include "base/lock.h"
#include "base/process.h"
#include "base/shared_memory.h"
#include "base/sys_info.h"
#include "chrome/browser/renderer_host/audio_sync_reader.h"
#include "chrome/common/render_messages.h"
#include "chrome/common/render_messages_params.h"
#include "ipc/ipc_logging.h"

// The minimum number of samples in a hardware packet.
// This value is selected so that we can handle down to 5khz sample rate.
static const int kMinSamplesPerHardwarePacket = 1024;

// The maximum number of samples in a hardware packet.
// This value is selected so that we can handle up to 192khz sample rate.
static const int kMaxSamplesPerHardwarePacket = 64 * 1024;

// This constant governs the hardware audio buffer size, this value should be
// chosen carefully.
// This value is selected so that we have 8192 samples for 48khz streams.
static const int kMillisecondsPerHardwarePacket = 170;

// We allow at most 50 concurrent audio streams in most case. This is a
// rather high limit that is practically hard to reach.
static const size_t kMaxStreams = 50;

// By experiment the maximum number of audio streams allowed in Leopard
// is 18. But we put a slightly smaller number just to be safe.
static const size_t kMaxStreamsLeopard = 15;

// Returns the number of audio streams allowed. This is a practical limit to
// prevent failure caused by too many audio streams opened.
static size_t GetMaxAudioStreamsAllowed() {
#if defined(OS_MACOSX)
  // We are hitting a bug in Leopard where too many audio streams will cause
  // a deadlock in the AudioQueue API when starting the stream. Unfortunately
  // there's no way to detect it within the AudioQueue API, so we put a
  // special hard limit only for Leopard.
  // See bug: http://crbug.com/30242
  int32 major, minor, bugfix;
  base::SysInfo::OperatingSystemVersionNumbers(&major, &minor, &bugfix);
  if (major < 10 || (major == 10 && minor <= 5))
    return kMaxStreamsLeopard;
#endif

  // In OS other than OSX Leopard, the number of audio streams allowed is a
  // lot more so we return a separate number.
  return kMaxStreams;
}

static uint32 SelectHardwarePacketSize(AudioParameters params) {
  // Select the number of samples that can provide at least
  // |kMillisecondsPerHardwarePacket| worth of audio data.
  int samples = kMinSamplesPerHardwarePacket;
  while (samples <= kMaxSamplesPerHardwarePacket &&
         samples * base::Time::kMillisecondsPerSecond <
         params.sample_rate * kMillisecondsPerHardwarePacket) {
    samples *= 2;
  }
  return params.channels * samples * params.bits_per_sample / 8;
}

///////////////////////////////////////////////////////////////////////////////
// AudioRendererHost implementations.
AudioRendererHost::AudioRendererHost()
    : process_id_(0),
      process_handle_(0),
      ipc_sender_(NULL) {
  // Increase the ref count of this object so it is active until we do
  // Release().
  AddRef();
}

AudioRendererHost::~AudioRendererHost() {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));
  DCHECK(audio_entries_.empty());

  // Make sure we received IPCChannelClosing() signal.
  DCHECK(!ipc_sender_);
  DCHECK(!process_handle_);
}

void AudioRendererHost::Destroy() {
  // Post a message to the thread where this object should live and do the
  // actual operations there.
  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(this, &AudioRendererHost::DoDestroy));
}

// Event received when IPC channel is connected to the renderer process.
void AudioRendererHost::IPCChannelConnected(int process_id,
                                            base::ProcessHandle process_handle,
                                            IPC::Message::Sender* ipc_sender) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  process_handle_ = process_handle;
  ipc_sender_ = ipc_sender;
}

// Event received when IPC channel is closing.
void AudioRendererHost::IPCChannelClosing() {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  // Reset IPC related member variables.
  ipc_sender_ = NULL;
  process_handle_ = 0;

  // Since the IPC channel is gone, close all requested audio streams.
  DeleteEntries();
}

///////////////////////////////////////////////////////////////////////////////
// media::AudioOutputController::EventHandler implementations.
void AudioRendererHost::OnCreated(media::AudioOutputController* controller) {
  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(this, &AudioRendererHost::DoCompleteCreation,
                        controller));
}

void AudioRendererHost::OnPlaying(media::AudioOutputController* controller) {
  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(this, &AudioRendererHost::DoSendPlayingMessage,
                        controller));
}

void AudioRendererHost::OnPaused(media::AudioOutputController* controller) {
  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(this, &AudioRendererHost::DoSendPausedMessage,
                        controller));
}

void AudioRendererHost::OnError(media::AudioOutputController* controller,
                                int error_code) {
  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(this, &AudioRendererHost::DoHandleError,
                        controller, error_code));
}

void AudioRendererHost::OnMoreData(media::AudioOutputController* controller,
                                   AudioBuffersState buffers_state) {
  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(this, &AudioRendererHost::DoRequestMoreData,
                        controller, buffers_state));
}

void AudioRendererHost::DoCompleteCreation(
    media::AudioOutputController* controller) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  if (!process_handle_) {
    NOTREACHED() << "Renderer process handle is invalid.";
    DeleteEntryOnError(entry);
    return;
  }

  // Once the audio stream is created then complete the creation process by
  // mapping shared memory and sharing with the renderer process.
  base::SharedMemoryHandle foreign_memory_handle;
  if (!entry->shared_memory.ShareToProcess(process_handle_,
                                            &foreign_memory_handle)) {
    // If we failed to map and share the shared memory then close the audio
    // stream and send an error message.
    DeleteEntryOnError(entry);
    return;
  }

  if (entry->controller->LowLatencyMode()) {
    AudioSyncReader* reader =
        static_cast<AudioSyncReader*>(entry->reader.get());

#if defined(OS_WIN)
    base::SyncSocket::Handle foreign_socket_handle;
#else
    base::FileDescriptor foreign_socket_handle;
#endif

    // If we failed to prepare the sync socket for the renderer then we fail
    // the construction of audio stream.
    if (!reader->PrepareForeignSocketHandle(process_handle_,
                                            &foreign_socket_handle)) {
      DeleteEntryOnError(entry);
      return;
    }

    SendMessage(new ViewMsg_NotifyLowLatencyAudioStreamCreated(
        entry->render_view_id, entry->stream_id, foreign_memory_handle,
        foreign_socket_handle, entry->shared_memory.max_size()));
    return;
  }

  // The normal audio stream has created, send a message to the renderer
  // process.
  SendMessage(new ViewMsg_NotifyAudioStreamCreated(
      entry->render_view_id, entry->stream_id, foreign_memory_handle,
      entry->shared_memory.max_size()));
}

void AudioRendererHost::DoSendPlayingMessage(
    media::AudioOutputController* controller) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  ViewMsg_AudioStreamState_Params params;
  params.state = ViewMsg_AudioStreamState_Params::kPlaying;
  SendMessage(new ViewMsg_NotifyAudioStreamStateChanged(
      entry->render_view_id, entry->stream_id, params));
}

void AudioRendererHost::DoSendPausedMessage(
    media::AudioOutputController* controller) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  ViewMsg_AudioStreamState_Params params;
  params.state = ViewMsg_AudioStreamState_Params::kPaused;
  SendMessage(new ViewMsg_NotifyAudioStreamStateChanged(
      entry->render_view_id, entry->stream_id, params));
}

void AudioRendererHost::DoRequestMoreData(
    media::AudioOutputController* controller,
    AudioBuffersState buffers_state) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  // If we already have a pending request then return.
  AudioEntry* entry = LookupByController(controller);
  if (!entry || entry->pending_buffer_request)
    return;

  DCHECK(!entry->controller->LowLatencyMode());
  entry->pending_buffer_request = true;
  SendMessage(
      new ViewMsg_RequestAudioPacket(
          entry->render_view_id, entry->stream_id, buffers_state));
}

void AudioRendererHost::DoHandleError(media::AudioOutputController* controller,
                                      int error_code) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  DeleteEntryOnError(entry);
}

///////////////////////////////////////////////////////////////////////////////
// IPC Messages handler
bool AudioRendererHost::OnMessageReceived(const IPC::Message& message,
                                          bool* message_was_ok) {
  if (!IsAudioRendererHostMessage(message))
    return false;
  *message_was_ok = true;

  IPC_BEGIN_MESSAGE_MAP_EX(AudioRendererHost, message, *message_was_ok)
    IPC_MESSAGE_HANDLER(ViewHostMsg_CreateAudioStream, OnCreateStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_PlayAudioStream, OnPlayStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_PauseAudioStream, OnPauseStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_FlushAudioStream, OnFlushStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_CloseAudioStream, OnCloseStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_NotifyAudioPacketReady, OnNotifyPacketReady)
    IPC_MESSAGE_HANDLER(ViewHostMsg_GetAudioVolume, OnGetVolume)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SetAudioVolume, OnSetVolume)
  IPC_END_MESSAGE_MAP_EX()

  return true;
}

bool AudioRendererHost::IsAudioRendererHostMessage(
    const IPC::Message& message) {
  switch (message.type()) {
    case ViewHostMsg_CreateAudioStream::ID:
    case ViewHostMsg_PlayAudioStream::ID:
    case ViewHostMsg_PauseAudioStream::ID:
    case ViewHostMsg_FlushAudioStream::ID:
    case ViewHostMsg_CloseAudioStream::ID:
    case ViewHostMsg_NotifyAudioPacketReady::ID:
    case ViewHostMsg_GetAudioVolume::ID:
    case ViewHostMsg_SetAudioVolume::ID:
      return true;
    default:
      break;
    }
    return false;
}

void AudioRendererHost::OnCreateStream(
    const IPC::Message& msg, int stream_id,
    const ViewHostMsg_Audio_CreateStream_Params& params, bool low_latency) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));
  DCHECK(LookupById(msg.routing_id(), stream_id) == NULL);

  // Limit the number of audio streams opened. This is to prevent using
  // excessive resources for a large number of audio streams. More
  // importantly it prevents instability on certain systems.
  // See bug: http://crbug.com/30242
  if (audio_entries_.size() >= GetMaxAudioStreamsAllowed()) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  // Select the hardwaer packet size if not specified.
  uint32 hardware_packet_size = params.packet_size;
  if (!hardware_packet_size) {
    hardware_packet_size = SelectHardwarePacketSize(params.params);
  }

  scoped_ptr<AudioEntry> entry(new AudioEntry());
  // Create the shared memory and share with the renderer process.
  if (!entry->shared_memory.Create("", false, false, hardware_packet_size) ||
      !entry->shared_memory.Map(entry->shared_memory.max_size())) {
    // If creation of shared memory failed then send an error message.
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  if (low_latency) {
    // If this is the low latency mode, we need to construct a SyncReader first.
    scoped_ptr<AudioSyncReader> reader(
        new AudioSyncReader(&entry->shared_memory));

    // Then try to initialize the sync reader.
    if (!reader->Init()) {
      SendErrorMessage(msg.routing_id(), stream_id);
      return;
    }

    // If we have successfully created the SyncReader then assign it to the
    // entry and construct an AudioOutputController.
    entry->reader.reset(reader.release());
    entry->controller =
        media::AudioOutputController::CreateLowLatency(
            this, params.params,
            hardware_packet_size,
            entry->reader.get());
  } else {
    // The choice of buffer capacity is based on experiment.
    entry->controller =
        media::AudioOutputController::Create(this, params.params,
                                             hardware_packet_size,
                                             3 * hardware_packet_size);
  }

  if (!entry->controller) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  // If we have created the controller successfully create a entry and add it
  // to the map.
  entry->render_view_id = msg.routing_id();
  entry->stream_id = stream_id;

 audio_entries_.insert(std::make_pair(
     AudioEntryId(msg.routing_id(), stream_id),
     entry.release()));
}

void AudioRendererHost::OnPlayStream(const IPC::Message& msg, int stream_id) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupById(msg.routing_id(), stream_id);
  if (!entry) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  entry->controller->Play();
}

void AudioRendererHost::OnPauseStream(const IPC::Message& msg, int stream_id) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupById(msg.routing_id(), stream_id);
  if (!entry) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  entry->controller->Pause();
}

void AudioRendererHost::OnFlushStream(const IPC::Message& msg, int stream_id) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupById(msg.routing_id(), stream_id);
  if (!entry) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  entry->controller->Flush();
}

void AudioRendererHost::OnCloseStream(const IPC::Message& msg, int stream_id) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupById(msg.routing_id(), stream_id);

  if (entry)
    CloseAndDeleteStream(entry);
}

void AudioRendererHost::OnSetVolume(const IPC::Message& msg, int stream_id,
                                    double volume) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupById(msg.routing_id(), stream_id);
  if (!entry) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  // Make sure the volume is valid.
  CHECK(volume >= 0 && volume <= 1.0);
  entry->controller->SetVolume(volume);
}

void AudioRendererHost::OnGetVolume(const IPC::Message& msg, int stream_id) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));
  NOTREACHED() << "This message shouldn't be received";
}

void AudioRendererHost::OnNotifyPacketReady(
    const IPC::Message& msg, int stream_id, uint32 packet_size) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntry* entry = LookupById(msg.routing_id(), stream_id);
  if (!entry) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  DCHECK(!entry->controller->LowLatencyMode());
  CHECK(packet_size <= entry->shared_memory.max_size());

  if (!entry->pending_buffer_request) {
    NOTREACHED() << "Buffer received but no such pending request";
  }
  entry->pending_buffer_request = false;

  // Enqueue the data to media::AudioOutputController.
  entry->controller->EnqueueData(
      reinterpret_cast<uint8*>(entry->shared_memory.memory()),
      packet_size);
}

void AudioRendererHost::DoDestroy() {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  // Reset IPC releated members.
  ipc_sender_ = NULL;
  process_handle_ = 0;

  // Close all audio streams.
  DeleteEntries();

  // Decrease the reference to this object, which may lead to self-destruction.
  Release();
}

void AudioRendererHost::SendMessage(IPC::Message* message) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  if (ipc_sender_)
    ipc_sender_->Send(message);
}

void AudioRendererHost::SendErrorMessage(int32 render_view_id,
                                         int32 stream_id) {
  ViewMsg_AudioStreamState_Params state;
  state.state = ViewMsg_AudioStreamState_Params::kError;
  SendMessage(new ViewMsg_NotifyAudioStreamStateChanged(
      render_view_id, stream_id, state));
}

void AudioRendererHost::DeleteEntries() {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  for (AudioEntryMap::iterator i = audio_entries_.begin();
       i != audio_entries_.end(); ++i) {
    CloseAndDeleteStream(i->second);
  }
}

void AudioRendererHost::CloseAndDeleteStream(AudioEntry* entry) {
  if (!entry->pending_close) {
    entry->controller->Close(
        NewRunnableMethod(this, &AudioRendererHost::OnStreamClosed, entry));
    entry->pending_close = true;
  }
}

void AudioRendererHost::OnStreamClosed(AudioEntry* entry) {
  // Delete the entry after we've closed the stream.
  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableMethod(this, &AudioRendererHost::DeleteEntry, entry));
}

void AudioRendererHost::DeleteEntry(AudioEntry* entry) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  // Delete the entry when this method goes out of scope.
  scoped_ptr<AudioEntry> entry_deleter(entry);

  // Erase the entry from the map.
  audio_entries_.erase(
      AudioEntryId(entry->render_view_id, entry->stream_id));
}

void AudioRendererHost::DeleteEntryOnError(AudioEntry* entry) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  // Sends the error message first before we close the stream because
  // |entry| is destroyed in DeleteEntry().
  SendErrorMessage(entry->render_view_id, entry->stream_id);
  CloseAndDeleteStream(entry);
}

AudioRendererHost::AudioEntry* AudioRendererHost::LookupById(
    int route_id, int stream_id) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  AudioEntryMap::iterator i = audio_entries_.find(
      AudioEntryId(route_id, stream_id));
  if (i != audio_entries_.end())
    return i->second;
  return NULL;
}

AudioRendererHost::AudioEntry* AudioRendererHost::LookupByController(
    media::AudioOutputController* controller) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::IO));

  // Iterate the map of entries.
  // TODO(hclam): Implement a faster look up method.
  for (AudioEntryMap::iterator i = audio_entries_.begin();
       i != audio_entries_.end(); ++i) {
    if (controller == i->second->controller.get())
      return i->second;
  }
  return NULL;
}
