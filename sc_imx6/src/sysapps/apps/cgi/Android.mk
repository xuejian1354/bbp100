LOCAL_PATH := $(call my-dir)


ifeq ($(TARGET_SC_LIGHTTPD_WEBSERVER), true)

$(info mkdir -p $(TARGET_OUT)/usr/webpages)
$(shell mkdir -p $(TARGET_OUT)/usr/webpages)
$(info cp -ad $(LOCAL_PATH)/html.bbp/* $(TARGET_OUT)/usr/webpages)
$(shell cp -ad $(LOCAL_PATH)/html.bbp/* $(TARGET_OUT)/usr/webpages)

endif

