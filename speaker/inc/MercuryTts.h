/*#########################################################################
 Amoi Tech Mercury Application Build System
 All Rights Reserved 2020
 Author: 
#########################################################################*/

#ifndef _MERCURY_TTS_H_
#define _MERCURY_TTS_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* the range of voice speed value is from -32768 to +32767 */
#define MC_TTS_SPEED_MIN                 -32768      /* slowest voice speed */
#define MC_TTS_SPEED_NORMAL              0           /* normal voice speed (default) */
#define MC_TTS_SPEED_MAX                 +32767      /* fastest voice speed */

/* the range of voice tone value is from -32768 to +32767 */
#define MC_TTS_PITCH_MIN                 -32768      /* lowest voice tone */
#define MC_TTS_PITCH_NORMAL              0           /* normal voice tone (default) */
#define MC_TTS_PITCH_MAX                 +32767      /* highest voice tone */


typedef struct
{
    int SpeakSpeed;
    int Pitch;
}TTS_PARAM_S;



int TTS_Init(void);
int TTS_Deinit(void);
int TTS_SetParams(TTS_PARAM_S* mode);
int TTS_GetParams(TTS_PARAM_S* mode);
int TTS_PlayText(const uint8 * text, int len);
int TTS_Abort(void);

int PCM_StartPlay(void);
int PCM_FillData(uint8 * chBuf,uint32 nSize);
int PCM_StopPlay(void);

int AMR_StartPlay(uint8* pucData, uint32 uiDataLength);
int AMR_StopPlay(void);

int MP3_StartPlay(uint8_t *mp3_buff, uint32_t mp3_size);
int MP3_StopPlay(void);

/*****************************************
与PCM_StartPlay()功能相同, 增加播放samplerate 设置
param[in] samplerate : 支持8000 和 16000
PCM_StartPlay()默认的播放采样率是16000, 
本地录音的数据的采样率为8000 , 故播放录音数据要使用此接口，
并设置采样率为8000. 
其他使用情况照旧。
*****************************************/
int PCM_StartPlay_Ex(uint32_t samplerate);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

