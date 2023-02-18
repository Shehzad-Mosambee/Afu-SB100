#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryFile.h"
#include "MercuryMem.h"
#include "SDK_API_AUDIO.h"

#include "audio.h"
#include "speakerapp.h"

// static BYTE l_pAudioBuff[100 * 1024] = {0};
// static BYTE l_pTempBuff[50 * 1024] = {0};
// int l_nAudioLen;
int l_audioList[] = {
	AUDIO_ONE,
	AUDIO_HUNDRED,
	AUDIO_AND,
	AUDIO_THREE,
	AUDIO_TWENTY,
	AUDIO_TWO,
	NULL
};


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

void _play_one_file(int audioId)
{
	char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};
	// int nAudioType = get_audio_type();

	get_audio_file_name(fileName, audioId);

	sdk_audio_play_file(fileName);
}

void audio_test_play_file_concat(void) {
	for (int i=0; ;i++) {
		int  audioId = l_audioList[i];
		
		_play_one_file(audioId);
	}
}

// const int l_audioList[] = {
// 	11001,
// 	11002,
// 	11003,
// 	-1
// 	// 11004
// 	// 11005
// 	// 11006
// 	// 11007
// 	// 11008
// 	// 11009
// 	// 11010
// 	// 11011
// 	// 11012
// 	// 11013
// 	// 11014
// 	// 11015
// 	// 11016
// 	// 11017
// 	// 11018
// 	// 11019
// 	// 11020
// 	// 11021
// 	// 11022
// 	// 11023
// 	// 11024
// 	// 11025
// 	// 11026
// 	// 11027
// 	// 11028
// 	// 11029
// 	// 11030
// 	// 11031
// 	// 11032
// 	// 11033
// 	// 11034
// 	// 11035
// 	// 21001
// 	// 21002
// 	// 21003
// 	// 21004
// 	// 21005
// 	// 21006
// 	// 21007
// 	// 21008
// 	// 21009
// 	// 31000
// 	// 31001
// 	// 31002
// 	// 31003
// 	// 31004
// 	// 31005
// 	// 31006
// 	// 31007
// 	// 31008
// 	// 31009
// 	// 31010
// 	// 31011
// 	// 31012
// 	// 31013
// 	// 31014
// 	// 31015
// 	// 31016
// 	// 31017
// 	// 31018
// 	// 31019
// 	// 31020
// 	// 31021
// 	// 31022
// };


// int _audio_id_to_nv_item_id(char * audioId) {
// 	#define AUDIO_BASE_NV_ID 		100
// 	int nAudioId = atoi(audioId);

// 	for (int i=0; ;i++) {
// 		int nID = l_audioList[i];
// 		if (nID == nAudioId) {
// 			return AUDIO_BASE_NV_ID + i;
// 		} else if (nID == -1) {
// 			break;
// 		}
// 	}
// 	return -1;
// }

// BOOL read_audio_data_from_nv(char * audioId, BYTE * lpBuffer, int * pnDataLen) {
// 	int nNvId = _audio_id_to_nv_item_id(audioId);
// 	if (nNvId < 0) {
// 		return FALSE;
// 	}

// 	// 从NV 读取数据，填充到缓冲区

// 	return TRUE;
// }

// BOOL read_audio_data(char * audioId, BYTE * lpBuffer, int * pnDataLen) {
// 	int type;
// 	if (type == 1) {
// 		return read_audio_data_from_nv(audioId, lpBuffer, pnDataLen);
// 	} else if (type == 2) {
// 		// get_audio_file_name(fileName, audioId);
// 		// open_audio_file(fileName, pAudioData, &nDataLen);
// 	} else {

// 	}
// }

void _play_one_file_data(int audioId) {
	int nDataLen = 0;
	int MAX_BUFF_SIZE = 50 * 1024;
	char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};
	int nAudioType = get_audio_type();

	BYTE * pAudioData = malloc(MAX_BUFF_SIZE);
	memset(pAudioData, 0, MAX_BUFF_SIZE);

	get_audio_file_name(fileName, audioId);
	open_audio_file(fileName, pAudioData, &nDataLen);

	sdk_audio_play_data(pAudioData, nDataLen, nAudioType);
}

void audio_test_play_data_concat(void) {
	for (int i=0; ;i++) {
		char * audioId = l_audioList[i];
		if (audioId == NULL) {
			break;
		}
		_play_one_file_data(audioId);
	}
}

void audio_test_play_composed_data(void) {
}

//demo for playing audio data
//in this demo, audio data read from file
void audio_test_play_data(int bSync)
{
	char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};
	char FilePath[256];
	HANDLE fp;
	LONG nmove, nread;
	int file_len;
	char *buff;

	// for file system, file name need to be converted to unicode encoding from ascii encoding
	// get_audio_file_name_byid(fileName, AUDIO_CHECK_SIM);
	CharString2UnicodeString(FilePath, fileName);

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
		
		//play audio data
		if (bSync)
		{
			sdk_audio_play_data_sync(buff, nread, AUDIO_TYPE_AMR);
			LocalFree(buff);
		}	
		else
		{
			//内存会在SDK播报完成后自动释放，应用不需要释放内存
			sdk_audio_play_data(buff, nread, AUDIO_TYPE_AMR);
		}
		
	}
	CloseHandle(fp);
}


void audio_test(void)
{
	int audioTypeIndex;
	char *audioTypeList[AUDIO_TYPE_COUNT] = {AUDIO_AMR, AUDIO_PCM, AUDIO_WAV, AUDIO_MP3};
	// play audio file by sync mode, audio format can be pcm、amr、mp3、wav
	// char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};
	// // get_audio_file_name_byid(fileName, AUDIO_DOWNLOAD_FAIL);
	// sdk_audio_play_file_sync(fileName);


	audioTypeIndex = 0; 
	while (1)
	{	
		set_audio_type(audioTypeList[audioTypeIndex % 4]);
		// sdk_audio_play_stop();
    	// MercuryTrace("audio_test_play_file_concat start \n");
		// audio_test_play_file_concat();
		// Sleep(10*1000);

    	MercuryTrace("audio_test_play_data_concat start \n");
		// play audio file by async mode,audio format can be pcm、amr、mp3、wav
		audio_test_play_data_concat();
		Sleep(10*1000);

		audioTypeIndex++;
	}
	

	//// play audio data by sync mode,audio format can be pcm、amr、mp3、wav
	audio_test_play_data(0);
	audio_test_play_data(1);
}

static BYTE l_pAudioBuff[200 * 1024] = {0};
static BYTE l_pTempBuff[50 * 1024] = {0};
int l_nAudioLen;
void audio_test_play_concat_file(void) {
	int nDataLen;
	memset(l_pAudioBuff, 0, sizeof(l_pAudioBuff));
	l_nAudioLen = 0;

	nDataLen = 0;
	open_audio_file("E://audio_41001.pcm", l_pTempBuff, &nDataLen);
	// open_audio_file("E://audio_11005.amr", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;

	nDataLen = 0;
	open_audio_file("E://audio_41014.pcm", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;
	
	nDataLen = 0;
	open_audio_file("E://audio_41027.pcm", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;

	nDataLen = 0;
	open_audio_file("E://audio_41003.pcm", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;

	nDataLen = 0;
	open_audio_file("E://audio_41017.pcm", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;

	nDataLen = 0;
	open_audio_file("E://audio_41012.pcm", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;

	nDataLen = 0;
	open_audio_file("E://audio_41027.pcm", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;

	nDataLen = 0;
	open_audio_file("E://audio_41002.pcm", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;

	nDataLen = 0;
	open_audio_file("E://audio_41036.pcm", l_pTempBuff, &nDataLen);
	memcpy(l_pAudioBuff + l_nAudioLen, l_pTempBuff, nDataLen);
	l_nAudioLen += nDataLen;
	
	sdk_audio_play_data_sync(l_pAudioBuff, l_nAudioLen, AUDIO_TYPE_PCM);
}

#include "nv.h"
extern const audioMapStruct g_audioMapTbl[];

void _write_audio_to_nv(void)
{
	int nDataLen = 0;
	int MAX_BUFF_SIZE = 50 * 1024;
	char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};

	int i,j;
	
	BYTE * pAudioData = LocalAlloc(LMEM_ZEROINIT, MAX_BUFF_SIZE);
	set_audio_type(AUDIO_AMR);

	for(i=0;i<AUDIO_INDEX_MAX;i++)
	{
		NV_Delete(i+NV_AUDIO_START_INDEX);
	}
	
	for (i=0;;i++)
	{
		if (g_audioMapTbl[i].id == AUDIO_INDEX_MAX)
			break;
			
		memset(pAudioData, 0, MAX_BUFF_SIZE);

		for (j=0;;j++)
		{
			if (g_audioMapTbl[j].id == AUDIO_INDEX_MAX)
				break;
			if (i == g_audioMapTbl[j].id)
				break;
		}

		get_audio_file_name(fileName, g_audioMapTbl[j].fileid);
		open_audio_file(fileName, pAudioData, &nDataLen);

		if (0 != NV_Write(i+NV_AUDIO_START_INDEX, nDataLen, pAudioData))
		{
			MercuryTrace("write nv audio file fail:%d\n ", i+NV_AUDIO_START_INDEX);
			break;
		}
		MercuryTrace("write NV audio index:%d, len:%d\n", i+NV_AUDIO_START_INDEX, nDataLen);
	}
	LocalFree(pAudioData);
	
	MercuryTrace("write NV audio success\n");
}

void _write_mp3_audios_to_nv(void)
{
	int nDataLen = 0;
	int MAX_BUFF_SIZE = 50 * 1024;
	char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};

	int i,j;
	
	BYTE * pAudioData = LocalAlloc(LMEM_ZEROINIT, MAX_BUFF_SIZE);
	set_audio_type(AUDIO_MP3);

	for(i=0;i<AUDIO_INDEX_MAX;i++)
	{
		NV_Delete(i+NV_AUDIO_START_INDEX);
	}
	
	for (i=0;;i++)
	{
		if (g_audioMapTbl[i].id == AUDIO_INDEX_MAX)
			break;
			
		memset(pAudioData, 0, MAX_BUFF_SIZE);

		for (j=0;;j++)
		{
			if (g_audioMapTbl[j].id == AUDIO_INDEX_MAX)
				break;
			if (i == g_audioMapTbl[j].id)
				break;
		}

		get_audio_file_name(fileName, g_audioMapTbl[j].fileid);
		open_audio_file(fileName, pAudioData, &nDataLen);

		if (0 != NV_Write(i+NV_AUDIO_START_INDEX, nDataLen, pAudioData))
		{
			MercuryTrace("write nv audio file fail:%d\n ", i+NV_AUDIO_START_INDEX);
			break;
		}
		MercuryTrace("write NV audio index:%d, len:%d\n", i+NV_AUDIO_START_INDEX, nDataLen);
	}
	LocalFree(pAudioData);
	
	MercuryTrace("write NV audio success\n");
}

int test_audio_nv(int index)
{
	int size;
	char *buff = NULL;

	size = get_nv_audio_data_len(index);

	buff = LocalAlloc(LMEM_ZEROINIT, size);
	if (buff == NULL)
	{
		MercuryTrace("malloc nv audio fail, len:%d\n", size);
		return -1;
	}
	if (0 != NV_Read(index+NV_AUDIO_START_INDEX, size, buff))
	{
		MercuryTrace("get nv audio data fail\n");
		LocalFree(buff);
		return -1;
	}

	MercuryTrace("Read NV success\n");
	LocalFree(buff);
	return 0;
}


