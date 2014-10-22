subdirs := 

ifeq ($(TARGET_CUSTOM_IC_HAL), true)
subdirs += battery btn led lpm sysinfo
endif

ifeq ($(TARGET_CUSTOM_IC_HAL_SAMPLE), true)
subdirs += tests
endif

include $(call all-named-subdir-makefiles,$(subdirs))

