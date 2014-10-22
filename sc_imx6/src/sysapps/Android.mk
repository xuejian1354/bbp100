bbp_dirs := userland custom_hal hal apps frameworks packages

include $(call all-named-subdir-makefiles,$(bbp_dirs))
