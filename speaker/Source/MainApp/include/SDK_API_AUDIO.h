#ifndef _QYY_API_AUDIO_H
#define _QYY_API_AUDIO_H

#include "MercuryDef.h"

#define AUDIO_TYPE_TTS  1
#define AUDIO_TYPE_AMR  2
#define AUDIO_TYPE_PCM  3
#define AUDIO_TYPE_MP3  4
#define AUDIO_TYPE_WAV  5

extern HANDLE g_event_audio_complete;
/***************************************************
 *function  : enable or disable PA, set TRUE before audio play, set FALSE after audio play
 *parameter : 
 *return    :
 * *************************************************/
void sdk_PA_ENABLE( BOOL enable );

/***************************************************
 *function  : audio play
 *parameter : tts-1 use TTS, 0 not use
 *return    :  0 indicate success, -1 indicate fail
 * *************************************************/
int sdk_voiceplay( int tts, const char *message );

/***************************************************
 *function  : register TTS play complete callback
 *parameter : 
 *return    :
 * *************************************************/
void sdk_reg_tts_complete_callback( int tts_complete_callback( void ) );

/***************************************************
 *function  : register PCM play complete callback
 *parameter : 
 *return    :
 * *************************************************/
void sdk_reg_pcm_complete_callback( int pcm_complete_callback( void ) );

/***************************************************
 *function  : register AMR play complete callback
 *parameter : 
 *return    :
 * *************************************************/
void sdk_reg_amr_complete_callback( int amr_complete_callback( void ) );

/***************************************************
 *function  : register MP3 play complete callback
 *parameter : 
 *return    :
 * *************************************************/
void sdk_reg_mp3_complete_callback( int mp3_complete_callback( void ) );

/***************************************************
 *function  : play audio file by async mode
 *parameter : full file path, for example: E://testaudio.mp3
 *return    : 0: success -1:fail
 * *************************************************/
int sdk_audio_play_file(const char *filepath);

/***************************************************
 *function  : stop audio playing
 *parameter : 
 *return    : 0: 
 * *************************************************/
int sdk_audio_play_stop(void);

/***************************************************
 *function  : play audio file by sync mode
 *parameter : full file path, for example: E://testaudio.mp3
 *return    : 0: success -1:fail
 * *************************************************/
int sdk_audio_play_file_sync(const char *filepath);

/***************************************************
 *function  : play audio data by sync mode
 *parameter : audio_data: audio data to play
 *            len: length of audio data
 *            type: audio type, value is AUDIO_TYPE_PCM、AUDIO_TYPE_AMR,AUDIO_TYPE_WAV or AUDIO_TYPE_MP3
 *return    : 0: success -1:fail
 * *************************************************/
int sdk_audio_play_data_sync(const char *audio_data, unsigned int len, int type);

/***************************************************
 *function  : play audio data by async mode
 *parameter : audio_data: audio data to play, the memory of audio_data will free by SDK after playing, 
                          application should not free the memory again
 *            len: length of audio data
 *            type: audio type, value is AUDIO_TYPE_PCM、AUDIO_TYPE_AMR,AUDIO_TYPE_WAV or AUDIO_TYPE_MP3
 *return    : 0: success -1:fail
 * *************************************************/
int sdk_audio_play_data(const char *audio_data, unsigned int len, int type);

/***************************************************
 *function  : play audio in nv by async mode
 *parameter : index: nv index stored audio data
 *            len:   audio data length       
 *            	 	 only amr format is supported.
 *return    : 0: success -1:fail
 * *************************************************/
int sdk_audio_play_nv(int index, int len);

/***************************************************
 *function  : play audio gap by async mode
 *parameter : 
 *            ms_gap:   gap time: ms       
 *            	 	 
 *return    : 0: success -1:fail
 * *************************************************/
int sdk_audio_play_gap(int ms_gap);

/***************************************************
 *function  : check whether audio playing
 *parameter : 
 *return    : 1: audio playing 0:none 
 * *************************************************/
BOOL sdk_audio_playing(void);

#endif
