#
# Copyright 2016 The Android Open Source Project
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

#
# Primary SoundTrigger HAL module
#
ifeq ($(BOARD_USE_COMMON_AUDIOHAL), true)
ifeq ($(BOARD_USE_SOUNDTRIGGER_HAL),true)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_PROPRIETARY_MODULE := true

DEVICE_BASE_PATH := $(TOP)/device/samsung

LOCAL_SRC_FILES := \
        sound_trigger_hw.c

LOCAL_C_INCLUDES += \
        external/tinyalsa/include \
	$(DEVICE_BASE_PATH)/$(TARGET_BOOTLOADER_BOARD_NAME)/conf \
	$(BOARD_SOUNDTRIGGER_CONFIG_PATH)

LOCAL_SHARED_LIBRARIES := \
        liblog libcutils libtinyalsa libhardware

ifeq ($(BOARD_USE_SOUNDTRIGGER_HAL_MMAP),true)
LOCAL_CFLAGS += -DMMAP_INTERFACE_ENABLED
endif

ifeq ($(BOARD_USE_SOUNDTRIGGER_HAL_2_3),true)
LOCAL_CFLAGS += -DSTHAL_2_3
endif

LOCAL_MODULE := sound_trigger.primary.$(TARGET_SOC)
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))
endif
endif
