/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __EXYNOS_SOUNDTRIGGERHAL_H__
#define __EXYNOS_SOUNDTRIGGERHAL_H__

#include <pthread.h>
#include <tinyalsa/asoundlib.h>

#include <hardware/hardware.h>
#include <system/sound_trigger.h>
#include <hardware/sound_trigger.h>

#include"soundtrigger_conf.h"

#define UEVENT_MSG_LEN          64*1024
/* Model index */
#define MAX_SOUND_MODELS    2
#define HOTWORD_INDEX       0
#define SVOICE_INDEX        1
#define HANDLE_NONE         -1

#define MODEL_START_CONTROL_COUNT       3
#define MODEL_STOP_CONTROL_COUNT        2
#define MODEL_BACKLOG_CONTROL_COUNT     1

static const struct sound_trigger_properties hw_properties = {
    "Samsung SLSI", // implementor
    "Exynos Primary SoundTrigger HAL, OK Google and SVoice", // description
    1, // version
    { 0x1817de20, 0xfa3b, 0x11e5, 0xbef2, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }, // uuid
    2, // max_sound_models
    1, // max_key_phrases
    1, // max_users
    RECOGNITION_MODE_VOICE_TRIGGER, // recognition_modes
    true, // capture_transition
    0, // max_buffer_ms
    false, // concurrent_capture
    false, // trigger_in_event
    0 // power_consumption_mw
};

static struct sound_trigger_properties_extended_1_3 hw_properties_1_3 = {
    {SOUND_TRIGGER_DEVICE_API_VERSION_1_3, 320}, // API version, rough total size
    {"Samsung SLSI", // implementor
    "Exynos Primary SoundTrigger HAL", // description
    1, // version
    { 0x1817de20, 0xfa3b, 0x11e5, 0xbef2, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b } }, // uuid
    2, // max_sound_models
    1, // max_key_phrases
    1, // max_users
    RECOGNITION_MODE_VOICE_TRIGGER, // recognition_modes
    true, // capture_transition
    0, // max_buffer_ms
    false, // concurrent_capture
    false, // trigger_in_event
    0}, // power_consumption_mw
    "374236809, 4ac4bfe8-a4c9-3f5c-8f02-6db7c7123cb6", // supportedModelArch
    0 // audioCapabilities
};

typedef enum {
       VTS_MAIN_MIC         = 0,    //Main mic
       VTS_HEADSET_MIC      = 1,    //Headset mic
}VTS_MIC_CONF;

typedef enum {
       VTS_RECOGNIZE_STOP                 = 0,    //Stop recognization
       VTS_RECOGNIZE_START                = 1,    //start recognization
}VTS_RECOGNIZE_STATE;

typedef enum {
       SVOICE_UNKNOWN_MODE          = 0,    //None
       SVOICE_BIXBY_MODE            = 1,    //Bixby (LPSD + voice trigger)
       SVOICE_LPSD_MODE             = 2,    //LPSD (babycry/doorbell)
       SVOICE_BIXBY_ALWAYS_MODE     = 3,    //Bixby (LPSD + voice trigger)
}SVOICE_MODEL_MODE;

typedef enum {
       VTS_MODE_OFF                     = 0,    //default is off
       VTS_MODE_VOICE_TRIGGER_ON        = 1,    //Both LPSD & Trigger are enabled (Voice trigger mode)
       VTS_MODE_SOUND_DETECT_ON         = 2,    //Low Power sound Detect only(Babycrying mode)
       VTS_MODE_VT_ALWAYS_ON            = 3,    //VTS key phrase Detection only(Trigger)
       VTS_MODE_GOOGLE_TRIGGER_ON       = 4,    //Google key phrase Detection only(Trigger)
       VTS_MODE_SENSORY_TRIGGER_ON      = 5,    //sensory key phrase Detection only(Trigger)
       VTS_MODE_VOICE_TRIGGER_OFF       = 6,    //OFF: Both LPSD & Trigger are enabled (Voice trigger mode)
       VTS_MODE_SOUND_DETECT_OFF        = 7,    //OFF: Low Power sound Detect only(Babycrying mode)
       VTS_MODE_VT_ALWAYS_OFF           = 8,    //OFF: VTS key phrase Detection only(Trigger)
       VTS_MODE_GOOGLE_TRIGGER_OFF      = 9,    //OFF: Google key phrase Detection
       VTS_MODE_SENSORY_TRIGGER_OFF     = 10,    //OFF: sensory key phrase Detection
       VTS_MODE_COUNT,
}VTS_EXECUTING_MODE;

typedef enum {
       MODEL_RECOGNIZE_STOPPED          = 0,    //Voice Recognition stopped
       MODEL_RECOGNIZE_STARTED          = 1,    //Voice Recognition started
}MODEL_RUNNING_STATE;

typedef enum {
       VOICECALL_STOPPED                = 0,    //Stop recognization
       VOICECALL_STARTED                = 1,    //start recognization
}VOICECALL_STATE;

typedef enum {
        RECOG_CB_NONE           = 0,    // Recognition event callback function not called
        RECOG_CB_STARTED        = 1,    // Recognition event received
        RECOG_CB_CALLED         = 2,    //Recognition event callback of STHW Service Called
}RECOG_CBSTATE;

typedef enum {
        MODEL_STATE_NONE            = 0,    // Model is not stated
        MODEL_STATE_RUNNING         = 1,    // Model is not stated
        MODEL_STATE_STOPABORT       = 2,    // Model is not stated
}MODEL_EXECSTATE;

/* VTS Force Reset control name */
char *vts_forcereset_ctlname[] = {
    "VTS Force Reset",
};

/* Backlog size control name */
char *model_backlog_size_ctlname[] = {
    "VTS VoiceTrigger Value",
};

/* VTS Model recognization start/stop controls  */
char *model_recognize_start_ctlname[] = {
    "VTS Active Keyphrase",
    "VTS VoiceRecognization Mode",
    "VTS VoiceTrigger Value",
};

// Delete Trigger Value setting in the stop control
// : unnecessary and can cause abnormal operation of the firmware
char *model_recognize_stop_ctlname[] = {
    "VTS Active Keyphrase",
    "VTS VoiceRecognization Mode",
};

int svoice_bixbyrecognize_start_ctlvalue[] = {
    0,  //"VTS Active Keyphrase",
    VTS_MODE_VOICE_TRIGGER_ON,  //"VTS Execution Mode",
    1800, //back log size from trigger point
};

int svoice_bixbyrecognize_stop_ctlvalue[] = {
    0, //"VTS Active Keyphrase",
    VTS_MODE_VOICE_TRIGGER_OFF, //"VTS Execution Mode",
};

int svoice_lpsdrecognize_start_ctlvalue[] = {
    0,  //"VTS Active Keyphrase",
    VTS_MODE_SOUND_DETECT_ON,  //"VTS Execution Mode",
    0, //back log size from trigger point
};

int svoice_lpsdrecognize_stop_ctlvalue[] = {
    0, //"VTS Active Keyphrase",
    VTS_MODE_SOUND_DETECT_OFF, //"VTS Execution Mode",
};

int svoice_bixbyalwaysrecognize_start_ctlvalue[] = {
    0,  //"VTS Active Keyphrase",
    VTS_MODE_VT_ALWAYS_ON,  //"VTS Execution Mode",
    1800, //back log size from trigger point
};

int svoice_bixbyalwaysrecognize_stop_ctlvalue[] = {
    0, //"VTS Active Keyphrase",
    VTS_MODE_VT_ALWAYS_OFF, //"VTS Execution Mode",
};

int hotword_recognize_start_ctlvalue[] = {
    1,  //"VTS Active Keyphrase",
    VTS_MODE_GOOGLE_TRIGGER_ON,  //"VTS Execution Mode",
    2000, //back log size from trigger point
};

int hotword_recognize_stop_ctlvalue[] = {
    1, //"VTS Active Keyphrase",
    VTS_MODE_GOOGLE_TRIGGER_OFF, //"VTS Execution Mode",
};

struct pcm_config pcm_config_vt_capture = {
    .channels = DEFAULT_VTS_CHANNELS,
    .rate = DEFAULT_VTS_SAMPLING_RATE,
    .period_size = PRIMARY_VTS_PERIOD_SIZE,
    .period_count = PRIMARY_VTS_PERIOD_COUNT,
    .format = PCM_FORMAT_S16_LE,
};

struct sound_trigger_device {
    struct sound_trigger_hw_device device;
    sound_model_handle_t model_handles[MAX_SOUND_MODELS];
    recognition_callback_t recognition_callbacks[MAX_SOUND_MODELS];
    void *recognition_cookies[MAX_SOUND_MODELS];

    sound_model_callback_t sound_model_callbacks[MAX_SOUND_MODELS];
    void *sound_model_cookies[MAX_SOUND_MODELS];
    pthread_t callback_thread;
    pthread_mutex_t lock;
    pthread_mutex_t trigger_lock;
    pthread_mutex_t recording_lock;
    int send_socket;
    int term_socket;
    int uevent_socket;
    struct sound_trigger_recognition_config *configs[MAX_SOUND_MODELS];
    struct mixer         *mixer;
#ifdef MMAP_INTERFACE_ENABLED
    int vtsdev_fd;
    void *mapped_addr;
#endif
    int is_streaming;
    int triggered_model;
    int sthal_opened;
    bool callback_thread_active;

    int is_seamless_recording;
    struct pcm *streaming_pcm;
    int recognize_started;
    int active_mic;    //Mic to be configured
    int is_mic_configured;
    void *audio_primary_lib;
    int (*notify_sthal_status)(int);

    int is_recording;
    struct pcm *recording_pcm;

    int backlog_size;
    int svoicemodel_mode;

    int voicecall_state;
    int recog_cbstate;
    int model_execstate[MAX_SOUND_MODELS];
    sound_model_handle_t model_stopfailedhandles[MAX_SOUND_MODELS];
    int is_generic;
};

#endif  // __EXYNOS_SOUNDTRIGGERHAL_H__
