#include "SDK_API_AUDIO.h"
#include "SDK_API_DISPLAY.h"
#include "SDK_API_SYS.h"

#include "qyy_mqtt_push_server.h"
#include "speakerapp.h"

#include "device.h"
#include "audio.h"
#include "nv.h"
#include "app_config.h"
#include "lcd.h"

#include "MercuryDef.h"
#include "SDK_API_WIFI.h"
#include "SDK_API_CONFIG.h"
#include "SDK_API_FOTA.h"
#include "speakerapp.h"

#include "global.h"

#define QYY_LBS_SERVICE_HOST "https://www.qiyinyi.cn:8151"
#define QYY_REPORT_SYS_INFO_URL "/iot/stress-test/logs/add"
#define QYY_REPORT_LOCATION_URL "/iot/lbs/report"

extern HANDLE g_voice_thread_tid;
int g_msgid;
int g_pay_amount;
char g_AudioBuff[512] = {0};

DWORD volume;

// ����֧�ֲ�������LED
void _sdk_voiceplay( int tts, const char *message )
{
	//����led
    anfu_led_on();
	// // sdk_voiceplay(tts, message);
}

void amount_replay(void)		
{
	if (strlen(g_AudioBuff))
	{
		MercuryTrace("pAudioBuff = [%s]\r\n",g_AudioBuff);
		// _// sdk_voiceplay(1, g_AudioBuff);
	}
}

void _getThreadStatus(cJSON *data)
{
	cJSON *threadStatusArray = NULL, *threadStatusJson = NULL;
	unsigned int nThreadCount = MercuryThreadCount();
    osiThreadStatus_t * pThreadStatus = (osiThreadStatus_t *)LocalAlloc(LMEM_ZEROINIT, nThreadCount * sizeof(osiThreadStatus_t));
    MercuryThreadGetAllStatus(pThreadStatus, nThreadCount);

	threadStatusArray = cJSON_CreateArray();

	for (int i = 0; i < nThreadCount; i++)
	{
		osiThreadStatus_t * pTemp = pThreadStatus + i;
		threadStatusJson = cJSON_CreateObject();
		
		cJSON_AddStringToObject(threadStatusJson, "name", pTemp->name);
		cJSON_AddNumberToObject(threadStatusJson, "id", pTemp->thread_number);
		cJSON_AddNumberToObject(threadStatusJson, "stackBase", (DWORD)(pTemp->stack_base));
		cJSON_AddNumberToObject(threadStatusJson, "stackSize", pTemp->stack_alloc_size);
		cJSON_AddNumberToObject(threadStatusJson, "minRemained", pTemp->stack_min_remained);

		cJSON_AddItemToArray(threadStatusArray, threadStatusJson);
	}

	cJSON_AddItemToObject(data, "threads", threadStatusArray);
	
	LocalFree(pThreadStatus);
}

void _getHeapStack(cJSON *data)
{
	MERCURY_HEAP_STATUS pHeapStatus;

	GetAppHeapUsageStatus(&pHeapStatus);

	cJSON_AddNumberToObject(data, "heapTotal", pHeapStatus.nTotalSize);
	cJSON_AddNumberToObject(data, "heapUsed", pHeapStatus.nUsedSize);
}

// ����MQTT ѹ�����Լ�¼�豸����״̬
DWORD l_loopIndex = 0;
int _postDeviceInfo(cJSON *data)
{
	cJSON *root = NULL, *json_loopIndex = NULL;
	char *sreq = NULL;
    char SN[64] = {0};

	char buff[1024] = {0};
	const char *http_post_addr = QYY_LBS_SERVICE_HOST QYY_REPORT_SYS_INFO_URL;
	int recv_len = 0;
	
	root = cJSON_CreateObject();
	json_loopIndex = cJSON_GetObjectItem(data, "loopIndex");

	if (json_loopIndex == NULL)
		return -1;

	NV_Read(NV_SN, 32, SN);

	if (json_loopIndex->valueint >= 0)
		l_loopIndex = json_loopIndex->valueint;

	l_loopIndex++;

	cJSON_AddStringToObject(root, "deviceName", SN);
	cJSON_AddStringToObject(root, "testItem", "Transact Stress Test");
	cJSON_AddNumberToObject(root, "loopIndex", l_loopIndex);
	cJSON_AddNumberToObject(root, "audioState", (int)sdk_audio_playing());
	cJSON_AddNumberToObject(root, "queueCount", (int)sdk_audio_queue_count());
	
	_getHeapStack(root);
	_getThreadStatus(root);

	// sreq = cJSON_Print(root);
	sreq = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	// MercuryTrace("sreq = %s\r\n", sreq);
	MercuryTrace("sreq len = %d\r\n", strlen(sreq));

	//set device info
	sdk_http_post_auto(http_post_addr, sreq, "application/json", buff, 1024, &recv_len);
	MercuryTrace("post recv_buf:%s\n", buff);
	MercuryTrace("post recv_len:%d\n", recv_len);

	LocalFree(sreq);

	return 0;
}

// ��ȡ�ַ����е�����С��λ
int _get_number(char *data,int *data_num)
{
	int nums = 0;
    int flag = -1;
    int count = 0;
    *data_num = 0;
	
	for (int i = 0; i < strlen(data); i++)
	{
		if ((data[i] >= '0') && (data[i] <= '9'))
		{
			nums = nums * 10 + data[i] - '0';
            if (flag == 0)
                count++;
		}
        else if(data[i] == '.')
        {
            flag = 0;
        }
	}
	MercuryTrace("nums = %d\n", nums);

	if (count == -1)
		*data_num = 0;
	else
    	*data_num = (count > 2) ? 2 : count;

	return nums;
}

static int _chk_num_count(int num)
{
    int count = 0;
    while (num != 0)
    {
        num /= 10;
        count++;
    }
    return count;
}

void play_amount_decimals_audio(int nNumber)
{
	// int integer;
	// int decimals;
	// char integer_filesBuffer[128] = {0};
	// char decimals_filesBuffer[128] = {0};
	// char fileName[MAX_AUDIO_FILE_NAME_LEN] = {0};
	// char *play_data1 = NULL, *play_data2 = NULL;
	// int buff_len, read_len = 0, offset = 0;

	// integer = nNumber / 100;
	// decimals = nNumber % 100;

	// MercuryTrace("nNumber = [%d] integer = [%d] decimals = [%d] \r\n",nNumber,integer,decimals);

	// if (integer != 0)
	// {
	// 	play_data1 = LocalAlloc(LMEM_ZEROINIT, 15*1024);
	// 	buff_len = 15*1024;
		
	// 	num_to_audio_files_list((int)integer, integer_filesBuffer);
	//     offset = get_amr_data_from_list(integer_filesBuffer,"|", play_data1, buff_len);

	// 	// if(_chk_num_count(integer) > 1)
	// 	// {
	// 	// 	get_audio_file_name_byid(fileName, AUDIO_DOLLARS);
	// 	// }
	// 	// else
	// 	// {
	// 	// 	get_audio_file_name_byid(fileName, AUDIO_DOLLAR);

	// 	// }

	// 	get_audio_file_name_byid(fileName, AUDIO_RUPEES);

	// 	buff_len -= offset;
	// 	get_audio_file_data(fileName, play_data1+offset, buff_len, 6, &read_len);
	// 	offset += read_len;
	// 	buff_len -= read_len;
	// 	// if (decimals)
	// 	// {
	// 	// 	// get_audio_file_name_byid(fileName, AUDIO_AND);
	// 	// 	get_audio_file_data(fileName, play_data1+offset, buff_len, 6, &read_len);
	// 	// 	offset += read_len;
	// 	// }

	// 	sdk_audio_play_data(play_data1, offset, AUDIO_TYPE_AMR);
	// }

	// // if (decimals)
	// // {	
	// // 	play_data2 = LocalAlloc(LMEM_ZEROINIT, 5*1024);
	// // 	buff_len = 5*1024;
	// // 	read_len = 0;
		
	// // 	num_to_audio_files_list((int)decimals,decimals_filesBuffer);
	// //     offset = get_amr_data_from_list(decimals_filesBuffer,"|", play_data2, buff_len);
	// // 	buff_len -= offset;

	// // 	// get_audio_file_name_byid(fileName, AUDIO_CENTS);

	// // 	get_audio_file_data(fileName, play_data2+offset, buff_len, 6, &read_len);
	// // 	offset += read_len;
	// // 	sdk_audio_play_data(play_data2, offset, AUDIO_TYPE_AMR);
	// // }
}	

void play_amonut_decimals_audio_index(int nNumber)
{
	// int integer;
	// int decimals;
	// int array[20] = {0};
	// int get_num = 0;
	// int i, len, index;

	// integer = nNumber / 100;
	// decimals = nNumber % 100;

	// if (integer != 0)
	// {			
	// 	num_to_audio_index_array(integer,array, &get_num);
	// 	for(i=0; i<get_num;i++)
	// 	{
	// 		index = array[i];
	// 		sdk_audio_play_nv(index+NV_AUDIO_START_INDEX, get_nv_audio_data_len(index));
	// 	}
	   
	// 	if(_chk_num_count(integer) > 1)
	// 		sdk_audio_play_nv(AUDIO_DOLLARS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_DOLLARS));
	// 	else
	// 		sdk_audio_play_nv(AUDIO_DOLLARS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_DOLLAR));


	// 	if (decimals)
	// 	{
	// 		sdk_audio_play_nv(AUDIO_AND+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_AND));
	// 	}
	// }

	// if (decimals)
	// {	
	// 	num_to_audio_index_array(decimals,array, &get_num);
	// 	for(i=0; i<get_num;i++)
	// 	{
	// 		index = array[i];
	// 		sdk_audio_play_nv(index+NV_AUDIO_START_INDEX, get_nv_audio_data_len(index));
	// 	}
	// 	if (decimals>1)
	// 		sdk_audio_play_nv(AUDIO_CENTS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CENTS));
	// 	else
	// 		sdk_audio_play_nv(AUDIO_CENT+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CENT));
	// }
}	

void play_amount_decimals(int nNumber)
{
	if (AUDIO_TYPE_TTS == app_get_audio_type())
	{
		char str_num[100];
	
		sprintf(str_num, "%d.%d%dԪ", nNumber/100, (nNumber%100)/10, nNumber%10);
		// _// sdk_voiceplay(1, str_num);
	}
	else if(app_GetAudioType() != AUDIO_TYPE_TTS && !app_IsExistExFlash())
	{
		// play_amount_decimals_audio_index(nNumber);
	}
	else
	{
		play_amount_decimals_audio(nNumber);
	}
}
//�̼��Զ���ģ��
int _json_get_template_str(int type, int index, char *buf)
{
	int nv_index;
	char data[AUDIO_TTS_LEN_MAX] = {0};
	NvPowerCfgStruct cfg = {0};
	int len = 0;

	if (index > AUDIO_TEMPLATE_MAX)
		return -1;
	
	if (type == VOICE_TEMPLATE_PAY )
	{
		nv_index = NV_AUDIO_TEMPLATE_START + index;
		if (0 == NV_Read(NV_AUDIO_CUSTOM_CFG, sizeof(NvPowerCfgStruct), (BYTE*)&cfg)) //�̼��Զ��岥��
		{
			if(cfg.mode == 0) //�������̼��Զ�������
			{
				if (0 != NV_Read(nv_index, AUDIO_TTS_LEN_MAX, (BYTE*)buf))
				{
					MercuryTrace("read audio info fail\n");
					return -1;
				}
				return 0;
			}
			
			NV_Read(NV_AUDIO_CUSTOM_DATA, sizeof(data), (BYTE*)&data); 
			
			len = strlen(data);
			if (cfg.mode == 1) ////�Ȳ����̼��Զ���������>��������ģ��
			{
				if (len)
					strcpy(buf, data);

				// important: NV_Read has problem
				if (0 != NV_Read(nv_index, sizeof(data)-len, (BYTE*)buf+len))
				{
					MercuryTrace("read audio info fail\n");
					return -1;
				}
			}
			else if (cfg.mode == 2) //�󲥷ţ���������ģ�塪>�̼��Զ�����
			{
				memset(buf,0,AUDIO_TTS_LEN_MAX);
				if (0 != NV_Read(nv_index, AUDIO_TTS_LEN_MAX, (BYTE*)buf))
				{
					MercuryTrace("read audio info fail\n");
					return -1;
				}
				if (len)
					strcat(buf, data);
			}
			else if (cfg.mode == 3) //���̼��Զ����������潻������ģ��
			{
				strcpy(buf, data);
			}
		}
		else
		{
			if (0 != NV_Read(nv_index, AUDIO_TTS_LEN_MAX, (BYTE*)buf))
			{
				MercuryTrace("read audio info fail\n");
				return -1;
			}
		}
		return 0;
	}

	if (type == VOICE_TEMPLATE_DISCOUNT)
	{
		nv_index = NV_DISCOUNT_TEMPLATE_START + index;
	}
	else if (type == VOICE_TEMPLATE_VERIFYCODE) 
	{
		nv_index = NV_VERIFYCODE_TEMPLATE_START + index;
	}
	else
	{
		MercuryTrace("invalid template type\n");
		return -1;
	}
				
 	MercuryTrace("read audio template nv_index:%d\n", nv_index);

	if (0 != NV_Read(nv_index, AUDIO_TTS_LEN_MAX, (BYTE*)buf))
	{
		MercuryTrace("read audio info fail\n");
		return -1;
	}
	MercuryTrace("audio info:%s\n", buf);
	return 0;
}

// #define TRANS_RECORD_NUM 10
// #define TRANS_LEN_MAX 100
// typedef struct
// {
// 	int total;
// 	int windex; // д��¼
// 	int rindex; // ����¼
// 	char data[TRANS_RECORD_NUM][TRANS_LEN_MAX];
// }transRecordStruct;

// transRecordStruct g_transRecord = {0};
TRANS_RECORD g_transRecord = {0};

int trans_record_write(char *data)
{
	int index;

	if (strlen(data) > TRANS_LEN_MAX)
	{
		MercuryTrace("trans_record_write data len exceed\n");
		return -1;
	}

	index = g_transRecord.windex;

	memset(g_transRecord.data[index], 0, TRANS_LEN_MAX);
	strcpy(g_transRecord.data[index], data);

	
	MercuryTrace("write data:%s\n",g_transRecord.data[index]);

	if (g_transRecord.total < TRANS_RECORD_NUM)
		g_transRecord.total++;

	if (g_transRecord.windex == TRANS_RECORD_NUM-1)
		g_transRecord.windex = 0;
	else
		g_transRecord.windex++;
	return 0;
}

int trans_record_play(int type, int bPlayOrShow)
{
	if (0 == g_transRecord.total) //û�м�¼
		return ERROR;

	MercuryTrace("total:%d, windex:%d\n", g_transRecord.total,g_transRecord.windex);

	if (type == 0) // ����ǰ��¼
	{
		if (g_transRecord.windex > 0)
			g_transRecord.rindex = g_transRecord.windex-1;
		else
			g_transRecord.rindex == TRANS_RECORD_NUM-1;

	}
	else if(type == 1) // ��ǰһ����¼
	{
		if (g_transRecord.rindex>0)
			g_transRecord.rindex --;
		else
			g_transRecord.rindex = g_transRecord.total - 1;
	}
	else if(type == 2) // ����һ����¼
	{
		if (g_transRecord.rindex == TRANS_RECORD_NUM-1 || g_transRecord.rindex == g_transRecord.total-1 || g_transRecord.rindex < 0)
			g_transRecord.rindex = 0;
		else
			g_transRecord.rindex++;
	}
	
	MercuryTrace("rindex:%d, record data:%s\n",g_transRecord.rindex, g_transRecord.data[g_transRecord.rindex]);

	// _sdk_voiceplay(1, g_transRecord.data[g_transRecord.rindex]);
	if(ERROR == parseTxnDetails_new(g_transRecord.data[g_transRecord.rindex]))
		return ERROR;
	
	start_bcs_clear_timer();
	if(!bPlayOrShow)	// Play
	{
		play_amount_mosambee((unsigned char )1);
	}	
	else				// Show
	{
		int INTEGER = 0;
		int DECIMAL = 0;
		sscanf(mqtt_txn_details.amt, "%d.%d", &INTEGER, &DECIMAL);

		MercuryTrace("INTEGER:%d\tDECIMAL:%d\r\n", INTEGER, DECIMAL);
		// if(INTEGER > 0 && INTEGER <= 999999)
		// 	sdk_bcs_number(INTEGER, 0);
		// else
		// 	sdk_bcs_number(0, 0);

        if(INTEGER < 10000)
        {
            // sdk_bcs_number((int)(total_amt*100.00), 2);
            sdk_bcs_number(INTEGER*100+DECIMAL, 2);
            // start_bcs_clear_timer();
        }
        else if(INTEGER > 9999 && INTEGER < 100000)
        {
            sdk_bcs_number((INTEGER*10)+(DECIMAL/10), 1);
        }
		else if(INTEGER > 99999 && INTEGER < 1000000)
        {
            sdk_bcs_number(INTEGER, 0);
        }
	}
	
	return SUCCESS;
}

void app_save_trans_record(void)
{
	NV_Delete(NV_TRANS_RECORD);
	// NV_Write(NV_TRANS_RECORD, sizeof(transRecordStruct), &g_transRecord);
	NV_Write(NV_TRANS_RECORD, sizeof(TRANS_RECORD), &g_transRecord);
}

void app_load_trans_record(void)
{
	// NV_Read(NV_TRANS_RECORD,sizeof(transRecordStruct), &g_transRecord);
	NV_Read(NV_TRANS_RECORD,sizeof(TRANS_RECORD), &g_transRecord);
}

//����������������
int _json_voice_trans1(cJSON *data)
{
	int amount = 0;
	char buf1[AUDIO_TTS_LEN_MAX] = {0};
	cJSON *json_template = NULL, *json_pay = NULL;


	json_template= cJSON_GetObjectItem(data, "voice-template");
	if (json_template == NULL)
	{
		MercuryTrace("no voice template\n");
		return -1;
	}

	json_pay= cJSON_GetObjectItem(data, "pay-amount");
	if (json_pay == NULL)
	{
		MercuryTrace("no pay amount\n");
		return -1;
	}

	_json_get_template_str(VOICE_TEMPLATE_PAY, json_template->valueint, buf1);
	amount = json_pay->valueint;

	memset(g_AudioBuff, 0, sizeof(g_AudioBuff));
	if (amount > 999999)
	{
		if (amount % 100 == 0)
		{
			snprintf(g_AudioBuff, 256, "%s%dԪ", buf1, amount / 100);
		}
		else
		{
			snprintf(g_AudioBuff, 256, "%s%d.%d%dԪ", buf1, amount / 100, (amount % 100) / 10, amount % 10);
		}
		sdk_bcs_number(000000, 2);
	}
	else
	{
		if (amount % 100 == 0)
		{
			snprintf(g_AudioBuff, 256, "%s%dԪ", buf1, amount / 100);
			sdk_bcs_number(amount, 2);
		}
		else
		{
			snprintf(g_AudioBuff, 256, "%s%d.%d%dԪ", buf1, amount / 100, (amount % 100) / 10, amount % 10);
			sdk_bcs_number(amount, 2);
		}
	}

	MercuryTrace("// sdk_voiceplay is [%s]\n",g_AudioBuff);
	// _// sdk_voiceplay(1, g_AudioBuff);
	trans_record_write(g_AudioBuff);

	sdk_bcs_set_speaker(TRUE);
	return 0;
}

#define _TEMP_USING_BCS		1
int _json_voice_trans2(cJSON *data)
{
	cJSON * pMsgType = cJSON_GetObjectItem(data, "type");
	cJSON * pMsgText = cJSON_GetObjectItem(data, "text");
	cJSON * pMsgData = cJSON_GetObjectItem(data, "data");
	
	if ( (pMsgType == NULL) || (pMsgText == NULL) || (pMsgData == NULL) )
	{
		return -1;
	}
#if (_TEMP_USING_BCS == 1)
	// ��ȡ�����Ϣ������ʾ���������������
	cJSON *pMsgAmout = cJSON_GetObjectItem(pMsgData, "pay_price");
	MercuryTrace("pMsgAmout = %d\r\n",atoi(pMsgAmout->valuestring));
	if (atoi(pMsgAmout->valuestring) > 999999){
		qyy_bcs_number(000000, 2);	
	}else{
	qyy_bcs_number(atoi(pMsgAmout->valuestring), 2);
	}
	// ���ö�����������Ϊ����״̬
	qyy_bcs_set_speaker(TRUE);

#endif

	//Push messages on Aliyun Iot platform
	// ��ȡ������Ϣ��������
	memset(g_AudioBuff, 0, sizeof(g_AudioBuff));

	sdk_Utf8ToGb2312(pMsgText->valuestring, strlen(pMsgText->valuestring), (char *)g_AudioBuff);
	// _// sdk_voiceplay(1, g_AudioBuff);
	trans_record_write(g_AudioBuff);
	MercuryTrace("g_AudioBuff = [%s]\r\n",g_AudioBuff);
	return 0;
}

char s_requestId[64]= {0};

//����������������
int _json_voice_trans3(cJSON *data)
{
	int amount;
	double _amount;
	char buf1[AUDIO_TTS_LEN_MAX] = {0};
	cJSON *json_temp = NULL, *json_pay = NULL, *json_paytype = NULL;

	json_temp = cJSON_GetObjectItem(data, "requestId");
	if (json_temp == NULL)
	{
		MercuryTrace("no requestId\n");
		return -1;
	}

	if (0 == strcmp(s_requestId, json_temp->valuestring )) //�ظ�����Ϣ
	{
		MercuryTrace("repeat message\n");
		return -1;
	}
	json_pay= cJSON_GetObjectItem(data, "requestData");
/*	if (json_pay == NULL)
	{
		MercuryTrace("no pay amount\n");
		return -1;
	}
*/

	json_paytype = cJSON_GetObjectItem(data, "payType");
	_json_get_template_str(VOICE_TEMPLATE_PAY, atoi(json_paytype->valuestring), buf1);

	if (json_pay && (atoi(json_paytype->valuestring) != 10)) //10��ȡ�����ף����������
	{
		snprintf(g_AudioBuff, 256, "%s%sԪ", buf1, json_pay->valuestring);
		MercuryTrace("// sdk_voiceplay is [%s]\n",g_AudioBuff);
		// _// sdk_voiceplay(1, g_AudioBuff);
		trans_record_write(g_AudioBuff);
		if (atoi(json_pay->valuestring) < 9999.99)
		{
			if (atof(json_pay->valuestring) - atoi(json_pay->valuestring) > 0)
			{
				MercuryTrace("atof(json_pay->valuestring) = %f\n",atof(json_pay->valuestring));
				_amount = atof(json_pay->valuestring);
				//���ĳЩ���֣���17.8��ת����17.79���쳣��BUG
				amount = (int)((_amount + 0.00001) * 100);
				MercuryTrace("amount = %d\n",amount);
				sdk_bcs_number(amount, 2);
			}
			else
			{
				sdk_bcs_number(atoi(json_pay->valuestring), 0);
			}
		}
		else
		{
			sdk_bcs_number(000000, 0);
		}

		sdk_bcs_set_speaker(TRUE);
	}
	else
	{
		// if (strlen(buf1))
		// 	_// sdk_voiceplay(1, buf1);
	}
	return 0;
}

//�����汾����
extern HANDLE g_event_ota_update;
int _json_ota_proc(cJSON *data)
{
	cJSON *json_fwbuild = NULL;

	json_fwbuild= cJSON_GetObjectItem(data, "fw-build");
	if (json_fwbuild == NULL)
	{
		MercuryTrace("no fwbuild json\n");
		return -1;
	}
	MercuryTrace("\njson_fwbuild->valueint is %d\n",json_fwbuild->valueint);

	if (json_fwbuild->valueint <= FW_BUILD_VERSION) //�������汾�����豸�汾
	{
		// _// sdk_voiceplay(1, "�����汾����Ҫ����");
		return 0;
	}
		
	// _// sdk_voiceplay(1, "�����汾����");
	// // app_audio_play(AUDIO_OTA_UPDATE);
	SetEvent(g_event_ota_update);
	
	return 0;
}

char char2int(char c)
{
	if (c >= '0' && c <= '9')
		return c-'0';
	else if(c >= 'a' && c <= 'z')
		return c -'a' + 10;
	else if(c >= 'A' && c <= 'Z')
		return c - 'A' + 10;

	return 0;
}

int str2hex(char *str, char *buf)
{
	int i, j = 0;
	char c1, c2;
	int len = strlen(str);
	
	for (i=0, j=0; i<len; )
	{
		c1 = char2int(str[i++]);
		c2 = char2int(str[i++]);
		
    	buf[j++] = c1*16 + c2; 
	}
	return j;
}
//�Զ��岥��
int _json_customize_proc(cJSON *data)
{
	cJSON *json_voice = NULL;
	char buf[MSG_DATA_LEN] = {0};
	int len;
	int num,porint;
	
	json_voice= cJSON_GetObjectItem(data, "custom-voice");
	if (json_voice == NULL)
	{
		MercuryTrace("no custom voice \n");
		return -1;
	}

	if (strlen(json_voice->valuestring) > MSG_DATA_LEN)
	{
		MercuryTrace("customize message too long \n");
		return -1;
	}

	MercuryTrace("custom message: %s\n", json_voice->valuestring);

	memset(buf, 0, sizeof(buf));
	len = str2hex(json_voice->valuestring, buf);

	if (len == 0)
		return -1;

	// _// sdk_voiceplay(1, buf);
	strcpy(g_AudioBuff,buf);

	num = _get_number(&buf,&porint);
	// num = get_number(&buf,&porint);
	qyy_bcs_number(num,porint);
	qyy_bcs_set_speaker(TRUE);

	return 0;
}
//�ۿ��Ż���Ϣ
int _json_discount(cJSON *data)
{
	int amount = 0;
	char buf1[128], buf2[256];

	cJSON *json_type = NULL, *json_amount = NULL;
		
	json_type = cJSON_GetObjectItem(data, "discount-type");
	if (json_type == NULL)
	{
		MercuryTrace("no discount type\n");
		return -1;
	}

	json_amount = cJSON_GetObjectItem(data, "discount-amount");
	if (json_amount == NULL)
	{
		MercuryTrace("no discount amont\n");
		return -1;
	}

	memset(buf1, 0, sizeof(buf1));
	_json_get_template_str(VOICE_TEMPLATE_DISCOUNT, json_type->valueint, buf1);
		
	amount = json_amount->valueint;
	
	snprintf(buf2, 256, "%s�Ż�%d.%d%dԪ", buf1, amount/100, (amount%100)/10, amount%10);
	// _// sdk_voiceplay(1, buf2);
	return 0;
}
//��̬��֤��
int _json_auth_code(cJSON *data)
{
	char buf[64]={0};
	int i,j;
	char buf3[128];
	char buf4[128];

	cJSON *json_code = NULL;
	cJSON *json_template = NULL;
		
	json_template = cJSON_GetObjectItem(data, "voice-template");
	if (json_template == NULL)
	{
		MercuryTrace("no voice-template \n");
		return -1;
	}
	
	json_code = cJSON_GetObjectItem(data, "auth-code");
	if (json_code == NULL)
	{
		MercuryTrace("no auth code \n");
		return -1;
	}
	if (json_template->valueint < 5 )
	{
		strcpy(buf, "��֤��");
		for(i = 0, j=6; i<strlen(json_code->valuestring); i++)
		{
			buf[j++] = json_code->valuestring[i];
			buf[j++] = ' ';
		}
		// _// sdk_voiceplay(1, buf);
	}
	else if (json_template->valueint > 17)
	{
		memset(buf3, 0, sizeof(buf3));
		_json_get_template_str(VOICE_TEMPLATE_VERIFYCODE, json_template->valueint, buf3);
		// _// sdk_voiceplay(1,buf3);
	}
	else{
		memset(buf3, 0, sizeof(buf3));
		_json_get_template_str(VOICE_TEMPLATE_VERIFYCODE,json_template->valueint,buf3);

		for(i = 0, j=0; i<strlen(json_code->valuestring); i++)
		{
			buf[j++] = json_code->valuestring[i];
			buf[j++] = ' ';
		}
		MercuryTrace("code is %s\n",json_code->valuestring);

		snprintf(buf4, 128, "%s,%s",buf3,buf);
		// _// sdk_voiceplay(1, buf4);

	}

	return 0;
}

//�����豸
int _json_dev_control(cJSON *data)
{
	cJSON *json_dev = NULL;
		
	json_dev = cJSON_GetObjectItem(data, "dev-control");
	if (json_dev == NULL)
	{
		MercuryTrace("no device control \n");
		return -1;
	}

	if (json_dev->valueint == 1) //�豸����
	{
		set_restart_reason( E_RST_REBOOT );
		// TerminateThread(g_voice_thread_tid, NULL);
		Sleep(3000);
		PowerReboot();
	}
	else if (json_dev->valueint == 2) //�豸�ػ�
	{
		powerkey_longpress_callback(NULL);
	}
	else
	{
		MercuryTrace("device control value error\n ");
		return -1;
	}
	return 0;
}

void _config_qr_code_params(cJSON * config) {
	cJSON *json_qr_url = NULL;
	cJSON *json_qr_gap = NULL;
	NvQrcodeCfgStruct qrcodeinfo = {0};
	NV_Read(NV_QRCODE_CFG, sizeof(NvQrcodeCfgStruct), (BYTE *)&qrcodeinfo);

	json_qr_url = cJSON_GetObjectItem(config,"qr-url");
	if (json_qr_url != NULL){
		MercuryTrace("\n json_qr_url->valuestring is [%s]\n",json_qr_url->valuestring);
		// MercuryTrace("\n qrcodeinfo.url is [%s]\n",qrcodeinfo.url);
		strcpy(qrcodeinfo.url, json_qr_url->valuestring);
	}

	json_qr_gap = cJSON_GetObjectItem(config,"qr-time-gap");
	if (json_qr_gap != NULL){
		MercuryTrace("\n qr-time-gap is [%d]\n",json_qr_gap->valueint);
		qrcodeinfo.gap = json_qr_gap->valueint;
	}

	if (json_qr_url != NULL || json_qr_gap != NULL) {
		NV_Delete(NV_QRCODE_CFG);
		NV_Write(NV_QRCODE_CFG, sizeof(qrcodeinfo), &qrcodeinfo);
		app_qrcode_cfg_init();
	}
}

//��̬����
extern DWORD volume;
extern BOOL g_volume_flag;//�����������С�������ٲ�������
extern app_set_tts_param();
extern qr_code_flag;
int _json_cfg_dyn(cJSON *data)
{
	cJSON *json_vol = NULL;
	cJSON *json_speed = NULL;
	cJSON *json_dialectid = NULL;
			
	json_vol = cJSON_GetObjectItem(data, "voice-volume");
	if (json_vol != NULL)
	{
		volume = json_vol->valueint/10 +1;
		if (volume <=9 && volume >=3)
		{
			app_SetVolume(volume);
			AudioSetVolume(volume);
		}
		// _// sdk_voiceplay(1, "�������");//��������
		// g_volume_flag = TRUE;
	}

	json_speed = cJSON_GetObjectItem(data,"tts-speed");
	if (json_speed != NULL){
		int speakSpeed;
		speakSpeed = atoi(json_speed->valuestring);
		app_set_tts_param(speakSpeed);//����
		NV_Write(NV_PLAY_SPEED, sizeof(speakSpeed), &speakSpeed);
		
	}

	json_dialectid = cJSON_GetObjectItem(data,"dialect-id");
	if (json_dialectid != NULL){
		MercuryTrace("dialect-id is %d \n",json_dialectid->valueint);
	}

	// if (0 == get_dev_config_info())
	// {
	// 	MercuryTrace("\n..$$.........get_dev_config_info.............\n");
	// }

	// ���ö�̬��λ�����
	_config_qr_code_params(data);

	return 0;
}


char *g_serv_ip[16];
int  g_serv_port;
int _json_cfg_server(cJSON *data)
{
	cJSON *json_ip = NULL, *json_port=NULL;
	AppInfoStruct app_info = {0};
	
	if ( 0 != NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE*)&app_info))
	{
		MercuryTrace("_json_cfg_server, fs_read_deviceinfo fail\n");
		return -1;
	}
				
	json_ip = cJSON_GetObjectItem(data, "config-server-ip");
	if (json_ip == NULL)
	{
		MercuryTrace("no config server ip \n");
		return -1;
	}

	json_port = cJSON_GetObjectItem(data, "config-server-port");
	if (json_port == NULL)
	{
		MercuryTrace("no config server port \n");
		return -1;
	}
		
	strcpy(app_info.cfg_serv_host , json_ip->valuestring);		//�������÷�����,δ��Ч
	app_info.cfg_serv_port = json_port->valueint;
	app_info.cfg_done = 0;

	NV_Write(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE*)&app_info);
	// _// sdk_voiceplay(1, "���÷������ı�,�������豸");
	
	return 0;
}

void _lcd_set_matrix(int nAmount,uint16 x,uint16 y,uint16 width, uint16 height,uint16 fgcolor, uint16 bgcolor)
{
	switch (nAmount)
	{
		case 0:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_zero,width*height/8,fgcolor,bgcolor);
			break;
		case 1:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_one,width*height/8,fgcolor,bgcolor);
			break;
		case 2:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_two,width*height/8,fgcolor,bgcolor);
			break;
		case 3:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_three,width*height/8,fgcolor,bgcolor);
			break;
		case 4:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_four,width*height/8,fgcolor,bgcolor);
			break;
		case 5:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_five,width*height/8,fgcolor,bgcolor);
			break;
		case 6:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_six,width*height/8,fgcolor,bgcolor);
			break;
		case 7:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_seven,width*height/8,fgcolor,bgcolor);
			break;
		case 8:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_eight,width*height/8,fgcolor,bgcolor);
			break;
		case 9:
			lcddrv_set_matrix_mercury(x,y,width,height,_16_24_nine,width*height/8,fgcolor,bgcolor);
			break;
		default:
			break;
	}
}



int _get_divisor(int ncount)
{
	int divisor = 1;

	for (int i = 0; i < ncount; i++)
	{
		divisor *= 10;
	}

	// MercuryTrace("_get_divisor ncount = [%d] divisor = [%d]\r\n",ncount,divisor);
	return divisor;
}

int _display_bitmap(int nAmount,uint16 x,uint16 y,uint16 width, uint16 height,uint16 fgcolor, uint16 bgcolor)
{
	int amount,count,divisor;
	int _x,_y;

	_x = x;
	_y = y;
	// MercuryTrace("_x = [%d] _y = [%d]\r\n",_x,_y);

	count = _chk_num_count(nAmount);
    while (count)
    {
		count--;
		_x += 12;

		divisor = _get_divisor(count);
		amount = nAmount / divisor % 10;
		_lcd_set_matrix(amount,_x,_y,width,height,fgcolor,bgcolor);
		MercuryTrace("amount = [%d] count = [%d]\r\n",amount,count);
    }
	
	return _x;
}

int _json_qr_code(cJSON *data)
{
	cJSON *json_payee = NULL,*json_payeeName = NULL,
			*json_transaction = NULL,*json_amount = NULL,
			*json_qr_code = NULL;

	// RECTL qrRect = {28, 40, 190, 190};
	RECTL qrRect = {64, 24, 216, 256};
	char str_qr[128] = {0};
	int olen;

	int numAmount = 0,integer = 0,decimals = 0,xRet = 0;

	json_payee = cJSON_GetObjectItem(data,"payee");
	if (json_payee != NULL)
	{
		json_payeeName = cJSON_GetObjectItem(json_payee,"payeeName");
		if (json_payeeName == NULL)
			MercuryTrace("no payee name \n");
	}

	json_transaction = cJSON_GetObjectItem(data,"transaction");
	if (json_transaction != NULL)
	{
		json_amount = cJSON_GetObjectItem(json_transaction,"amount");
		if (json_amount == NULL)
			MercuryTrace("no amount \n");
	}

	json_qr_code = cJSON_GetObjectItem(data,"qr-code");
	if (json_qr_code == NULL)
	{
		MercuryTrace("no qr code \n");
		return -1;
	}

	// mbedtls_base64_decode(str_qr, 128, &olen, json_qr_code->valuestring, strlen(json_qr_code->valuestring));
	// ���ַ�����ʾΪ��ά��
	// sdk_disp_qrcode(&qrRect, str_qr, olen);
	sdk_disp_qrcode(&qrRect, json_qr_code->valuestring, strlen(json_qr_code->valuestring));

	lcddrv_end_paint();
	return 0;
}

//����������Ϣ
static struct msg_json_tbl g_msg_tbl[] =
{
	{QYY_MSG_TYPE_VOICE_TRANS1, _json_voice_trans1},
	{QYY_MSG_TYPE_VOICE_TRANS2, _json_voice_trans2},
	{QYY_MSG_TYPE_VOICE_TRANS3, _json_voice_trans3},
	{QYY_MSG_TYPE_OTA, _json_ota_proc},
	{QYY_MSG_TYPE_VOICE_CUSTOMIZE, _json_customize_proc},
	{QYY_MSG_TYPE_DISCOUNT, _json_discount},
	{QYY_MSG_TYPE_VERF_CODE, _json_auth_code},
	{QYY_MSG_TYPE_SET_POWER, _json_dev_control},
	{QYY_MSG_TYPE_CFG_DYN, _json_cfg_dyn},
	{QYY_MSG_TYPE_CFG_SERV, _json_cfg_server},
	{QYY_MSG_TYPE_QR_CODE, _json_qr_code},
	{QYY_TEST_GET_SYS_INFO, _postDeviceInfo},
	
	// {QYY_MSG_TYPE_AUDIO_UPDATE, _json_audio_update},
	{QYY_MSG_TYPE_MAX, NULL},
};

int qyy_mqtt_parse_payload(const unsigned char *abuff, uint32_t abufflen)
{
	int  i, ret =-1;
	cJSON *root = NULL, *cjson_header = NULL, *cjson_data = NULL;
	cJSON *id = NULL, *version = NULL;
	cJSON *json_category = NULL;

	root = cJSON_Parse((char*)abuff);

	if (root == NULL)
	{
		MercuryTrace("push message json error\n ");
		return -1;
	}

	cjson_header = cJSON_GetObjectItem(root, "header");
	if (cjson_header == NULL)
	{
		MercuryTrace("push message json no header\n ");
		//�ڿͻ��Զ���ģʽʱ��û��header
		//Ŀǰû��ʹ�ò�ͬ����ѡ����߿ͻ��ŵ���Ϣ���֣���ʱ�ֱ���2�ָ�ʽ
		//��������и��ඨ�Ƶĸ�ʽ����Ҫ�ò�ͬ�ͻ���������
		if (0 != _json_voice_trans2(root))
		{
			_json_voice_trans3(root);
		}
		cJSON_Delete(root);
		return 0;
		//goto exit;
	}

	cjson_data = cJSON_GetObjectItem(root, "data");
	if (cjson_data == NULL)
	{
		MercuryTrace("push message json no data\n ");
		goto exit;
	}
	
	id = cJSON_GetObjectItem(cjson_header, "id");
	if (id == NULL)
	{
		MercuryTrace("push message json no id\n ");
		goto exit;
	}

	version = cJSON_GetObjectItem(cjson_header, "version");
	if (version == NULL)
	{
		MercuryTrace("push message json no version\n ");
		goto exit;
	}
	if (version->valueint != 1)
	{
		MercuryTrace("push message json version error \n ");
		goto exit;
	}

	g_msgid = id->valueint;
	
	json_category = cJSON_GetObjectItem(cjson_header, "category");
	if (json_category == NULL)
	{
		MercuryTrace("push message json no category\n ");
		goto exit;
	}

	for (i=0; ;i++)
	{
		if (json_category->valueint == g_msg_tbl[i].type)
		{
			ret = g_msg_tbl[i].func(cjson_data);
			break;
		}
		if (g_msg_tbl[i].type == QYY_MSG_TYPE_MAX)
		{
			MercuryTrace("not find correct msg type\n");
			goto exit;
		}
	}
exit:
	if (root)
		cJSON_Delete(root);
	return ret;
}

extern HANDLE g_voice_thread_tid;
int g_msgid;

int _app_config_mqtt_parse(cJSON *json_mqtt)
{
	cJSON *json_ip = NULL, *json_port = NULL, *json_username = NULL, *json_pwd = NULL, *json_clientid = NULL;
	cJSON *json_subtopic = NULL, *json_pubtopic = NULL, *json_keepalive = NULL;
	AppMqttCfgStruct mqtt_cfg;
	char *p = NULL;

	json_ip = cJSON_GetObjectItem(json_mqtt, "broker-ip");
	json_port = cJSON_GetObjectItem(json_mqtt, "broker-port");
	json_username = cJSON_GetObjectItem(json_mqtt, "user-name");
	json_pwd = cJSON_GetObjectItem(json_mqtt, "password");
	json_subtopic = cJSON_GetObjectItem(json_mqtt, "subscribe-topic");
	json_pubtopic = cJSON_GetObjectItem(json_mqtt, "publish-topic");
	json_keepalive = cJSON_GetObjectItem(json_mqtt, "keep-alive");
	json_clientid = cJSON_GetObjectItem(json_mqtt, "client-id");

	if (json_clientid == NULL || json_ip == NULL || json_port == NULL || 
		json_username == NULL || json_pwd == NULL || json_subtopic == NULL)
	{
		MercuryTrace("mqtt config item not valid \n");
		return ERR_DATA_PARSE_FAIL;
	}
		

	//need add string length 
	strcpy(mqtt_cfg.clientid, json_clientid->valuestring);
	strcpy(mqtt_cfg.ip, json_ip->valuestring);
	strcpy(mqtt_cfg.username, json_username->valuestring);
	strcpy(mqtt_cfg.password, json_pwd->valuestring);
	strcpy(mqtt_cfg.subtopic, json_subtopic->valuestring);


	//�������豸��username�а��� username&productkey
	p = strchr(json_username->valuestring, '&'); 
	if (p != NULL)	
		strcpy(mqtt_cfg.productkey, ++p);
	
	
	mqtt_cfg.port = json_port->valueint;

	if(json_pubtopic && json_pubtopic->valuestring)
		strcpy(mqtt_cfg.pubtopic, json_pubtopic->valuestring);
	if (json_keepalive != NULL)
		mqtt_cfg.keep_alive =json_keepalive->valueint;
	else
		mqtt_cfg.keep_alive = DEFAULT_KEEP_ALIVE;

	if (0 != NV_Delete(NV_MQTT_CFG))
	{
		MercuryTrace("delete mqtt config nv fail\n");
		//return ERR_UPDATE_FAIL;
	}
	if ( 0 != NV_Write(NV_MQTT_CFG, sizeof(AppMqttCfgStruct), (BYTE*)&mqtt_cfg))
	{
		MercuryTrace("write mqtt config fail\n");
		return ERR_UPDATE_FAIL;
	}

	return 0;
}


static int _app_config_audio_parse(cJSON *json_audio)
{
	cJSON *json_type = NULL, *json_vol = NULL, *json_fmt = NULL, *json_dialect = NULL;
	AppAudioCfgStruct audio_cfg;

	json_type = cJSON_GetObjectItem(json_audio, "voice-type");
	json_vol = cJSON_GetObjectItem(json_audio, "voice-volume");
	json_fmt = cJSON_GetObjectItem(json_audio, "voice-format");
	json_dialect = cJSON_GetObjectItem(json_audio, "dialect-id");

	if (json_type != NULL)
	{
		audio_cfg.type = json_type->valueint;
	}
	if (json_vol != NULL)
	{
		audio_cfg.volume = json_vol->valueint;
		volume = audio_cfg.volume / 10 + 1;
		AudioSetVolume(volume);
	}
	if (json_fmt != NULL)
	{
		audio_cfg.format = json_fmt->valueint;
	}
	if (json_dialect != NULL)
	{
		audio_cfg.dialect_id = json_dialect->valueint;
	}
	NV_Delete(NV_AUDIO_CFG);
	if (0 != NV_Write(NV_AUDIO_CFG, sizeof(AppMqttCfgStruct), (BYTE *)&audio_cfg))
	{
		MercuryTrace("write aduio config fail\n");
		return ERR_UPDATE_FAIL;
	}

	return 0;
}

static int _app_config_audio_template_parse(int type, cJSON *json_template)
{
	cJSON *json_item = NULL, *json_id = NULL, *json_fmt = NULL, *json_data = NULL;
	int itemNum, i, len;
	size_t outlen;
	int nv_index, id;
	int ret;
	unsigned char data[AUDIO_TTS_LEN_MAX];

	itemNum = cJSON_GetArraySize(json_template);

	MercuryTrace("template array num:%d\n", itemNum);

	for (i = 0; i < itemNum; i++)
	{
		json_item = cJSON_GetArrayItem(json_template, i);
		json_id = cJSON_GetObjectItem(json_item, "template-id");
		json_fmt = cJSON_GetObjectItem(json_item, "voice-format");
		json_data = cJSON_GetObjectItem(json_item, "data");

		if (json_data == NULL || json_id == NULL || json_fmt == NULL)
		{
			MercuryTrace("invalid audio template \n");
			return ERR_DATA_PARSE_FAIL;
		}

		id = json_id->valueint;
		if (id >= 200)
			id -= 200;
		else if (id >= 100)
			id -= 100;

		if (id > AUDIO_TEMPLATE_MAX)
		{
			MercuryTrace("template id exceed max:%d\n", AUDIO_TEMPLATE_MAX);
			return ERR_DATA_PARSE_FAIL;
		}

		if (type == VOICE_TEMPLATE_PAY)
			nv_index = NV_AUDIO_TEMPLATE_START + id;
		else if (type == VOICE_TEMPLATE_DISCOUNT)
			nv_index = NV_DISCOUNT_TEMPLATE_START + id;
		else if (type == VOICE_TEMPLATE_VERIFYCODE)
			nv_index = NV_VERIFYCODE_TEMPLATE_START + id;

		len = strlen(json_data->valuestring);
		memset(data, 0, AUDIO_TTS_LEN_MAX);
		if (0 != mbedtls_base64_decode(data, len, &outlen, (unsigned char *)json_data->valuestring, len))

		{
			MercuryTrace("base64 decode fail: %s\n", json_data->valuestring);
			return ERR_DATA_PARSE_FAIL;
		}

		NV_Delete(nv_index);

		MercuryTrace("write audio template nv_index:%d\n", nv_index);
		ret = NV_Write(nv_index, sizeof(data), (BYTE *)data);

		if (ret != 0)
		{
			MercuryTrace("write template data fail \n");
			return ERR_UPDATE_FAIL;
		}
	}
	return 0;
}

static int _app_config_power_parse(int type, cJSON *json_power)
{
	cJSON *json_mode = NULL, *json_fmt = NULL, *json_data = NULL;
	int len;
	size_t outlen;
	NvPowerCfgStruct cfg = {0};
	int nv_index, nv_data_index;
	//unsigned char data[AUDIO_TTS_LEN_MAX] = {0}; //For now, only TTS mode is supported. If other formats are supported, malloc can be used
	char *data = NULL;
	
	json_mode = cJSON_GetObjectItem(json_power, "play-mode");
	json_fmt = cJSON_GetObjectItem(json_power, "voice-format");
	json_data = cJSON_GetObjectItem(json_power, "data");

	if (json_data == NULL || json_fmt == NULL || json_mode == NULL)
	{
		MercuryTrace("invalid audio template \n");
		return ERR_DATA_PARSE_FAIL;
	}
	if (type == VOICE_POWERON)
	{
		nv_index = NV_POWERON_CFG;
		nv_data_index = NV_POWERON_DATA;

		//char *spoweron = cJSON_Print(json_power);
		//MercuryTrace("spoweron:%s\n", spoweron);
		//LocalFree(spoweron);
	}
	else if (type == VOICE_POWEROFF)
	{
		nv_index = NV_POWEROFF_CFG;
		nv_data_index = NV_POWEROFF_DATA;
		
		//char *spoweroff = cJSON_Print(json_power);
		//MercuryTrace("spoweroff:%s\n", spoweroff);
		//LocalFree(spoweroff);
	}
	else if (type == VOICE_CUSTOM)
	{
		nv_index = NV_AUDIO_CUSTOM_CFG;
		nv_data_index = NV_AUDIO_CUSTOM_DATA;
	}

	len = strlen(json_data->valuestring);

	data = LocalAlloc(LMEM_ZEROINIT, 10240);
	
	if (0 != mbedtls_base64_decode(data, 10240, &outlen, (unsigned char *)json_data->valuestring, len))
	{
		MercuryTrace("base64 decode fail\n");
		LocalFree(data);
		return ERR_DATA_PARSE_FAIL;
	}

	NV_Delete(nv_index);
	NV_Delete(nv_data_index);

	//cfg.len = outlen;
	cfg.format = json_fmt->valueint;
	cfg.mode = json_mode->valueint;

	if (0 != NV_Write(nv_index, sizeof(NvPowerCfgStruct), (BYTE *)&cfg))
	{
		MercuryTrace("write nv audio template fail\n");
		LocalFree(data);
		return ERR_UPDATE_FAIL;
	}
	if (0 != NV_Write(nv_data_index, outlen+1, (BYTE *)data))
	{
		MercuryTrace("write nv audio data fail\n");
		LocalFree(data);
		return ERR_UPDATE_FAIL;
	}
	LocalFree(data);
	return 0;
}

static int _app_config_ad_parse(cJSON *json_ad)
{
	cJSON *json_audio = NULL, *json_interval = NULL, *json_pay = NULL, *json_fmt = NULL, *json_data = NULL;
	cJSON *json_mode = NULL, *json_period = NULL, *json_start = NULL, *json_end = NULL;
	int len;
	size_t outlen = 0;
	NvAdCfgStruct *pdata = NULL;

	json_interval = cJSON_GetObjectItem(json_ad, "interval");
	json_fmt = cJSON_GetObjectItem(json_ad, "voice-format");
	json_pay = cJSON_GetObjectItem(json_ad, "pay-count");
	json_data = cJSON_GetObjectItem(json_ad, "data");
	json_audio = cJSON_GetObjectItem(json_ad, "ad_audio");

	if (json_audio)
	{
		json_mode = cJSON_GetObjectItem(json_audio, "play-mode");
		json_period = cJSON_GetObjectItem(json_audio, "time-period");
		if (json_period)
		{
			json_start = cJSON_GetObjectItem(json_period, "starte");
			json_end = cJSON_GetObjectItem(json_period, "end");
		}
	}

	if (json_data && json_data->valuestring)
		len = strlen(json_data->valuestring);
	else
		len = 0;

	pdata = (NvAdCfgStruct *)LocalAlloc(LMEM_ZEROINIT, len + sizeof(NvAdCfgStruct));
	if (pdata == NULL)
	{
		MercuryTrace("_app_config_ad_parse malloc fail \n ");
		return ERR_MEMORY_NOT_ENOUGH;
	}

	if (len)
	{
		if (0 != mbedtls_base64_decode((unsigned char *)pdata->data, len, &outlen, (unsigned char *)json_data->valuestring, len))
		{
			MercuryTrace("base64 decode fail\n");
			LocalFree(pdata);
			return ERR_DATA_PARSE_FAIL;
		}
	}

	pdata->len = outlen;

	if (json_mode)
		pdata->mode = json_mode->valueint;
	if (json_start)
		pdata->period_start = json_start->valueint;
	if (json_end)
		pdata->period_end = json_end->valueint;

	if (json_pay)
		pdata->pay_count = json_pay->valueint;
	if (json_fmt)
		pdata->format = json_fmt->valueint;
	if (json_interval)
		pdata->interval = json_interval->valueint;

	NV_Delete(NV_AD_CFG);

	if (0 != NV_Write(NV_AD_CFG, outlen + sizeof(NvAdCfgStruct), (BYTE *)pdata))
	{
		MercuryTrace("write nv ad fail\n");
		LocalFree(pdata);
		return ERR_UPDATE_FAIL;
	}
	LocalFree(pdata);
	pdata = NULL;

	return 0;
}

static int _app_config_qrcode_parse(cJSON *json_qr)
{
	cJSON *j_url = NULL, *j_gap = NULL;
	NvQrcodeCfgStruct qr_cfg = {0};

	j_url = cJSON_GetObjectItem(json_qr, "url");
	j_gap = cJSON_GetObjectItem(json_qr, "gap");

	if(j_url == NULL || j_gap == NULL)
		return -1;

	strcpy(qr_cfg.url, j_url->valuestring);
	qr_cfg.gap = j_gap->valueint;

	NV_Delete(NV_QRCODE_CFG);
	NV_Write(NV_QRCODE_CFG, sizeof(NvQrcodeCfgStruct), (BYTE*)&qr_cfg);
	
	return 0;
}

static void _app_add_result_list(cJSON *j_succ, cJSON *j_fail, char *str, int ret)
{
	if (ret == 0)
	{
		cJSON_AddItemToArray(j_succ, cJSON_CreateString(str));
	}
	else
	{
		cJSON_AddItemToArray(j_fail, cJSON_CreateString(str));
	}
}


static int _app_config_rsp_parse(unsigned char *info)
{
	int ret = 0;
	cJSON *root = NULL, *cjson_result = NULL, *cjson_bind = NULL, *cfg_ver = NULL;
	cJSON *json_mqtt = NULL, *json_audio = NULL;
	cJSON *json_trade_template = NULL, *json_discount_template = NULL, *json_vericode_template = NULL;
	cJSON *json_poweron = NULL, *json_poweroff = NULL, *json_custom = NULL;
	cJSON *json_ad = NULL, *json_qr = NULL;

	cJSON *json_rsp = NULL, *json_suclist = NULL, *json_faillist = NULL;

	AppInfoStruct app_info = {0};
	char *str_out = NULL;

	json_rsp = cJSON_CreateObject();
	json_suclist = cJSON_CreateArray();
	json_faillist = cJSON_CreateArray();
	if (json_rsp == NULL || json_suclist == NULL || json_faillist == NULL)
	{
		MercuryTrace("_app_config_rsp_parse make response json fail\n");
		return -1;
	}

	root = cJSON_Parse((char *)info);

	if (root == NULL)
	{
		MercuryTrace("push message json error\n ");
		ret = ERR_DATA_PARSE_FAIL;		
		app_play_error_code(ret);
		goto exit;
	}

	//error-code
	cjson_result = cJSON_GetObjectItem(root, "error-code");
	if (cjson_result == NULL)
	{
		MercuryTrace("config json no error code\n ");
		ret = ERR_DATA_PARSE_FAIL;		
		app_play_error_code(ret);
		goto exit;
	}
	if (cjson_result->valueint != 0)
	{
		MercuryTrace("config json error code:%d\n ", cjson_result->valueint);
		ret = ERR_DATA_PARSE_FAIL;
		app_play_error_code(cjson_result->valueint);
		goto exit;
	}

	MercuryTrace("error code :success\n");

	//bind-state
	cjson_bind = cJSON_GetObjectItem(root, "bind-state");
	if (cjson_bind == NULL)
	{
		MercuryTrace("config json no bind state\n ");
		ret = ERR_DATA_PARSE_FAIL;
		goto exit;
	}

	app_info.bind = cjson_bind->valueint;

	MercuryTrace("bind state  :%d\n", app_info.bind);

	//app config version
	cfg_ver = cJSON_GetObjectItem(root, "app-config-version");
	if (cfg_ver == NULL)
	{
		MercuryTrace("config json no app config version\n ");
		ret = ERR_DATA_PARSE_FAIL;
		goto exit;
	}

	app_info.app_config_ver = cfg_ver->valueint;

	MercuryTrace("app_config_ver :%d\n", app_info.app_config_ver);
	//mqtt
	json_mqtt = cJSON_GetObjectItem(root, "mqtt");
	if (json_mqtt != NULL)
	{
		ret = _app_config_mqtt_parse(json_mqtt);

		if (ret != 0)
			app_play_error_code(ERR_DATA_PARSE_FAIL);
		MercuryTrace("_app_config_mqtt_parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "mqtt", ret);
	}

	//audio
	json_audio = cJSON_GetObjectItem(root, "audio");
	if (json_audio != NULL)
	{
		ret = _app_config_audio_parse(json_audio);
		MercuryTrace("_app_config_audio_parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "audio", ret);
	}

	//trade audio
	json_trade_template = cJSON_GetObjectItem(root, "trade-audio-templates");
	if (json_trade_template != NULL)
	{
		ret = _app_config_audio_template_parse(VOICE_TEMPLATE_PAY, json_trade_template);
		MercuryTrace("voice pay template parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "trade-audio-templates", ret);
	}
	//discount audio
	json_discount_template = cJSON_GetObjectItem(root, "discount-audio-templates");
	if (json_discount_template != NULL)
	{
		ret = _app_config_audio_template_parse(VOICE_TEMPLATE_DISCOUNT, json_discount_template);
		MercuryTrace("voice discount template parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "discount-audio-templates", ret);
	}

	//verify audio
	json_vericode_template = cJSON_GetObjectItem(root, "verify-code-audio-templates");
	if (json_vericode_template != NULL)
	{
		ret = _app_config_audio_template_parse(VOICE_TEMPLATE_VERIFYCODE, json_vericode_template);
		MercuryTrace("voice verify template parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "verify-code-audio-templates", ret);
	}
	//power on
	json_poweron = cJSON_GetObjectItem(root, "power-on-audio");
	if (json_poweron != NULL)
	{
		ret = _app_config_power_parse(VOICE_POWERON, json_poweron);
		MercuryTrace("power on parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "power-on-audio", ret);
	}
	//power off
	json_poweroff = cJSON_GetObjectItem(root, "power-off-audio");
	if (json_poweroff != NULL)
	{
		ret = _app_config_power_parse(VOICE_POWEROFF, json_poweroff);
		MercuryTrace("power off parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "power-off-audio", ret);
	}
	//custom
	json_custom = cJSON_GetObjectItem(root, "customer-audio");
	if (json_custom != NULL)
	{
		ret = _app_config_power_parse(VOICE_CUSTOM, json_custom);
		MercuryTrace("voice custom parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "customer-audio", ret);
	}
	//advertise
	json_ad = cJSON_GetObjectItem(root, "ad-audio");
	if (json_ad != NULL)
	{
		ret = _app_config_ad_parse(json_ad);
		MercuryTrace("voice advertisement template parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "ad-audio", ret);
	}
	json_qr = cJSON_GetObjectItem(root, "dynamic-qr");
	if (json_qr != NULL)
	{
		ret = _app_config_qrcode_parse(json_qr);
		MercuryTrace("qr code parse :%d\n", ret);
		_app_add_result_list(json_suclist, json_faillist, "dynamic-qr", ret);
	}
exit:
	if (root)
		cJSON_Delete(root);

	// cJSON_AddItemToObject(json_rsp, "model", cJSON_CreateString(app_info.model));	
	cJSON_AddItemToObject(json_rsp, "dev-sn", cJSON_CreateString(app_info.SN));
	cJSON_AddItemToObject(json_rsp, "result", cJSON_CreateNumber(ret));
	cJSON_AddItemToObject(json_rsp, "success-list", json_suclist);
	cJSON_AddItemToObject(json_rsp, "fail-list", json_faillist);

	//str_out = cJSON_Print(json_rsp);
	cJSON_Delete(json_rsp);
	MercuryTrace("root:%s\n ", root->valuestring);
	return ret;
}

typedef int (*http_callback)(unsigned char *);

int _app_http_reqest(char *serv_addr, int port, char *point, char *sendbuff, int type, http_callback p_callback);


int get_dev_config_info(void)		//��ȡ�豸������Ϣ
{
	AppInfoStruct app_info = {0};
	SIM_IMSI_T imsi;
	SIM_ICCID_T ccid;
	cJSON *root = NULL;
	char *sreq = NULL;
	char buf[8] = {0};

	if (0 != NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info))
	{
		MercuryTrace("speaker_app_init, fs_read_deviceinfo fail\n");
		return -1;
	}
	
	root = cJSON_CreateObject();
	if (root == NULL)
	{
		MercuryTrace("create device config info fail\n");
		return -1;
	}

	// if(app_info.model[0]==0)
    //     strncpy(app_info.model, "QF100", 5);

	cJSON_AddStringToObject(root, "dev-model", app_info.model);
	cJSON_AddStringToObject(root, "item-number", app_info.item_no);
	// cJSON_AddStringToObject(root, "model", app_info.model);
	cJSON_AddStringToObject(root, "dev-sn", app_info.SN);
	cJSON_AddStringToObject(root, "fw-version", APP_VERSION);
	cJSON_AddNumberToObject(root, "fw-build", FW_BUILD_VERSION);
	cJSON_AddNumberToObject(root, "app-config-version", app_info.app_config_ver);
	cJSON_AddStringToObject(root, "imei", (char *)GetImei());

	if (1 != GetSimInfo(SIM_ALL, &imsi, &ccid))
	{
		MercuryTrace("get sim imsi & ccid fail \n");
		cJSON_AddStringToObject(root, "imsi", "");
		cJSON_AddStringToObject(root, "iccid", "");
	}
	else{
		cJSON_AddStringToObject(root, "imsi", (char *)imsi.imsi_val);
		cJSON_AddStringToObject(root, "iccid", (char *)ccid.id_num);
	}

	// sreq = cJSON_Print(root);
	sreq = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);

	memset(app_info.cfg_serv_host, 0x00, sizeof(app_info.cfg_serv_host));
	strcpy(app_info.cfg_serv_host, "https://test.mosambee.in");
	app_info.cfg_serv_port = 443;

	MercuryTrace("get config req:%s\n", sreq);

	// if (0 != _app_http_reqest(app_info.cfg_serv_host, app_info.cfg_serv_port, SERVER_GET_CFG, sreq, 0, _app_config_rsp_parse))
	// {
	// 	MercuryTrace("get config_request fail \n");
	// 	LocalFree(sreq);
	// 	return -1;
	// }

	if (0 != _app_http_reqest2(app_info.cfg_serv_host, app_info.cfg_serv_port, SERVER_GET_CFG, sreq, 0, _app_config_rsp_parse))
	{
		MercuryTrace("get config_request fail \n");
		LocalFree(sreq);
		return -1;
	}

	LocalFree(sreq);

	return 0;
}
int report_update_result(int update_status)			//�ϱ��̼��������
{
	char *send = NULL;
	unsigned long handle;
	cJSON *root = NULL;
	AppInfoStruct app_info = {0};
	int ret = -1;
	int recv_len = 0;

	char rsp[128] = {0};

	if (0 != NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info))
	{
		MercuryTrace("report_update_result, fs_read_deviceinfo fail\n");
		return -1;
	}

	root = cJSON_CreateObject();
	if (root == NULL)
	{
		MercuryTrace("report_update_result create json object fail\n");
		return -1;
	}

	char url[128] = {0};

	strcpy(app_info.cfg_serv_host, "https://test.mosambee.in");
	app_info.cfg_serv_port = 443;

	// sprintf(url, "http://%s:%d%s", app_info.cfg_serv_host, app_info.cfg_serv_port, REPORT_FOTA_RESULTS);
	sprintf(url, "%s:%d%s", app_info.cfg_serv_host, app_info.cfg_serv_port, REPORT_FOTA_RESULTS);
	MercuryTrace("report_update_result(): url: %s\r\n", url);

	cJSON_AddItemToObject(root, "dev-sn", cJSON_CreateString(app_info.SN));
	cJSON_AddItemToObject(root, "model", cJSON_CreateString(app_info.model));
	if (update_status == DOWNLOAD_FAILED)
	{
		cJSON_AddItemToObject(root, "result", cJSON_CreateNumber(1009)); //����ʧ��,ԭ��δ֪
		MercuryTrace("\n DOWNLOAD_FAILED \n");
	}
	else if (update_status == UPDATE_FAILED)
	{
		cJSON_AddItemToObject(root, "result", cJSON_CreateNumber(1006)); //У��ʧ��
		MercuryTrace("\n UPDATE_FAILED \n");
	}
	else
	{
		cJSON_AddItemToObject(root, "result", cJSON_CreateNumber(0)); //���³ɹ�
		MercuryTrace("\n UPDATE_SUCCESS \n");
	}
	
	// send = cJSON_Print(root);
	send = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);

	MercuryTrace("send: (%d) - %s\r\n", strlen(send), send);
	
	sdk_http_post_auto(url,send,NULL,rsp,60,&recv_len);
	MercuryTrace("rsp len: %d\n%s\n", strlen(rsp), rsp);
	
	LocalFree(send);
	return 0;
}

int report_cfg_update_result(void)
{
	char *send = NULL;
	unsigned long handle;
	cJSON *root = NULL;	
	cJSON *success_list = NULL;	
	cJSON *fail_list = NULL;
	int recv_len = 0;

	AppInfoStruct app_info = {0};
	int ret = -1;
	char rsp_cfg[1024] = {0};

	root = cJSON_CreateObject();

	if (0 != NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info))
	{
		MercuryTrace("report_cfg_update_result, fs_read_deviceinfo fail\n");
		return -1;
	}
	if (root == NULL)
	{
		MercuryTrace("report_cfg_update_result create json object fail\n");
		return -1;
	}
	char url[128] = {0};
	sprintf(url, "http://%s:%d%s", app_info.cfg_serv_host, app_info.cfg_serv_port, REPORT_CFG_UPDATE_RESULT);

  
	// cJSON_AddItemToObject(root, "model", cJSON_CreateString(app_info.model));
	cJSON_AddItemToObject(root, "dev-sn", cJSON_CreateString(app_info.SN));
	cJSON_AddItemToObject(root, "result", cJSON_CreateNumber(0));
	cJSON_AddItemToObject(root,"success_list",cJSON_CreateString("[]"));
	cJSON_AddItemToObject(root, "fail_list", cJSON_CreateString("[]"));

	// send = cJSON_Print(root);
	send = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	// MercuryTrace("send = %s \r\n",send);

	MercuryTrace("\n url is %s \n",url);
	MercuryTrace("\n send is %s \n",send);
	sdk_http_post_auto(url, send, NULL, rsp_cfg,sizeof(rsp_cfg),&recv_len);

	// MercuryTrace("rsp_cfg len: %d\n%s\n", sizeof(rsp_cfg), rsp_cfg);

	LocalFree(send);
	//  MercuryTrace("\n....................0..............\n");
	return 0;

}


