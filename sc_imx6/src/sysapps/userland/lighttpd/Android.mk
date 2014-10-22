subdirs := src

include $(call all-named-subdir-makefiles,$(subdirs))

