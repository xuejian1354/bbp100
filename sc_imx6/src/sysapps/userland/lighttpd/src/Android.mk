LOCAL_PATH := $(call my-dir)

mod_flags := -DHAVE_CONFIG_H -DHAVE_VERSION_H -DLIBRARY_DIR="\"/system/lib\"" -DSBIN_DIR="\"/system/xbin\""  -D_REENTRANT -D__EXTENSIONS__ -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES

common_src_files := array.c buffer.c configfile-glue.c stat_cache.c chunk.c http_chunk.c http-header-glue.c data_string.c log.c keyvalue.c splaytree.c etag.c fdevent.c fdevent_freebsd_kqueue.c fdevent_libev.c fdevent_select.c fdevent_poll.c fdevent_linux_sysepoll.c fdevent_solaris_devpoll.c fdevent_solaris_port.c

ifeq ($(TARGET_SC_LIGHTTPD_WEBSERVER), true)

$(info cp -ad $(LOCAL_PATH)/../web.conf/lighttpd $(TARGET_OUT)/etc)
$(shell cp -ad $(LOCAL_PATH)/../web.conf/lighttpd $(TARGET_OUT)/etc)

############################################
include $(CLEAR_VARS)

LOCAL_MODULE := libmodlightcommon
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_SRC_FILES := $(common_src_files)

include $(BUILD_STATIC_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_flv_streaming
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_LDFLAGS	+= -Wall -W -ldl 

LOCAL_SRC_FILES := mod_flv_streaming.c 

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_evasive
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_evasive.c  inet_ntop_cache.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_webdav
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_webdav.c stream.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_magnet
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_magnet.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_cml
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_cml.c mod_cml_lua.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_trigger_b4_dl
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_trigger_b4_dl.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_mysql_vhost
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_mysql_vhost.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_cgi
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_cgi.c connections-glue.c joblist.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_scgi
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_scgi.c inet_ntop_cache.c joblist.c connections-glue.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_staticfile
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_staticfile.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_dirlisting
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_dirlisting.c stream.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_indexfile
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_indexfile.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_setenv
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_setenv.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_alias
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_alias.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_userdir
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_userdir.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_rrdtool
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_rrdtool.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_usertrack
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_usertrack.c md5.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_proxy
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_proxy.c data_fastcgi.c joblist.c data_array.c inet_ntop_cache.c connections-glue.c crc32.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_ssi
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_ssi.c inet_ntop_cache.c stream.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_secdownload
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_secure_download.c md5.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_expire
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_expire.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_evhost
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_evhost.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_simple_vhost
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_simple_vhost.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_fastcgi
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_fastcgi.c status_counter.c joblist.c inet_ntop_cache.c connections-glue.c data_integer.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_extforward
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_extforward.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_access
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_access.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_compress
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../ $(ANDROID_BUILD_TOP)/external/zlib
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon libz
LOCAL_SRC_FILES := mod_compress.c crc32.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_auth
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_auth.c http_auth.c md5.c stream.c data_array.c inet_ntop_cache.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_rewrite
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_rewrite.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_redirect
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_redirect.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_status
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_status.c connections-glue.c inet_ntop_cache.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE := mod_accesslog
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl 
LOCAL_STATIC_LIBRARIES := libmodlightcommon
LOCAL_SRC_FILES := mod_accesslog.c inet_ntop_cache.c

include $(BUILD_SHARED_LIBRARY)


############################################
include $(CLEAR_VARS)

LOCAL_MODULE    := lighttpd
LOCAL_MODULE_PATH := $(TARGET_OUT)/xbin
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../ $(ANDROID_BUILD_TOP)/external/zlib
LOCAL_CFLAGS    := $(mod_flags)
LOCAL_LDFLAGS	+= -Wall -W -ldl

# assign all of our C files to LOCAL_SRC_FILES
#
LOCAL_SRC_FILES := server.c response.c connections.c network.c configfile.c configparser.c request.c proc_open.c buffer.c log.c keyvalue.c chunk.c http_chunk.c stream.c fdevent.c stat_cache.c plugin.c joblist.c etag.c array.c data_string.c data_count.c data_array.c data_integer.c md5.c data_fastcgi.c fdevent_select.c fdevent_libev.c fdevent_poll.c fdevent_linux_sysepoll.c fdevent_solaris_devpoll.c fdevent_solaris_port.c fdevent_freebsd_kqueue.c data_config.c bitset.c inet_ntop_cache.c crc32.c connections-glue.c configfile-glue.c http-header-glue.c network_write.c network_linux_sendfile.c network_freebsd_sendfile.c network_writev.c network_solaris_sendfilev.c network_openssl.c splaytree.c status_counter.c

include $(BUILD_EXECUTABLE)

endif
