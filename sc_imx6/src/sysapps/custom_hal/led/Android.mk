##
##
## Build the library
##
##
ifeq ($(TARGET_CUSTOM_IC_HAL_LED), true)

LOCAL_PATH:= $(call my-dir)


## HAL Library ##
include $(CLEAR_VARS)

LOCAL_MODULE := led.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES := ledHW.c hwled.c
LOCAL_COPY_HEADERS_TO := hal
LOCAL_COPY_HEADERS := ledHW.h
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw

include $(BUILD_SHARED_LIBRARY)

## C/C++ dynamic link Library ##
ifeq ($(TARGET_CUSTOM_IC_HAL_LED_SAMPLE), true)
include $(CLEAR_VARS)

LOCAL_MODULE := libled-$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := ledHW.c

include $(BUILD_SHARED_LIBRARY)
endif

endif
