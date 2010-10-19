# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := net_unittests
DEFS_Debug := '-DNO_HEAPCHECKER' \
	'-DCHROMIUM_BUILD' \
	'-DENABLE_REMOTING=1' \
	'-DENABLE_GPU=1' \
	'-DUNIT_TEST' \
	'-DGTEST_HAS_RTTI=0' \
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

INCS_Debug := -I. \
	-Itesting/gmock/include \
	-Itesting/gtest/include

DEFS_Release := '-DNO_HEAPCHECKER' \
	'-DCHROMIUM_BUILD' \
	'-DENABLE_REMOTING=1' \
	'-DENABLE_GPU=1' \
	'-DUNIT_TEST' \
	'-DGTEST_HAS_RTTI=0' \
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

INCS_Release := -I. \
	-Itesting/gmock/include \
	-Itesting/gtest/include

OBJS := $(obj).target/$(TARGET)/net/base/address_list_unittest.o \
	$(obj).target/$(TARGET)/net/base/cert_database_nss_unittest.o \
	$(obj).target/$(TARGET)/net/base/cookie_monster_unittest.o \
	$(obj).target/$(TARGET)/net/base/data_url_unittest.o \
	$(obj).target/$(TARGET)/net/base/directory_lister_unittest.o \
	$(obj).target/$(TARGET)/net/base/dnssec_unittest.o \
	$(obj).target/$(TARGET)/net/base/dns_util_unittest.o \
	$(obj).target/$(TARGET)/net/base/dnsrr_resolver_unittest.o \
	$(obj).target/$(TARGET)/net/base/escape_unittest.o \
	$(obj).target/$(TARGET)/net/base/file_stream_unittest.o \
	$(obj).target/$(TARGET)/net/base/filter_unittest.o \
	$(obj).target/$(TARGET)/net/base/forwarding_net_log_unittest.o \
	$(obj).target/$(TARGET)/net/base/gzip_filter_unittest.o \
	$(obj).target/$(TARGET)/net/base/host_cache_unittest.o \
	$(obj).target/$(TARGET)/net/base/host_mapping_rules_unittest.o \
	$(obj).target/$(TARGET)/net/base/host_resolver_impl_unittest.o \
	$(obj).target/$(TARGET)/net/base/keygen_handler_unittest.o \
	$(obj).target/$(TARGET)/net/base/listen_socket_unittest.o \
	$(obj).target/$(TARGET)/net/base/mapped_host_resolver_unittest.o \
	$(obj).target/$(TARGET)/net/base/mime_sniffer_unittest.o \
	$(obj).target/$(TARGET)/net/base/mime_util_unittest.o \
	$(obj).target/$(TARGET)/net/base/net_util_unittest.o \
	$(obj).target/$(TARGET)/net/base/pem_tokenizer_unittest.o \
	$(obj).target/$(TARGET)/net/base/registry_controlled_domain_unittest.o \
	$(obj).target/$(TARGET)/net/base/run_all_unittests.o \
	$(obj).target/$(TARGET)/net/base/ssl_cipher_suite_names_unittest.o \
	$(obj).target/$(TARGET)/net/base/ssl_client_auth_cache_unittest.o \
	$(obj).target/$(TARGET)/net/base/ssl_config_service_unittest.o \
	$(obj).target/$(TARGET)/net/base/ssl_false_start_blacklist_unittest.o \
	$(obj).target/$(TARGET)/net/base/static_cookie_policy_unittest.o \
	$(obj).target/$(TARGET)/net/base/transport_security_state_unittest.o \
	$(obj).target/$(TARGET)/net/base/test_completion_callback_unittest.o \
	$(obj).target/$(TARGET)/net/base/upload_data_stream_unittest.o \
	$(obj).target/$(TARGET)/net/base/x509_certificate_unittest.o \
	$(obj).target/$(TARGET)/net/base/x509_cert_types_unittest.o \
	$(obj).target/$(TARGET)/net/disk_cache/addr_unittest.o \
	$(obj).target/$(TARGET)/net/disk_cache/backend_unittest.o \
	$(obj).target/$(TARGET)/net/disk_cache/bitmap_unittest.o \
	$(obj).target/$(TARGET)/net/disk_cache/block_files_unittest.o \
	$(obj).target/$(TARGET)/net/disk_cache/disk_cache_test_base.o \
	$(obj).target/$(TARGET)/net/disk_cache/entry_unittest.o \
	$(obj).target/$(TARGET)/net/disk_cache/mapped_file_unittest.o \
	$(obj).target/$(TARGET)/net/disk_cache/storage_block_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_auth_cache_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_ctrl_response_buffer_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_buffer_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_ls_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_mlsd_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_netware_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_vms_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_directory_listing_parser_windows_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_network_transaction_unittest.o \
	$(obj).target/$(TARGET)/net/ftp/ftp_util_unittest.o \
	$(obj).target/$(TARGET)/net/http/des_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_alternate_protocols_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_cache_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_filter_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_gssapi_posix_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_basic_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_digest_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_factory_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_mock.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_negotiate_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_handler_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_auth_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_byte_range_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_cache_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_chunked_decoder_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_network_layer_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_network_transaction_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_proxy_client_socket_pool_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_request_headers_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_response_body_drainer_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_response_headers_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_transaction_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_util_unittest.o \
	$(obj).target/$(TARGET)/net/http/http_vary_data_unittest.o \
	$(obj).target/$(TARGET)/net/http/mock_gssapi_library_posix.o \
	$(obj).target/$(TARGET)/net/http/url_security_manager_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/init_proxy_resolver_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/multi_threaded_proxy_resolver_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_bypass_rules_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_config_service_linux_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_config_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_list_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_resolver_js_bindings_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_resolver_v8_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_script_fetcher_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_server_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/proxy_service_unittest.o \
	$(obj).target/$(TARGET)/net/proxy/sync_host_resolver_bridge_unittest.o \
	$(obj).target/$(TARGET)/net/socket/client_socket_pool_base_unittest.o \
	$(obj).target/$(TARGET)/net/socket/socks5_client_socket_unittest.o \
	$(obj).target/$(TARGET)/net/socket/socks_client_socket_pool_unittest.o \
	$(obj).target/$(TARGET)/net/socket/socks_client_socket_unittest.o \
	$(obj).target/$(TARGET)/net/socket/ssl_client_socket_unittest.o \
	$(obj).target/$(TARGET)/net/socket/ssl_client_socket_pool_unittest.o \
	$(obj).target/$(TARGET)/net/socket/tcp_client_socket_pool_unittest.o \
	$(obj).target/$(TARGET)/net/socket/tcp_client_socket_unittest.o \
	$(obj).target/$(TARGET)/net/socket_stream/socket_stream_metrics_unittest.o \
	$(obj).target/$(TARGET)/net/socket_stream/socket_stream_unittest.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_framer_test.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_http_stream_unittest.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_network_transaction_unittest.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_protocol_test.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_session_unittest.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_stream_unittest.o \
	$(obj).target/$(TARGET)/net/spdy/spdy_test_util.o \
	$(obj).target/$(TARGET)/net/test/python_utils_unittest.o \
	$(obj).target/$(TARGET)/net/tools/dump_cache/url_to_filename_encoder.o \
	$(obj).target/$(TARGET)/net/tools/dump_cache/url_to_filename_encoder_unittest.o \
	$(obj).target/$(TARGET)/net/tools/dump_cache/url_utilities.o \
	$(obj).target/$(TARGET)/net/tools/dump_cache/url_utilities_unittest.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_job_tracker_unittest.o \
	$(obj).target/$(TARGET)/net/url_request/url_request_unittest.o \
	$(obj).target/$(TARGET)/net/url_request/view_cache_helper_unittest.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_frame_handler_unittest.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_handshake_draft75_unittest.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_handshake_handler_unittest.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_handshake_unittest.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_job_unittest.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_throttle_unittest.o \
	$(obj).target/$(TARGET)/net/websockets/websocket_unittest.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# Make sure our dependencies are built before any of us.
$(OBJS): | $(obj).target/net/libnet.a $(obj).target/net/libnet_test_support.a $(obj).target/base/libbase.a $(obj).target/base/libbase_i18n.a $(obj).target/base/libtest_support_base.a $(obj).target/testing/libgmock.a $(obj).target/testing/libgtest.a $(obj).target/third_party/zlib/libzlib.a $(obj).target/base/allocator/liballocator.a $(obj).target/third_party/modp_b64/libmodp_b64.a $(obj).target/base/third_party/dynamic_annotations/libdynamic_annotations.a $(obj).target/base/libsymbolize.a $(obj).target/net/third_party/nss/libssl.a $(obj).target/base/libxdg_mime.a $(obj).target/third_party/libevent/libevent.a $(obj).target/third_party/icu/libicui18n.a $(obj).target/third_party/icu/libicuuc.a $(obj).target/third_party/icu/libicudata.a $(obj).target/build/temp_gyp/libgoogleurl.a $(obj).target/sdch/libsdch.a $(obj).target/net/libnet_base.a $(obj).target/v8/tools/gyp/libv8_snapshot.a $(obj).target/v8/tools/gyp/libv8_base.a

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
	-Wl,-z,noexecstack \
	-Wl,-uIsHeapProfilerRunning,-uProfilerStart \
	-Wl,-u_Z21InitialMallocHook_NewPKvj,-u_Z22InitialMallocHook_MMapPKvS0_jiiix,-u_Z22InitialMallocHook_SbrkPKvi \
	-Wl,-u_Z21InitialMallocHook_NewPKvm,-u_Z22InitialMallocHook_MMapPKvS0_miiil,-u_Z22InitialMallocHook_SbrkPKvl

LDFLAGS_Release := -pthread \
	-Wl,-z,noexecstack \
	-Wl,-uIsHeapProfilerRunning,-uProfilerStart \
	-Wl,-u_Z21InitialMallocHook_NewPKvj,-u_Z22InitialMallocHook_MMapPKvS0_jiiix,-u_Z22InitialMallocHook_SbrkPKvi \
	-Wl,-u_Z21InitialMallocHook_NewPKvm,-u_Z22InitialMallocHook_MMapPKvS0_miiil,-u_Z22InitialMallocHook_SbrkPKvl \
	-Wl,-O1 \
	-Wl,--as-needed \
	-Wl,--gc-sections

LIBS := -lrt \
	-ldl \
	-lgtk-x11-2.0 \
	-lgdk-x11-2.0 \
	-latk-1.0 \
	-lgio-2.0 \
	-lpangoft2-1.0 \
	-lgdk_pixbuf-2.0 \
	-lm \
	-lpangocairo-1.0 \
	-lcairo \
	-lpango-1.0 \
	-lfreetype \
	-lfontconfig \
	-lgobject-2.0 \
	-lgmodule-2.0 \
	-lgthread-2.0 \
	-lglib-2.0 \
	-lnss3 \
	-lnssutil3 \
	-lsmime3 \
	-lplds4 \
	-lplc4 \
	-lnspr4 \
	-lpthread \
	-lz \
	-lgconf-2 \
	-lresolv

$(builddir)/net_unittests: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(builddir)/net_unittests: LIBS := $(LIBS)
$(builddir)/net_unittests: TOOLSET := $(TOOLSET)
$(builddir)/net_unittests: $(OBJS) $(obj).target/net/libnet.a $(obj).target/net/libnet_test_support.a $(obj).target/base/libbase.a $(obj).target/base/libbase_i18n.a $(obj).target/base/libtest_support_base.a $(obj).target/testing/libgmock.a $(obj).target/testing/libgtest.a $(obj).target/third_party/zlib/libzlib.a $(obj).target/base/allocator/liballocator.a $(obj).target/third_party/modp_b64/libmodp_b64.a $(obj).target/base/third_party/dynamic_annotations/libdynamic_annotations.a $(obj).target/base/libsymbolize.a $(obj).target/net/third_party/nss/libssl.a $(obj).target/base/libxdg_mime.a $(obj).target/third_party/libevent/libevent.a $(obj).target/third_party/icu/libicui18n.a $(obj).target/third_party/icu/libicuuc.a $(obj).target/third_party/icu/libicudata.a $(obj).target/build/temp_gyp/libgoogleurl.a $(obj).target/sdch/libsdch.a $(obj).target/net/libnet_base.a $(obj).target/v8/tools/gyp/libv8_snapshot.a $(obj).target/v8/tools/gyp/libv8_base.a FORCE_DO_CMD
	$(call do_cmd,link)

all_deps += $(builddir)/net_unittests
# Add target alias
.PHONY: net_unittests
net_unittests: $(builddir)/net_unittests

# Add executable to "all" target.
.PHONY: all
all: $(builddir)/net_unittests

