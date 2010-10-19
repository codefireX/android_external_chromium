# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := net
DEFS_Debug := '-DNO_HEAPCHECKER' \
	'-DCHROMIUM_BUILD' \
	'-DENABLE_REMOTING=1' \
	'-DENABLE_GPU=1' \
	'-DU_STATIC_IMPLEMENTATION' \
	'-DUSE_SYSTEM_ZLIB' \
	'-D__STDC_FORMAT_MACROS' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=1' \
	'-D_DEBUG'

# Flags passed to both C and C++ files.
CFLAGS_Debug := -Werror \
	-pthread \
	-fno-exceptions \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-D_FILE_OFFSET_BITS=64 \
	-fvisibility=hidden \
	-pipe \
	-fno-strict-aliasing \
	-pthread \
	-D_REENTRANT \
	-I/usr/include/gtk-2.0 \
	-I/usr/lib/gtk-2.0/include \
	-I/usr/include/atk-1.0 \
	-I/usr/include/cairo \
	-I/usr/include/pango-1.0 \
	-I/usr/include/gio-unix-2.0/ \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-I/usr/include/pixman-1 \
	-I/usr/include/freetype2 \
	-I/usr/include/directfb \
	-I/usr/include/libpng12 \
	-DORBIT2=1 \
	-pthread \
	-I/usr/include/gconf/2 \
	-I/usr/include/orbit-2.0 \
	-I/usr/include/dbus-1.0 \
	-I/usr/lib/dbus-1.0/include \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-pthread \
	-D_REENTRANT \
	-I/usr/include/gtk-2.0 \
	-I/usr/lib/gtk-2.0/include \
	-I/usr/include/gio-unix-2.0/ \
	-I/usr/include/cairo \
	-I/usr/include/pango-1.0 \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-I/usr/include/pixman-1 \
	-I/usr/include/freetype2 \
	-I/usr/include/directfb \
	-I/usr/include/libpng12 \
	-I../net/third_party/nss/ssl \
	-Inet/third_party/nss/ssl \
	-IWebKit/chromium/net/third_party/nss/ssl \
	-I/usr/include/nss \
	-I/usr/include/nspr \
	-O0 \
	-g

# Flags passed to only C (and not C++) files.
CFLAGS_C_Debug := 

# Flags passed to only C++ (and not C) files.
CFLAGS_CC_Debug := -fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden

INCS_Debug := -Ithird_party/icu/public/common \
	-Ithird_party/icu/public/i18n \
	-I. \
	-Isdch/open-vcdiff/src \
	-I$(obj)/gen/net \
	-Iv8/include

DEFS_Release := '-DNO_HEAPCHECKER' \
	'-DCHROMIUM_BUILD' \
	'-DENABLE_REMOTING=1' \
	'-DENABLE_GPU=1' \
	'-DU_STATIC_IMPLEMENTATION' \
	'-DUSE_SYSTEM_ZLIB' \
	'-D__STDC_FORMAT_MACROS' \
	'-DNDEBUG' \
	'-DNVALGRIND' \
	'-DDYNAMIC_ANNOTATIONS_ENABLED=0'

# Flags passed to both C and C++ files.
CFLAGS_Release := -Werror \
	-pthread \
	-fno-exceptions \
	-Wall \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-D_FILE_OFFSET_BITS=64 \
	-fvisibility=hidden \
	-pipe \
	-fno-strict-aliasing \
	-pthread \
	-D_REENTRANT \
	-I/usr/include/gtk-2.0 \
	-I/usr/lib/gtk-2.0/include \
	-I/usr/include/atk-1.0 \
	-I/usr/include/cairo \
	-I/usr/include/pango-1.0 \
	-I/usr/include/gio-unix-2.0/ \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-I/usr/include/pixman-1 \
	-I/usr/include/freetype2 \
	-I/usr/include/directfb \
	-I/usr/include/libpng12 \
	-DORBIT2=1 \
	-pthread \
	-I/usr/include/gconf/2 \
	-I/usr/include/orbit-2.0 \
	-I/usr/include/dbus-1.0 \
	-I/usr/lib/dbus-1.0/include \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-pthread \
	-D_REENTRANT \
	-I/usr/include/gtk-2.0 \
	-I/usr/lib/gtk-2.0/include \
	-I/usr/include/gio-unix-2.0/ \
	-I/usr/include/cairo \
	-I/usr/include/pango-1.0 \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-I/usr/include/pixman-1 \
	-I/usr/include/freetype2 \
	-I/usr/include/directfb \
	-I/usr/include/libpng12 \
	-I../net/third_party/nss/ssl \
	-Inet/third_party/nss/ssl \
	-IWebKit/chromium/net/third_party/nss/ssl \
	-I/usr/include/nss \
	-I/usr/include/nspr \
	-O2 \
	-fno-ident \
	-fdata-sections \
	-ffunction-sections

# Flags passed to only C (and not C++) files.
CFLAGS_C_Release := 

# Flags passed to only C++ (and not C) files.
CFLAGS_CC_Release := -fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden

INCS_Release := -Ithird_party/icu/public/common \
	-Ithird_party/icu/public/i18n \
	-I. \
	-Isdch/open-vcdiff/src \
	-I$(obj)/gen/net \
	-Iv8/include

OBJS := $(obj).target/$(TARGET)/net/disk_cache/addr.o \
	$(obj).target/$(TARGET)/net/disk_cache/backend_impl.o \
	$(obj).target/$(TARGET)/net/disk_cache/bitmap.o \
	$(obj).target/$(TARGET)/net/disk_cache/block_files.o \
	$(obj).target/$(TARGET)/net/disk_cache/cache_util_posix.o \
	$(obj).target/$(TARGET)/net/disk_cache/entry_impl.o \
	$(obj).target/$(TARGET)/net/disk_cache/eviction.o \
	$(obj).target/$(TARGET)/net/disk_cache/file_lock.o \
	$(obj).target/$(TARGET)/net/disk_cache/file_posix.o \
	$(obj).target/$(TARGET)/net/disk_cache/hash.o \
	$(obj).target/$(TARGET)/net/disk_cache/in_flight_backend_io.o \
	$(obj).target/$(TARGET)/net/disk_cache/in_flight_io.o \
	$(obj).target/$(TARGET)/net/disk_cache/mapped_file_posix.o \
	$(obj).target/$(TARGET)/net/disk_cache/mem_backend_impl.o \
	$(obj).target/$(TARGET)/net/disk_cache/mem_entry_impl.o \
	$(obj).target/$(TARGET)/net/disk_cache/mem_rankings.o \
	$(obj).target/$(TARGET)/net/disk_cache/rankings.o \
	$(obj).target/$(TARGET)/net/disk_cache/sparse_control.o \
	$(obj).target/$(TARGET)/net/disk_cache/stats.o \
	$(obj).target/$(TARGET)/net/disk_cache/stats_histogram.o \
	$(obj).target/$(TARGET)/net/disk_cache/trace.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_auth_cache.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_ctrl_response_buffer.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_buffer.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_ls.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_mlsd.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_netware.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_vms.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_windows.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_network_layer.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_network_transaction.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_server_type_histograms.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_util.o \
	$(obj).target/$(TARGET)/net/http/des.o \
	$(obj).target/$(TARGET)/net/http/disk_cache_based_ssl_host_info.o \
	$(obj).target/$(TARGET)/net/http/http_alternate_protocols.o \
	$(obj).target/$(TARGET)/net/http/http_auth.o \
	$(obj).target/$(TARGET)/net/http/http_auth_cache.o \
	$(obj).target/$(TARGET)/net/http/http_auth_controller.o \
	$(obj).target/$(TARGET)/net/http/http_auth_filter.o \
	$(obj).target/$(TARGET)/net/http/http_auth_gssapi_posix.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_basic.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_digest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_factory.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_negotiate.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_ntlm.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_ntlm_portable.o \
	$(obj).target/$(TARGET)/net/http/http_basic_stream.o \
	$(obj).target/$(TARGET)/net/http/http_byte_range.o \
	$(obj).target/$(TARGET)/net/http/http_cache.o \
	$(obj).target/$(TARGET)/net/http/http_cache_transaction.o \
	$(obj).target/$(TARGET)/net/http/http_chunked_decoder.o \
	$(obj).target/$(TARGET)/net/http/http_network_layer.o \
	$(obj).target/$(TARGET)/net/http/http_network_session.o \
	$(obj).target/$(TARGET)/net/http/http_network_transaction.o \
	$(obj).target/$(TARGET)/net/http/http_request_headers.o \
	$(obj).target/$(TARGET)/net/http/http_response_body_drainer.o \
	$(obj).target/$(TARGET)/net/http/http_response_headers.o \
	$(obj).target/$(TARGET)/net/http/http_response_info.o \
	$(obj).target/$(TARGET)/net/http/http_stream_factory.o \
	$(obj).target/$(TARGET)/net/http/http_stream_parser.o \
	$(obj).target/$(TARGET)/net/http/http_stream_request.o \
	$(obj).target/$(TARGET)/net/http/url_security_manager.o \
	$(obj).target/$(TARGET)/net/http/url_security_manager_posix.o \
	$(obj).target/$(TARGET)/net/http/http_proxy_client_socket.o \
	$(obj).target/$(TARGET)/net/http/http_proxy_client_socket_pool.o \
	$(obj).target/$(TARGET)/net/http/http_util.o \
	$(obj).target/$(TARGET)/net/http/http_util_icu.o \
	$(obj).target/$(TARGET)/net/http/http_vary_data.o \
	$(obj).target/$(TARGET)/net/http/md4.o \
	$(obj).target/$(TARGET)/net/http/partial_data.o \
	$(obj).target/$(TARGET)/net/ocsp/nss_ocsp.o \
	$(obj).target/$(TARGET)/net/proxy/init_proxy_resolver.o \
	$(obj).target/$(TARGET)/net/proxy/multi_threaded_proxy_resolver.o \
	$(obj).target/$(TARGET)/net/proxy/polling_proxy_config_service.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_bypass_rules.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_config.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_config_service_linux.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_info.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_list.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_resolver_js_bindings.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_resolver_script_data.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_resolver_v8.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_script_fetcher.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_server.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_service.o \
	$(obj).target/$(TARGET)/net/proxy/sync_host_resolver_bridge.o \
	$(obj).target/$(TARGET)/net/socket/client_socket.o \
	$(obj).target/$(TARGET)/net/socket/client_socket_factory.o \
	$(obj).target/$(TARGET)/net/socket/client_socket_handle.o \
	$(obj).target/$(TARGET)/net/socket/client_socket_pool.o \
	$(obj).target/$(TARGET)/net/socket/client_socket_pool_base.o \
	$(obj).target/$(TARGET)/net/socket/client_socket_pool_histograms.o \
	$(obj).target/$(TARGET)/net/socket/client_socket_pool_manager.o \
	$(obj).target/$(TARGET)/net/socket/socks5_client_socket.o \
	$(obj).target/$(TARGET)/net/socket/socks_client_socket.o \
	$(obj).target/$(TARGET)/net/socket/socks_client_socket_pool.o \
	$(obj).target/$(TARGET)/net/socket/ssl_client_socket_nss.o \
	$(obj).target/$(TARGET)/net/socket/ssl_client_socket_pool.o \
	$(obj).target/$(TARGET)/net/socket/tcp_client_socket_libevent.o \
	$(obj).target/$(TARGET)/net/socket/tcp_client_socket_pool.o \
	$(obj).target/$(TARGET)/net/socket_stream/socket_stream.o \
	$(obj).target/$(TARGET)/net/socket_stream/socket_stream_job.o \
	$(obj).target/$(TARGET)/net/socket_stream/socket_stream_job_manager.o \
	$(obj).target/$(TARGET)/net/socket_stream/socket_stream_metrics.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_frame_builder.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_framer.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_http_stream.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_http_utils.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_io_buffer.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_session.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_session_pool.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_settings_storage.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_stream.o \
	$(obj).target/$(TARGET)/net/url_request/https_prober.o \
	$(obj).target/$(TARGET)/net/url_request/url_request.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_about_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_context.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_data_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_error_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_file_dir_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_file_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_filter.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_ftp_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_http_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_job_manager.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_job_metrics.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_job_tracker.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_netlog_params.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_redirect_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_simple_job.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_test_job.o \
	$(obj).target/$(TARGET)/net/url_request/view_cache_helper.o \
	$(obj).target/$(TARGET)/net/websockets/websocket.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_frame_handler.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_handshake.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_handshake_draft75.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_handshake_handler.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_job.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_throttle.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# Make sure our dependencies are built before any of us.
$(OBJS): | $(obj).target/net/net_resources.stamp $(obj).target/v8/tools/gyp/v8.stamp

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE)) $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE)) $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cc FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := -pthread \
	-Wl,-z,noexecstack

LDFLAGS_Release := -pthread \
	-Wl,-z,noexecstack \
	-Wl,-O1 \
	-Wl,--as-needed \
	-Wl,--gc-sections

LIBS := 

$(obj).target/net/libnet.a: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/net/libnet.a: LIBS := $(LIBS)
$(obj).target/net/libnet.a: TOOLSET := $(TOOLSET)
$(obj).target/net/libnet.a: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,alink)

all_deps += $(obj).target/net/libnet.a
# Add target alias
.PHONY: net
net: $(obj).target/net/libnet.a

# Add target alias to "all" target.
.PHONY: all
all: net

