/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author: 
#########################################################################*/

#ifndef __MERCURY_AUDIO_H__
#define __MERCURY_AUDIO_H__
#include "MercuryDef.h"

typedef enum
{
	AUDIO_MODE_HANDHOLD,
	AUDIO_MODE_HANDFREE,
	AUDIO_MODE_EARPHONE,
	AUDIO_MODE_EARFREE,
	AUDIO_MODE_MAX,
	MERCURY_ENUM_MODE_MAX = 0x7fffffff
}MERCURY_AUDIO_MODE_TYPE_E;

typedef enum
{
    AUDIO_VOLUME_LEVEL1 = 1,
    AUDIO_VOLUME_LEVEL2 = 2,
    AUDIO_VOLUME_LEVEL3 = 3,
    AUDIO_VOLUME_LEVEL4 = 4,
    AUDIO_VOLUME_LEVEL5 = 5,
    AUDIO_VOLUME_LEVEL6 = 6,
    AUDIO_VOLUME_LEVEL7 = 7,
    AUDIO_VOLUME_LEVEL8 = 8,
    AUDIO_VOLUME_LEVEL9 = 9,
    MERCURY_ENUM_VOLUME_MAX = 0x7fffffff
}AUDIO_VOLUME_LEVEL_E;

typedef enum
{
    DTMF_One,           // 1
    DTMF_Two,           // 2
    DTMF_Three,         // 3
    DTMF_letterA,       // A
    DTMF_Four,          // 4
    DTMF_Five,          // 5
    DTMF_Six,           // 6
    DTMF_letterB,       // B
    DTMF_Seven,         // 7
    DTMF_Eight,         // 8
    DTMF_Nine,          // 9
    DTMF_letterC,       // C
    DTMF_Star,          // *
    DTMF_Zero,          // 0
    DTMF_Pond,          // #
    DTMF_letterD,       // D
    DTMF_MAX_ID,         // Reserved, user can't use it.
    MERCURY_ENUM_DTMF_MAX = 0x7fffffff
} MERCURY_DTMF_TONE_ID_E;

typedef enum
{
    AUD_SPKPA_TYPE_CLASSAB,
    AUD_SPKPA_TYPE_CLASSD,
    AUD_SPKPA_TYPE_CLASSK,
    AUD_SPKPA_INPUT_TYPE_QTY = 0x7fffffff
} MERCURY_AUD_SPKPA_TYPE_T;

//RECORD ERROR CODE
typedef enum
{
    AUDIOHAL_ERR_NO = 0,  //No error
    AUDIOHAL_ERR_RESOURCE_BUSY,
    AUDIOHAL_ERR_RESOURCE_TIMEOUT,
    AUDIOHAL_ERR_RESOURCE_NOT_ENABLED,
    AUDIOHAL_ERR_BAD_PARAMETER,
    AUDIOHAL_ERR_NO_MEMORY,
    AUDIOHAL_ERR_EXT_FAIL,
    
	AUDIOHAL_ERR_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} AUDIOHAL_ERR_CODE_T;


typedef enum
{
	AUD_RECORD_ERROR = -1,
	AUD_RECORD_START = 0,
	AUD_RECORD_DATA,
	AUD_RECORD_PAUSE,
	AUD_RECORD_FINISHED,
}enum_aud_record_state;


typedef enum
{
	AUD_PLAYER_ERROR = -1,
	AUD_PLAYER_START = 0,
	AUD_PLAYER_DATA_COMPLETE,
	AUD_PLAYER_FINISHED,
    AUD_PLAYER_ERR_ENUM_32_BIT                = 0x7FFFFFFF //32bit enum compiling enforcement
}enum_aud_player_state;


typedef int(*cb_on_record)(char *p_data, int len, enum_aud_record_state state);
typedef int(*cb_on_player)(enum_aud_player_state state);

typedef struct MC_AudioPoC
{
    void *poc;  // 由底层创建和释放
    bool duplexmode; // 底层保存, 可以不设置
    uint32_t samplerate; // 设置播放和录音采样率,只支持8000 和 16000
    uint8_t format;     //format: 0 PCM, 1 WAV, 2 amr      
    cb_on_record audrec_cb; //record callback,上报录音数据
    cb_on_player audplay_cb;//play callback 上报播放状态
}MC_AudioPoC_t;

extern int AudioSetChannel(MERCURY_AUDIO_MODE_TYPE_E channel);
extern MERCURY_AUDIO_MODE_TYPE_E AudioGetChannel(void);
extern int AudioSetVolume(DWORD volume);
extern int AudioGetVolume(void);
extern int AudioDtmfPlay(MERCURY_DTMF_TONE_ID_E tone,BYTE time);
extern void AudioDtmfAbort(void);
extern void AudioDtmfVolume(uint32 volume);
extern int AudioSingleTonePlay(uint32 freq,BYTE time);
extern void AudioSingleToneAbort(void);
extern void AudioSingleToneVolume(uint32 volume);
extern int AudioSetPaType(MERCURY_AUD_SPKPA_TYPE_T type);
extern MERCURY_AUD_SPKPA_TYPE_T AudioGetPaType(void);

/******************************************************
设置外部PA
param[in] enable: true enable, false disable
param[in] gpio_id: gpio id
param[in] level: 1, high level enable PA, 0 low level enable PA
******************************************************/
extern void AudioSetExtPA(bool enable, uint8_t gpio_id, uint8_t level);

/****************
param[in] type: 1 mic record; 2 voice record
param[in] format: 0 PCM, 1 WAV, 2 amr
param[in] file_name: 如果为空， 则录音到buffer, 通过回调函数把数据发送给APP
param[in] timeout: 录制时间, 0 则有stop 停止录音,单位:S
param[in] aud_cb: callback

***************/
int AudioRecordStart(int type, int format, uint32_t timeout, char *file_name, cb_on_record aud_cb);
int AudioRecordStop(void);
int AudioRecordPause(void);
int AudioRecordResume(void);


/********************************************************************************
开启AudioPoc 功能
param[in] handle: 详见MC_AudioPoC_t 说明
param[in] fullduplex:  true 则开启 全双工模式, 开始之后马上开始录音, 
录音数据在有设置callback 时实时上报,如果没有callback 则通过AudioPoCReadData可读
并且随时可以通过AudioPoCFillData写入播放数据进行播放

false: 开启半双工模式, 通过AudioPoCHalfModeSwitch 切换录音和播放单独工作

return : AUDIOHAL_ERR_CODE_T
************************************************************************************/
int AudioPoCStart(MC_AudioPoC_t *handle, bool fullduplex);

/*************************************************
停止audioPoC功能

param[in] handle: 详见MC_AudioPoC_t 说明
return : AUDIOHAL_ERR_CODE_T
**************************************************/
int AudioPoCStop(MC_AudioPoC_t *handle);

/******************************************************
切换半双工模式
param[in] handle: 详见MC_AudioPoC_t 说明
param[in] halfmode:  false 开始录音 true : 播放模式,AudioPoCFillData填充播放数据
return : AUDIOHAL_ERR_CODE_T
******************************************************/
int AudioPoCHalfModeSwitch(MC_AudioPoC_t *handle, bool halfmode);

/******************************************************
填充播放数据
param[in] handle: 详见MC_AudioPoC_t 说明
param[in] chBuf:  data buffer
param[in] nSize:  data len
return : AUDIOHAL_ERR_CODE_T
******************************************************/
int AudioPoCFillData(MC_AudioPoC_t *handle, uint8_t * chBuf, uint32 nSize);

/******************************************************
读取录音数据,没有设置录音callback 时由此接口读数据, 有设置,该接口无数据
param[in] handle: 详见MC_AudioPoC_t 说明
param[in] chBuf:  read data buffer
param[in] nSize:  read data len
return : AUDIOHAL_ERR_CODE_T
******************************************************/
int AudioPoCReadData(MC_AudioPoC_t *handle, uint8_t * chBuf, uint32 nSize);


/*******************************************
set extPA start delay
*******************************************/
void AudioSetExtPADelay(uint16_t delay_ms);

#endif //__MERCURY_AUDIO_H__

