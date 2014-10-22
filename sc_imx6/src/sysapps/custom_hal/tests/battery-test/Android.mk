# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
ifeq ($(TARGET_CUSTOM_IC_HAL_BATTERY_SAMPLE), true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_PATH := $(TARGET_OUT)/xbin/ic
LOCAL_MODULE_TAGS := tests
LOCAL_MODULE    := battery-test.exe
LOCAL_CFLAGS    := -DBUILDENV_android

# assign all of our C files to LOCAL_SRC_FILES
#
MY_SOURCES := $(wildcard $(LOCAL_PATH)/*.c ..$(LOCAL_PATH))
LOCAL_SRC_FILES := $(MY_SOURCES:$(LOCAL_PATH)%=%) 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include

LOCAL_SHARED_LIBRARIES := libbattery-$(TARGET_BOARD_PLATFORM)

include $(BUILD_EXECUTABLE)


endif
