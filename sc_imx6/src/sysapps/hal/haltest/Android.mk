##
##
## Build the library
##
##
ifeq ($(TARGET_SC_HAL_TEST), true)

LOCAL_PATH:= $(call my-dir)

## HAL Library ##
include $(CLEAR_VARS)

LOCAL_MODULE := haltest.$(TARGET_BOARD_PLATFORM)
LOCAL_SHARED_LIBRARIES := libcutils liblog libdl
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES := haltest.c hwhaltest.c
LOCAL_COPY_HEADERS_TO := hal
LOCAL_COPY_HEADERS := haltest.h
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw

include $(BUILD_SHARED_LIBRARY)

endif
