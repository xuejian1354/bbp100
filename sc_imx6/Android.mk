bbp_dirs := src/kmods src/sysapps

include $(call all-named-subdir-makefiles,$(bbp_dirs))
