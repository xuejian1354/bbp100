ifeq ($(TARGET_SC_KMODS_TEST), true)
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

kdrv_module_file :=kdrv_test.ko

KMODS_ANDROID_SRC_BASE:= $(LOCAL_PATH)

KMODS_ANDROID_ROOT:= $(ANDROID_BUILD_TOP)

KMODS_LINUXPATH=$(KMODS_ANDROID_ROOT)/kernel_imx
KMODS_CROSS_COMPILE=$(KMODS_ANDROID_ROOT)/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-

.PHONY: module_first
module_first:
	@echo [module $(kdrv_module_file)]

$(KMODS_ANDROID_SRC_BASE)/$(kdrv_module_file):module_first $(TARGET_PREBUILT_KERNEL) $(ACP)
	$(MAKE) -C $(KMODS_ANDROID_SRC_BASE) ARCH=arm CROSS_COMPILE=$(KMODS_CROSS_COMPILE)

include $(CLEAR_VARS)
LOCAL_MODULE := $(kdrv_module_file)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/lib/modules
LOCAL_SRC_FILES := $(kdrv_module_file)
include $(BUILD_PREBUILT)
endif
