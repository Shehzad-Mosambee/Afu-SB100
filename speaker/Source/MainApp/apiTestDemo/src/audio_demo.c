#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryFile.h"
#include "MercuryMem.h"
#include "SDK_API_AUDIO.h"

#include "speakerapp.h"

void open_audio_file(char *filepath,char *buffer_data, int * pnLen)
{
	char FilePath[256];
	HANDLE fp;
	LONG nmove, nread;
	int file_len;
	char *buff;

	// for file system, file name need to be converted to unicode encoding from ascii encoding
	CharString2UnicodeString(FilePath, filepath);

	//open the audio file
	fp = CreateFile(FilePath,MCFILE_OPERATE_MODE_READ|MCFILE_ACCESS_MODE_OPEN_EXISTING,
            0,NULL,0,0,0);

    if(fp == NULL)
    {
    	MercuryTrace("open audio file fail \n");
    	return;
    }

	//get file size 
	SetFilePointer(fp, 0, &nmove, MCFILE_SEEK_BEGIN);
	file_len = GetFileSize(fp, NULL);
	buff = LocalAlloc(LMEM_ZEROINIT, file_len);
	if (buff)
	{
		// read audio file
		ReadFile(fp, buff, file_len, &nread, NULL);
		memcpy(buffer_data ,buff,file_len);
		//play audio data
		// sdk_audio_play_data_sync(buff, nread, AUDIO_TYPE_MP3);
		LocalFree(buff);
	}
	*pnLen = nread;
	CloseHandle(fp);
}


static BYTE l_pAudioBuff[200 * 1024] = {0};
int l_nAudioLen;
void play_audio_data_sync(void)
{
    MercuryTrace("play amr audio one\r\n");
    memset(l_pAudioBuff,0,sizeof(l_pAudioBuff));
    open_audio_file("E://audio_41001.amr",l_pAudioBuff,&l_nAudioLen);
    sdk_audio_play_data_sync(l_pAudioBuff,l_nAudioLen,AUDIO_TYPE_AMR);

    MercuryTrace("play mp3 audio two\r\n");
    memset(l_pAudioBuff,0,sizeof(l_pAudioBuff));
    open_audio_file("E://audio_41002.mp3",l_pAudioBuff,&l_nAudioLen);
    sdk_audio_play_data_sync(l_pAudioBuff,l_nAudioLen,AUDIO_TYPE_MP3);

    MercuryTrace("play pcm audio three\r\n");
    memset(l_pAudioBuff,0,sizeof(l_pAudioBuff));
    open_audio_file("E://audio_41003.pcm",l_pAudioBuff,&l_nAudioLen);
    sdk_audio_play_data_sync(l_pAudioBuff,l_nAudioLen,AUDIO_TYPE_PCM);

    MercuryTrace("play wav audio four\r\n");
    memset(l_pAudioBuff,0,sizeof(l_pAudioBuff));
    open_audio_file("E://audio_41004.wav",l_pAudioBuff,&l_nAudioLen);
    sdk_audio_play_data_sync(l_pAudioBuff,l_nAudioLen,AUDIO_TYPE_WAV);
}


void play_audio_data_async(void)
{
    char *amrBuffer = LocalAlloc(LMEM_ZEROINIT,5 * 1024);
    char *mp3Buffer = LocalAlloc(LMEM_ZEROINIT,15 * 1024);
    char *pcmBuffer = LocalAlloc(LMEM_ZEROINIT,30 * 1024);
    char *wavBuffer = LocalAlloc(LMEM_ZEROINIT,35 * 1024);

    MercuryTrace("play amr audio one\r\n");
    open_audio_file("E://audio_41001.amr",amrBuffer,&l_nAudioLen);
    sdk_audio_play_data(amrBuffer,l_nAudioLen,AUDIO_TYPE_AMR);

    MercuryTrace("play mp3 audio two\r\n");
    open_audio_file("E://audio_41002.mp3",mp3Buffer,&l_nAudioLen);
    sdk_audio_play_data(mp3Buffer,l_nAudioLen,AUDIO_TYPE_MP3);

    MercuryTrace("play pcm audio three\r\n");
    open_audio_file("E://audio_41003.pcm",pcmBuffer,&l_nAudioLen);
    sdk_audio_play_data(pcmBuffer,l_nAudioLen,AUDIO_TYPE_PCM);

    MercuryTrace("play wav audio four\r\n");
    open_audio_file("E://audio_41004.wav",wavBuffer,&l_nAudioLen);
    sdk_audio_play_data(wavBuffer,l_nAudioLen,AUDIO_TYPE_WAV);
}

void play_audio_file_sync(void)
{
    MercuryTrace("play amr audio one\r\n");
    sdk_audio_play_file_sync("E://audio_41001.amr");

    MercuryTrace("play mp3 audio two\r\n");
    sdk_audio_play_file_sync("E://audio_41002.mp3");

    MercuryTrace("play pcm audio three\r\n");
    sdk_audio_play_file_sync("E://audio_41003.pcm");

    MercuryTrace("play wav audio four\r\n");
    sdk_audio_play_file_sync("E://audio_41004.wav");
}

void play_audio_file_async(void)
{
    MercuryTrace("play amr audio one\r\n");
    sdk_audio_play_file("E://audio_41001.amr");

    MercuryTrace("play mp3 audio two\r\n");
    sdk_audio_play_file("E://audio_41002.mp3");

    MercuryTrace("play pcm audio three\r\n");
    sdk_audio_play_file("E://audio_41003.pcm");

    MercuryTrace("play wav audio four\r\n");
    sdk_audio_play_file("E://audio_41004.wav");
}

void audio_stop(void)
{
    MercuryTrace("play amr audio one\r\n");
    sdk_audio_play_file("E://audio_41001.amr");

    MercuryTrace("play mp3 audio two\r\n");
    sdk_audio_play_file("E://audio_41002.mp3");

    Sleep(3000);
    sdk_audio_play_stop();

    MercuryTrace("play pcm audio three\r\n");
    sdk_audio_play_file("E://audio_41003.pcm");

    MercuryTrace("play wav audio four\r\n");
    sdk_audio_play_file("E://audio_41004.wav");
}

void audio_test(void)
{
    // play audio data sync test
    play_audio_data_sync();

    // play audio data async test
    play_audio_data_async();
    
    // play audio file sync test
    play_audio_file_sync();

    // play audio file async test
    play_audio_file_async();

    // audio stop
    // audio_stop();
}