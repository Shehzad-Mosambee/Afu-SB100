#include "MercuryPlat.h"
#include "MercuryDef.h"
#include "MercuryMem.h"
#include "MercuryFile.h"
#include "MercuryThread.h"
#include "MercuryNv.h"

#include "audio.h"
#include "app_config.h"
#include "nv.h"
#include "cjson.h"
#include "speakerapp.h"


// ---------------------------------------------------------------------------------------------------------- //

// #define HOST_ADDR "http://101.132.112.165:10081"
// #define HOST_ADDR "http://114.143.22.141:10081"
// #define AUDIO_HOST_ADDR "https://test.mosambee.in"

// // #define AUDIO_RESOURCE_COUNT_URL "/szanfu-admin/audio/count?productId=a1TrHYpY5LD&version=1.3"
// // #define AUDIO_FILE_INFO_URL "/szanfu-admin/audio/info?productId=a1TrHYpY5LD&version=1.3&index="
// #define AUDIO_RESOURCE_COUNT_URL "/szanfu-admin/audio/count?productId=a1TrHYpY5LD&version="##MOS_AUDIO_VERSION
// #define AUDIO_FILE_INFO_URL "/szanfu-admin/audio/info?productId=a1TrHYpY5LD&version="##MOS_AUDIO_VERSION##"&index="

// // #define AUDIO_RESOURCE_URL "/iot/resources/audio/list?productId=a1TrHYpY5LD&version=1.4"
// // #define AUDIO_RESOURCE_LIST "/szanfu-admin/audio/download?productId=a1TrHYpY5LD&version=1.3&audioId="
// #define AUDIO_RESOURCE_URL "/iot/resources/audio/list?productId=a1TrHYpY5LD&version="##MOS_AUDIO_VERSION
// #define AUDIO_RESOURCE_LIST "/szanfu-admin/audio/download?productId=a1TrHYpY5LD&version="##MOS_AUDIO_VERSION##"&audioId="

// #define AUDIO_INFO_URL "/speaker/audio-res/info"


#define AUDIO_BUFF_SIZE (32*1024)

typedef struct 
{
	int format;
	int len;
	char ver[8+1];
	char url[128+1];
}AudioResStruct;

int audio_update = 0;
AudioResStruct *pAudioUpdate = NULL;


// ---------------------------------------------------------------------------------------------------------- //




int get_nv_audio_data_len(int nAudioId) {
	int nLen = NV_GetItemidDataLen((WORD)nAudioId + NV_AUDIO_START_INDEX);
	return nLen;
}

void get_audio_fileId(audioMapStruct *audioMapTbl)
{
	for (int i = 0; ; i++)
	{
		if (audioMapTbl[i].fileid != 0)
			MercuryTrace("%d, ",atoi(audioMapTbl[i].fileid));
		if (audioMapTbl[i].tts == NULL)
			break;
	}
	MercuryTrace("\r\n");
}

const audioMapStruct g_audioMapTbl[] =
{
	/*
	// {AUDIO_DEVICE_STARTING_UP,     71001, "��ӭʹ��������"},
	{AUDIO_DEVICE_STARTING_UP,     71001, "Welcome"},
	// {AUDIO_DEVICE_SHUTDOWN,        71002, "��лʹ�������䣬�´��ټ�"},
	{AUDIO_DEVICE_SHUTDOWN,        71002, "Shutting Down"},
	// {AUDIO_DEVICE_REBOOTING,       51026, "�豸������������"},
	{AUDIO_DEVICE_REBOOTING,       51026, "Rebooting Now"},
	
	{AUDIO_MQTT_SUBSCRIBE_SUCCESS,   51028, "��Ϣ���ĳɹ�"},
	{AUDIO_CHECK_SIM,                21003, "�����ֻ���"},
	{AUDIO_MQTT_SUBSCRIBE_FAIL,      11027, "��Ϣ����ʧ��"},
	{AUDIO_MQTT_CONNECT,             51030, "MQTT���ӳɹ�"},
	{AUDIO_MQTT_SERVER_CONNECT,      51010, "���������ӳɹ�"},

	{AUDIO_DOWNLOAD_FAIL,          11003, "����ʧ��"},

	{AUDIO_WIFI_CONNECT_SUCCESS,   51025, "�����������ӳɹ�"},
	{AUDIO_WIFI_NETWORK_INIT,	   51022, "���ڳ�ʼ����������"},
	{AUDIO_WIFI_NETWORK_INIT_FAIL, 11008, "���������ʼ��ʧ��"},
	// {AUDIO_NO_WIFI_CONFIG,         51023, "û��������������"},
	{AUDIO_WIFI_NETWORK_CONNETC,   51009, "����������������"},
	{AUDIO_NETWORK_CONFIG_FAIL,    11011,  "�������û�ȡʧ��"},
	{AUDIO_CONFIG_COMPLETE,	       51006,  "�������"},
	{AUDIO_WIFI_NETWORK_CHECK_COMPLETED,  51043, "�������������"},


	{AUDIO_VOLUME_UP,          31002, "������"},
	{AUDIO_VOLUME_DOWN,        31003, "������"},
	{AUDIO_MAXIMUM_VOLUME,     31004, "�������"},
	{AUDIO_MINIMUM_VOLUME,     31005, "������С"},
	{AUDIO_VOLUME_UP_BUTTON,   51039, "�����Ӽ�"},
	{AUDIO_VOLUME_DOWN_BUTTON, 51040, "��������"},
	{AUDIO_REPLAY_KEY_BUTTON,   51041, "�ز���"},
	{AUDIO_POWER_BUTTON,        51042, "��Դ��"},
	
	{AUDIO_SIGNAL_STRENGTH,     51047, "�ź�ǿ��"},
	{AUDIO_BATTERY,             51001, "��ص���"},
	{AUDIO_PERCENT,             51002, "�ٷ�֮"},
	{AUDIO_MOBILE_NETWORK,      51003, "��������������"},
	{AUDIO_WIRELESS_NETWORK,    51004, "��������������"},
	{AUDIO_NO_WIRELESS_NETWORK_CONFIGURED,  51023, "û�����õ���������"},

	{AUDIO_REACH_THE_MOBILE_NETWORK,     31011, "�е���������"},
	{AUDIO_REACH_THE_WIRELESS_NETWORK,   31012, "�е���������"},
	{AUDIO_MOBLILE_NETWORK_CONNECTING,   51008, "����������������"},
	{AUDIO_MOBLILE_NETWORK_CONNECTED,    51024, "�����������ӳɹ�"},

	{AUDIO_PAY_WECHAT,    61006, "΢��֧��"},
	
	{AUDIO_ONE,    41001, "1"},
	{AUDIO_TWO,    41002, "2"},
	{AUDIO_THREE,  41003, "3"},
	{AUDIO_FOUR,   41004, "4"},
	{AUDIO_FIVE,   41005, "5"},
	{AUDIO_SIX,    41006, "6"},
	{AUDIO_SEVEN,  41007, "7"},
	{AUDIO_EIGHT,  41008, "8"},
	{AUDIO_NINE,   41009, "9"},
	{AUDIO_TEN,    41013, "ʮ"},
	{AUDIO_ZERO,   41010, "��"},
	{AUDIO_HUNDRED,   41014, "��"},
	{AUDIO_THOUSAND,  41015, "ǧ"},
	{AUDIO_MILLION,   41037, "����"},
	{AUDIO_AND,       41012, "��"},

	{AUDIO_ELEVN, 	  41018, "ʮһ"},
	{AUDIO_TWELVE,	  41019, "ʮ��"},
	{AUDIO_THIRTEEN,  41020, "ʮ��"},
	{AUDIO_FOURTEEN,  41021, "ʮ��"},
	{AUDIO_FIFTEEN,	  41022, "ʮ��"},
	{AUDIO_SIXTEEN,	  41023, "ʮ��"},
	{AUDIO_SEVENTEEN, 41024, "ʮ��"},
	{AUDIO_EIGHTEEN,  41025, "ʮ��"},
	{AUDIO_NINETEEN,  41026, "ʮ��"},	
	
	{AUDIO_TWENTY,    41027, "��ʮ"},
	{AUDIO_THIRTY,    41028, "��ʮ"},
	{AUDIO_FOURTY,    41029, "��ʮ"},
	{AUDIO_FIFTY,     41030, "��ʮ"},
	{AUDIO_SIXTY,     41031, "��ʮ"},
	{AUDIO_SEVENTY,   41032, "��ʮ"},
	{AUDIO_EIGHTY,    41033, "��ʮ"},
	{AUDIO_NINETY,    41034, "��ʮ"},	


	{AUDIO_POINT,     41011, "��"},
	{AUDIO_DOLLAR,    41016, "Ԫ"},
	{AUDIO_DOLLARS,   41017, "Ԫ"},
	{AUDIO_CENT,      41035, "��"},
	{AUDIO_CENTS,     41036, "��"},

	{AUDIO_OTA_UPDATE,			51013, "OTA����"},
	{AUDIO_UPGRADE_FILE_DL_SUC, 51000, "�����ļ��������"},
	{AUDIO_UPGRADE_DL_FAIL,		11021, "��������ʧ��"},
	{AUDIO_START_UPGRADE,		51014, "������ʼ"},
	{AUDIO_WAIT_UPGRADING,		51015, "�����������Ժ�"},
	{AUDIO_UPDATE_FAIL,			11022, "���µ�ǰ�汾ʧ��"},
	
	// TODO: ��Ӣ�Ĳ���ȫƥ�䡣Ӣ����Ҫ�滻
	{AUDIO_NO_UPGRADING,		0, "�����汾����Ҫ����"},
	{AUDIO_UPDATE_SUC,			21002, "�̼������ɹ�"},

	{AUDIO_ONLY_WIRELESS_NETWORKS,		51016, "�豸��֧����������"},
	{AUDIO_ONLY_MOBILE_NETWORKS,		51017, "�豸��֧���ƶ�����"},

	{AUDIO_WIFI_AKCFG,		  0, "΢�ſ�������ģʽ"},
	{AUDIO_WIFI_APCFG,		  0, "�������ȵ�����ģʽ"},
	{AUDIO_GET_SERV_CONFIG,	  0, "���ڻ�ȡ������Ϣ"},
	{AUDIO_GET_SERVCFG_FAIL,	  0, "������Ϣ��ȡʧ��"},
	{AUDIO_GET_SERVCFG_SUCC,	  0, "������Ϣ��ȡ�ɹ�"},

	{AUDIO_MQTT_SERVER_FAIL,	  0, "����������ʧ��"},

	{AUDIO_BAT_POWEROFF,	  0, "�豸�����ͣ������ػ�"},
	{AUDIO_BAT_POWEROFF2,	  0, "��лʹ�ã��´��ټ�"},
	{AUDIO_BAT_WARNING,	      0, "�������㣬�뼰ʱ���"},
	{AUDIO_CHARGER_IN,	      0, "���ڳ��"},	
	{AUDIO_CHARGER_OUT,	      0, "����Ƴ�"},

	*/
	{AUDIO_INDEX_MAX, 0, NULL},
	
};

int app_get_audio_type(void)
{
	return app_GetAudioType();
}

int app_audio_play_file(int fileId)
{
	char filepath[MAX_AUDIO_FILE_NAME_LEN] = {0};

	get_audio_file_name(filepath, fileId);
	return sdk_audio_play_file(filepath);
}

int app_audio_tts_callback(void)
{
    //Ϩ��led
    // anfu_led_off();
	return 0;
}

int app_audio_amr_callback(void)
{

    //Ϩ��led
    // anfu_led_off();


	bcs_clear_timer_reset();
	return 0;
}


int app_audio_mp3_callback(void)
{
	bcs_clear_timer_reset();
	return 0;
}


int app_audio_play(int index)
{
	// int i;
	
	// for (i=0;;i++)
	// {
	// 	if (g_audioMapTbl[i].id == AUDIO_INDEX_MAX)
	// 		break;
	// 	if (index == g_audioMapTbl[i].id)
	// 		break;
	// }

	// if (i == AUDIO_INDEX_MAX)
	// {
	// 	MercuryTrace("not find audio\n");
	// 	return -1;
	// }
	
	// MercuryTrace("app_GetAudioType %d\n", app_GetAudioType());
	
    // //����led
    // anfu_led_on();

	// if (app_GetAudioType() == AUDIO_TYPE_TTS)
	// {
	// 	return // sdk_voiceplay(1, g_audioMapTbl[i].tts);
	// }
	// else if(app_GetAudioType() != AUDIO_TYPE_TTS && !app_IsExistExFlash())
	// {
	// 	MercuryTrace("app audio play nv, index:%d, len:%d\n", index+NV_AUDIO_START_INDEX, get_nv_audio_data_len(index));
	// 	if (g_audioMapTbl[i].fileid != 0)
	// 		return sdk_audio_play_nv(index+NV_AUDIO_START_INDEX, get_nv_audio_data_len(index));
	// }
	// else
	// {
	// 	if (g_audioMapTbl[i].fileid != 0)
	// 		return app_audio_play_file(g_audioMapTbl[i].fileid);
	// }
}


struct audio_err
{
	int err;
	char *tts;
};

const struct audio_err g_audioErrTbl[] =
{
	{1001, "SN������,����ϵ�ͷ�"},
	{1002, "����Ϊ���°汾,����Ҫ����"},
	{1003, "�������ܾ�����"},
	{1004, "ϵͳæ"},
	{1005, "�Ҳ������ذ汾"},
	{1006, "ժҪ�������"},
	{1007, "�豸�洢�ռ䲻��"},
	{1008, "���ݶ������ʧ��"},
	{1009, "����ʧ��"},
	{1010, "����������Ϣ����"},
	{1011, "������������Ч"},
	{1099, "����ӿڲ���ʶ��"},
	{0, NULL},
};
void app_play_error_code(int code)
{
	int i;

	for (i=0;;i++)
	{
		if (g_audioErrTbl[i].err == 0)
			break;
		
		if (g_audioErrTbl[i].err == code)
		{
			// // sdk_voiceplay(1, g_audioErrTbl[i].tts);
			break;
		}
	}
}


// ---------------------------------------------------------------------------------------------------------- //

void start_audio_upgrade_thread(void)
{
	CreateThread(NULL,8*1024,app_audio_check_update,0,NULL,STACK_SIZE_RESERVATION,NULL);
}


int app_audio_check_update()
{
	cJSON *root = NULL;
	char sn[32] = {0};
	char ver[8] = {0};
	char *sSend = NULL;
	
	int ret = 0;
	char *sbuff;
	char *rbuff;
	int recv_len = 0;  

//	MERCURY_HEAP_STATUS pHeapStatus;
//
//	GetAppHeapUsageStatus(&pHeapStatus);
//	MercuryTrace("enter app_audio_check_update, heapusedsize:%d\n", pHeapStatus.nUsedSize);

	audio_update = 0;

	NV_Read(NV_SN, 32, sn);
	NV_Read(NV_AUDIO_VERSION, 8, ver);
	if(ver[0] == 0)
	{
		strcpy(ver, MOS_AUDIO_VERSION);
	}

	int ntype = app_GetAudioType();

	// strcpy(sn, "AF10099016");
	// strcpy(ver,"1.2");
	// int ntype = AUDIO_TYPE_AMR;

//	root = cJSON_CreateObject();
//	cJSON_AddStringToObject(root, "dev-sn", sn);
//	cJSON_AddNumberToObject(root, "voice-format", ntype);
//	cJSON_AddNumberToObject(root, "version", ver);
//	sSend = cJSON_Print(root);
//	MercuryTrace("app_audio_check_update req:%s\n", sSend);
//	cJSON_Delete(root);

	//malloc memory to store data received from http server
	// buff = LocalAlloc(LMEM_ZEROINIT, 16*1024);
	sbuff = LocalAlloc(LMEM_ZEROINIT, 164);
	rbuff = LocalAlloc(LMEM_ZEROINIT, 1024);
	// strcpy(buff, AUDIO_HOST_ADDR);
	// strcat(buff, AUDIO_RESOURCE_URL);

	// strcat(buff,"/szanfu-admin/audio/list?voiceFormat=2&version=1.2&devSn=AF10099016");
	
	// sprintf(sbuff,"%s/szanfu-admin/audio/list?voiceFormat=%d&version=%s&devSn=%s", AUDIO_HOST_ADDR, ntype, MOS_AUDIO_VERSION, sn);
	// sprintf(sbuff,"%s/szanfu-admin/audio/list?voiceFormat=%d&version=%s&devSn=%s", AUDIO_HOST_ADDR, ntype, ver, sn);
	sprintf(sbuff,"%s/szanfu-admin/audio/list?version=%s&devSn=%s", AUDIO_HOST_ADDR, ver, sn);

	// AppInfoStruct app_info = {0};
	// if (0 != NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info))
	// {
	// 	MercuryTrace("app_audio_check_update, fs_read_deviceinfo fail\n");
	// 	LocalFree(sbuff);
	// 	LocalFree(rbuff);
	// 	return -1;
	// }
	// sprintf(sbuff,"%s/szanfu-admin/audio/list?version=%s&devSn=%s&model=%s", AUDIO_HOST_ADDR, ver, sn, app_info.model);




	MercuryTrace(">>>>>>>>>> Audio TMS Endpoint: %s\r\n", sbuff);

	//get data by http-get-mode		
//	ret = sdk_http_get(buff, sSend, NULL, buff, 16*1024, &recv_len);
	ret = sdk_http_get(sbuff, NULL, NULL, rbuff, 16*1024, &recv_len);
//	LocalFree(sSend);
	
	MercuryTrace("http buff :%s,get len:%d\n", rbuff, recv_len);
	//http get success
	if (ret != 0)  
	{
		LocalFree(sbuff);
		LocalFree(rbuff);
		return -1;
	}
	
	cJSON *j_rsp = NULL;
	cJSON *j_errcode = NULL;
	
	j_rsp = cJSON_Parse(rbuff);	
	LocalFree(rbuff);
	LocalFree(sbuff);
	
	if (j_rsp == NULL)
	{
		return -1;
	}
	j_errcode = cJSON_GetObjectItem(j_rsp, "error-code");
	if (j_errcode == NULL || j_errcode->valueint != 0)
	{
		cJSON_Delete(j_rsp);
		return -1;
	}
	cJSON *j_temp = NULL;
	
	if (pAudioUpdate)
	{
		LocalFree(pAudioUpdate);
		pAudioUpdate = NULL;
	}
	pAudioUpdate = LocalAlloc(LMEM_ZEROINIT, sizeof(AudioResStruct));
	j_temp = cJSON_GetObjectItem(j_rsp, "voice-format");
	if(j_temp)
	{
		pAudioUpdate->format = j_temp->valueint;
	}
	j_temp = cJSON_GetObjectItem(j_rsp, "latest-version");
	if(j_temp)
	{
		memset(pAudioUpdate->ver, 0x00, sizeof(pAudioUpdate->ver));
		strcpy(pAudioUpdate->ver, j_temp->valuestring);
	}

	cJSON_Delete(j_rsp);
	// MercuryTrace("pAudioUpdate->ver is %lf and atof(ver) is %lf ",pAudioUpdate->ver, atof(ver));
	// MercuryTrace("pAudioUpdate->ver is %lf and MOS_AUDIO_VERSION is %lf ",pAudioUpdate->ver, atof(MOS_AUDIO_VERSION));

	MercuryTrace("pAudioUpdate->ver is %s and APPLICATION AUDIO VERSION is %s\r\n",pAudioUpdate->ver, ver);
	
	//If the audio version on the server is larger than the local version, it needs to be updated
	// if (pAudioUpdate->ver > atof(ver)) 
	// if (pAudioUpdate->ver > atof(MOS_AUDIO_VERSION)) 
	if (strcmp(pAudioUpdate->ver, ver))
	{
		play_audio_by_name("audUpd", (unsigned char)0);

		audio_update = 1;
		app_audio_get_list_from_server(ntype);
	}
	
	LocalFree(pAudioUpdate);
	pAudioUpdate = NULL;

	
//	GetAppHeapUsageStatus(&pHeapStatus);
//	MercuryTrace("leave app_audio_check_update, heapusedsize:%d\n", pHeapStatus.nUsedSize);
	return 0;	
}



int app_audio_get_list_from_server(int nformat)
{
	cJSON *root = NULL, *j_payload = NULL, *j_rsp = NULL, *j_temp = NULL;
	int ret = 0;
	char *buff, *sbuff;
	int recv_len = 0;

	//malloc memory to store data received from http server
	sbuff = LocalAlloc(LMEM_ZEROINIT, 164);
	buff = LocalAlloc(LMEM_ZEROINIT, AUDIO_BUFF_SIZE);
	
	//get data by http-get-mode
	// sprintf(sbuff, "%s%s", AUDIO_HOST_ADDR, AUDIO_RESOURCE_COUNT_URL);
	sprintf(sbuff, "%s%s%s", AUDIO_HOST_ADDR, AUDIO_RESOURCE_COUNT_URL_BASE, pAudioUpdate->ver);

	MercuryTrace("File Count URL: %s", sbuff);
	ret = sdk_http_get(sbuff, NULL, NULL, buff, 32*1024, &recv_len);
	
	LocalFree(sbuff);

	MercuryTrace("http get len:%d, %s\n", recv_len, buff);
	//http get success
	if (ret != 0)  
	{
		LocalFree(buff);
		return -1;
	}
	
	j_rsp = cJSON_Parse(buff);	
	LocalFree(buff);
	buff = NULL;
	
	if (j_rsp == NULL)
	{
		return -1;
	}
	j_temp = cJSON_GetObjectItem(j_rsp, "code");
	if (j_temp == NULL || j_temp->valueint != 0)
	{
		cJSON_Delete(j_rsp);
		return -1;
	}
		
	j_payload = cJSON_GetObjectItem(j_rsp, "payload");
	if (j_payload == NULL)
	{
		cJSON_Delete(j_rsp);
		return -1;
	}

	if (0 != app_audio_download_from_server(j_payload->valueint))
	{
		MercuryTrace("audio download failed\r\n");
		return -1;
	}

	set_audio_type_int(nformat);

	cJSON_Delete(j_rsp);
	return 0;
}


int app_audio_download_from_server(int count)
{
	int ret = 0, i, j;
	char *data = NULL;
	char *buff = NULL;
	char audioId[64], fileName[64], filePath[64];
	int olen = 0;

	buff = LocalAlloc(LMEM_ZEROINIT, AUDIO_BUFF_SIZE);
	data = LocalAlloc(LMEM_ZEROINIT, AUDIO_BUFF_SIZE);
	if(buff == NULL || data == NULL)
		goto exit;

	for (i=0; i<count; i++)
	{
		//Get individual file information based on index
		memset(buff, 0,  AUDIO_BUFF_SIZE);
		ret = app_audio_get_file_info(i, buff, AUDIO_BUFF_SIZE, fileName, audioId);
		if (ret != 0)
			goto exit;

		// //Query if the filename is in the list
		// for (j=0;j<AUDIO_INDEX_MAX;j++)
		// {
		// 	sprintf(filePath, "%d.amr", g_audioMapTbl[j].fileid);
		// 	if (0 == strcmp(filePath,fileName))
		// 		break;
		// }
		// if (j == AUDIO_INDEX_MAX) // If the filename is not in the list, no file data will be retrieved
		// {
		// 	MercuryTrace("not find match index:%s\n", fileName);
		// 	continue;
		// }

		//Get the audio file data
		ret = app_audio_get_file_data(buff, data,AUDIO_BUFF_SIZE, audioId, &olen);
		if (ret != 0)
			goto exit;
		//Save data to NV or external flash
		if (app_IsExistExFlash()) //exist extern flash
		{
			char FilePath[256];
			HANDLE fp;
			//char filename[MAX_AUDIO_FILE_NAME_LEN] = {0};
		
			//get_audio_file_name(filename, fileid);
			sprintf(filePath, "E://%s", fileName);
			MercuryTrace("filePath:%s\r\n",filePath);
			
			// for file system, file name need to be converted to unicode encoding from ascii encoding
			// pointer to the name of file,it must be Unicode string
			CharString2UnicodeString(FilePath, filePath);
		
			//create the audio file
			fp = CreateFile(FilePath,MCFILE_OPERATE_MODE_WRITE|MCFILE_ACCESS_MODE_CREATE_ALWAYS,
					0,NULL,0,0,0);
		
			if(fp == NULL)
			{
				MercuryTrace("create audio file fail \n");
				return -1;
			}
			LONG nWrite;
			WriteFile(fp, data, olen, &nWrite, NULL);
			if (nWrite != olen)
			{
				MercuryTrace("Write audio file fail,id:%s datalen:%d, writelen:%d\n", fileName, olen, nWrite);
			}else{
				MercuryTrace("write audio file success:%s\n", fileName);
			}
			CloseHandle(fp);
		}
		else
		{
			NV_Delete(g_audioMapTbl[j].id+NV_AUDIO_START_INDEX);
			if (0 != NV_Write(g_audioMapTbl[j].id+NV_AUDIO_START_INDEX, olen, data))
			{
				MercuryTrace("FotaUpgrade_GetSpace : %d\r\n",FotaUpgrade_GetSpace());
				MercuryTrace("GetLastError: %x\r\n",GetLastError());
				MercuryTrace("!!!!!!!!!!!!!!!!!!!write nv fail, nvindex:%d id:%s\n\n\n", g_audioMapTbl[j].id+NV_AUDIO_START_INDEX, fileName);
			}
			else
			{
				MercuryTrace("#################write nv success, nvindex:%d id:%s, len:%d\n\n\n", g_audioMapTbl[j].id+NV_AUDIO_START_INDEX, fileName, olen);
				MercuryTrace("FotaUpgrade_GetSpace : %d\r\n",FotaUpgrade_GetSpace());
			}
		}		

	}

	if (buff)
	{
		LocalFree(buff);
		buff = NULL;
	}
	
	if (data)
	{
		LocalFree(data);
		data = NULL;
	}

	NV_Delete(NV_AUDIO_VERSION);
	NV_Write(NV_AUDIO_VERSION, 8, pAudioUpdate->ver);

	// play_audio_by_name("sUpdS", (unsigned char)0);
	play_audio_by_name("audUpdS", (unsigned char)0);
	return 0;
exit:
	if (buff)
		LocalFree(buff);
	if (data)
		LocalFree(data);

	MercuryTrace("get file info fail____________");
	// play_audio_by_name("sUpdF", (unsigned char)0);
	play_audio_by_name("audUpdF", (unsigned char)0);
	return -1;
}




int app_audio_get_file_info(int index, char *buff, int buff_len, char *filename, char *audioid)
{
	int ret;
	int recv_len;
	cJSON *j_rsp = NULL, *j_errcode = NULL;
	cJSON *j_payload = NULL, *j_id = NULL, *j_fileName = NULL;
	
	// sprintf(buff, "%s%s%d", AUDIO_HOST_ADDR, AUDIO_FILE_INFO_URL, index+1);
	sprintf(buff, "%s%s%s%s%d", AUDIO_HOST_ADDR, AUDIO_FILE_INFO_URL_BASE, pAudioUpdate->ver, AUDIO_FILE_INFO_URL_IDX, index+1);

	MercuryTrace("Get File Info URL: %s\r\n", buff);

	//get data by http-get-mode
	ret = sdk_http_get(buff, NULL, NULL, buff, buff_len, &recv_len);
	
	//http get success
	if (ret != 0)  
	{
		MercuryTrace("download audio fail:%d\n", ret);
		return -1;
	}
	
	MercuryTrace("get audio info success:%s\n", buff);
	j_rsp = cJSON_Parse(buff);	
	
	if (j_rsp == NULL)
		goto exit;

	j_errcode = cJSON_GetObjectItem(j_rsp, "code");
	if (j_errcode == NULL || j_errcode->valueint != 0)
		goto exit;

	j_payload = cJSON_GetObjectItem(j_rsp, "payload");
	if (j_payload == NULL)
		goto exit;

	j_fileName = cJSON_GetObjectItem(j_payload, "fileName");
	j_id = cJSON_GetObjectItem(j_payload, "audioId");
	if (j_fileName == NULL || j_id == NULL)
		goto exit;

	strcpy(filename, j_fileName->valuestring);
	strcpy(audioid, j_id->valuestring);

	cJSON_Delete(j_rsp);
	return 0;
exit:
	if (j_rsp)
		cJSON_Delete(j_rsp);
	return -1;
}



int app_audio_get_file_data(char *recv_buff, char *out_buff, int buff_len, char *audioId, int *olen )
{
	int recv_len;
	int ret;
	cJSON *j_rsp = NULL, *j_errcode = NULL;
	cJSON *j_temp = NULL;
	int length;
		
	// sprintf(recv_buff, "%s%s%s", AUDIO_HOST_ADDR, AUDIO_RESOURCE_LIST,audioId);
	sprintf(recv_buff, "%s%s%s%s%s", AUDIO_HOST_ADDR, AUDIO_RESOURCE_LIST_BASE, pAudioUpdate->ver, AUDIO_RESOURCE_LIST_IDX, audioId);
	
	recv_len = 0;
	MercuryTrace("download url:%s\n", recv_buff);
	ret = sdk_http_get(recv_buff, NULL, NULL, recv_buff, buff_len, &recv_len);

	if (ret != 0)  
	{
		MercuryTrace("download audio fail:%s\n", audioId);
		goto exit;
	}

	j_rsp = cJSON_Parse(recv_buff);	
	
	if (j_rsp == NULL)
		goto exit;
	
	j_errcode = cJSON_GetObjectItem(j_rsp, "error-code");
	if (j_errcode == NULL || j_errcode->valueint != 0)
		goto exit;
	
	j_temp = cJSON_GetObjectItem(j_rsp,"length");
	if (j_temp)
		length = j_temp->valueint;
	j_temp = cJSON_GetObjectItem(j_rsp, "data");
	if (j_temp == NULL)
		goto exit;

	MercuryTrace("get file info success, id:%s, length:%d",audioId, length);
	
	int hash_len = 0;
	unsigned char out_hash[32], in_hash[32];

	mbedtls_base64_decode(out_buff, 32*1024, olen, j_temp->valuestring, strlen(j_temp->valuestring));
	mbedtls_sha256(out_buff, *olen, out_hash, 0);
//	MercuryTrace("\ndev calc : %s\r\n",out_hash);

	j_temp = cJSON_GetObjectItem(j_rsp, "audio-hash");
	if (j_temp == NULL)
		goto exit;
	
	mbedtls_base64_decode(in_hash, 32, &hash_len, j_temp->valuestring, strlen(j_temp->valuestring));

//	MercuryTrace("from server : %s\r\n",in_hash);

	if (memcmp(out_hash, in_hash, 32) != 0)
	{
		MercuryTrace("hash not match\n");
		goto exit;
	}
	cJSON_Delete(j_rsp);
	return 0;
	
exit:
	if (j_rsp)
		cJSON_Delete(j_rsp);
	return -1;
}



