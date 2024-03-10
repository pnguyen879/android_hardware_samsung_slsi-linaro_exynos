# Copyright (C) 2014 The Android Open Source Project
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
# Primary Audio HAL
#
ifeq ($(BOARD_USE_AUDIOHAL_COMV1), true)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	audio_hw.c \
	../odm_specific/factory_manager.c \
	../odm_specific/audio_odm_impl.c

ifeq ($(BOARD_USE_SITRIL), true)
LOCAL_SRC_FILES += \
	../odm_specific/voice_manager_sit.c

LOCAL_C_INCLUDES += \
	$(TOP)/hardware/samsung_slsi-linaro/exynos/libaudio/audiohal_comv1/odm_specific/audioril-sit \
	$(TOP)/hardware/samsung_slsi-linaro/exynos/libaudio/audiohal_comv1/odm_specific/audioril-sit/include

LOCAL_CFLAGS += -DUSE_SITRIL
else
LOCAL_SRC_FILES += \
	../odm_specific/voice_manager_sec.c

LOCAL_C_INCLUDES += \
	$(TOP)/hardware/samsung_slsi-linaro/exynos/libaudio/audiohal_comv1/odm_specific/audioril-sec \
	$(TOP)/hardware/samsung_slsi-linaro/exynos/libaudio/audiohal_comv1/odm_specific/audioril-sec/include
endif

LOCAL_C_INCLUDES += \
	$(TOP)/hardware/samsung_slsi-linaro/exynos/include/libaudio/audiohal_comv1 \
	$(TOP)/hardware/samsung_slsi-linaro/exynos/libaudio/audiohal_comv1/odm_specific \
	$(call include-path-for, audio-utils)

LOCAL_HEADER_LIBRARIES := libhardware_headers
LOCAL_SHARED_LIBRARIES := liblog libcutils libprocessgroup
LOCAL_SHARED_LIBRARIES += libaudioproxy libaudio-ril

LOCAL_CFLAGS += -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function

# Audio Feature based Configuration
ifeq ($(BOARD_USE_BTA2DP_OFFLOAD),true)
LOCAL_CFLAGS += -DSUPPORT_BTA2DP_OFFLOAD
endif

ifeq ($(BOARD_USE_SOUNDTRIGGER_HAL),true)
LOCAL_CFLAGS += -DSUPPORT_STHAL_INTERFACE
endif

ifeq ($(BOARD_USE_USB_OFFLOAD),true)
LOCAL_CFLAGS += -DSUPPORT_USB_OFFLOAD
endif

ifeq ($(BOARD_USE_MMAP_HW_VOLUME_CONTROL),true)
LOCAL_CFLAGS += -DSUPPORT_MMAP_HW_VOLUME_CONTROL
endif

ifeq ($(TARGET_SOC), exynos9820)
LOCAL_CFLAGS += -DSUPPORT_DIRECT_MULTI_CHANNEL_STREAM
endif

ifneq ($(BOARD_LOW_LATENCY_CAPTURE_DURATION),)
LOCAL_CFLAGS += -DPREDEFINED_LOW_CAPTURE_DURATION=$(BOARD_LOW_LATENCY_CAPTURE_DURATION)
endif

ifneq ($(BOARD_USB_PLAYBACK_DURATION),)
LOCAL_CFLAGS += -DPREDEFINED_USB_PLAYBACK_DURATION=$(BOARD_USB_PLAYBACK_DURATION)
endif

LOCAL_MODULE := audio.primary.$(TARGET_SOC)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS := optional

LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_SHARED_LIBRARY)
endif
