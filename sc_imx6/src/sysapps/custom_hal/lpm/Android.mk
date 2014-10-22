##
##
## Build the library
##
##
ifeq ($(TARGET_CUSTOM_IC_HAL_LPM),true)

LOCAL_PATH:= $(call my-dir)


## HAL Library ##
include $(CLEAR_VARS)

LOCAL_MODULE := lpm.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES := lpmHW.c hwlpm.c
LOCAL_COPY_HEADERS_TO := hal
LOCAL_COPY_HEADERS := lpmHW.h
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw

include $(BUILD_SHARED_LIBRARY)

## C/C++ dynamic link Library ##
ifeq ($(TARGET_CUSTOM_IC_HAL_LPM_SAMPLE),true)
include $(CLEAR_VARS)

LOCAL_MODULE := liblpm-$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := lpmHW.c

include $(BUILD_SHARED_LIBRARY)
endif

endif
