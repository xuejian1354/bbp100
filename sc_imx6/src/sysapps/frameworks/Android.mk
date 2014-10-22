LOCAL_PATH:= $(call my-dir)

# the jni test liarary
# ============================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := 
ifeq ($(TARGET_SC_FRAMWORKS_TEST), true)
LOCAL_SRC_FILES += jni/com_bbp_server_HalTestService.cpp
endif

LOCAL_SHARED_LIBRARIES := libutils libbinder libhardware
LOCAL_MODULE := libhardware_bbp
LOCAL_MODULE_TAGS := optional

ifneq ($(LOCAL_SRC_FILES),)
include $(BUILD_SHARED_LIBRARY)
endif


# the frameworks library
# ============================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := 
ifeq ($(TARGET_SC_FRAMWORKS_TEST), true)
LOCAL_SRC_FILES += \
        aidl/com/bbp/manager/IHalTestService.aidl \
	aidl/com/bbp/manager/HalTestManager.java \
	services/com/bbp/server/HalTestSystemServer.java \
	services/com/bbp/server/HalTestService.java
endif

LOCAL_MODULE:= bbp

ifneq ($(LOCAL_SRC_FILES),)
include $(BUILD_JAVA_LIBRARY)
endif
