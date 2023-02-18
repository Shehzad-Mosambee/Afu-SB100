#include "MercuryDef.h"
#include "MercuryPlat.h"

#include "SDK_API_MQTT.h"
#include "SDK_API_AUDIO.h"
#include "SDK_API_SYS.h"
#include "SDK_API_TCP.h"
#include "SDK_API_BCS_DISPLAY.h"
#include "SDK_API_WIFI.h"

#include "cJSON.h"

#include "audio.h"
#include "device.h"
#include "connect_mqtt_soundbox_server.h"
#include "nv.h"

#include "speakerapp.h"

#include "global.h"
#include "keypad.h"

// extern DWORD volume;
unsigned long l_soundbox_mqtt_handle = 0;
static BOOL is_soundbox_mqtt_close = TRUE;

HANDLE l_soundbox_event_network_success;
int soundbox_mqtt_lost = 0;

int is_lpm = 0;

AppMqttCfgStruct l_soundbox_mqtt_cfg;

unsigned long app_mqtt_soundbox_get_handle(void)
{
	return l_soundbox_mqtt_handle;
}

/*
static cJSON *_get_mqtt_soundbox_cfg_json(void)
{
	int nMqttDataLen;
	BYTE pLenBuff[2];
	// read MQTT JSON string length
	if (0 != NV_Read(NV_MQTT_CFG, 2, pLenBuff))
	{
		MercuryTrace("read mqtt cfg length fail \n");
		return -1;
	}

	// ����MQTT JSON ������
	nMqttDataLen = (pLenBuff[0] << 8) + pLenBuff[1];

	// ���� JSON ��������
#ifdef QYY_SER_VER
	BYTE *pDataBuff = malloc(2048); //�л��������׵İ�����������ƽ̨
#else
	BYTE *pDataBuff = malloc(nMqttDataLen + 2); //�л��������İ�����������ƽ̨
#endif
	// ��ȡJSON��
	if (0 != NV_Read(NV_MQTT_CFG, nMqttDataLen + 2, pDataBuff))
	{
		MercuryTrace("read mqtt cfg data fail \n");
		return -1;
	}

#ifdef QYY_SER_VER //�л���QYY�����������豸

	MercuryTrace("Before Change mqtt cfg to QYY \n");
	MercuryTrace("mqtt cfg: %s \n", (char *)(pDataBuff + 2));
	_test_qyy_iot_dev((char *)(pDataBuff + 2));
	MercuryTrace("After Change mqtt cfg to QYY \n");
	MercuryTrace("mqtt cfg: %s \n", (char *)(pDataBuff + 2));
#endif

	// ���� JSON ���ɶ���
	cJSON *pMqttJson = cJSON_Parse((char *)(pDataBuff + 2));
	// �ͷ� JSON ��������
	free(pDataBuff);

	return pMqttJson;
}

static char *_get_mqtt_soundbox_cfg_str(cJSON *pMqttJson, char *sKey)
{
	cJSON *pValue = cJSON_GetObjectItem(pMqttJson, sKey);
	return pValue->valuestring;
}

static int _get_mqtt_soundbox_cfg_int(cJSON *pMqttJson, char *sKey)
{
	cJSON *pValue = cJSON_GetObjectItem(pMqttJson, sKey);
	return pValue->valueint;
}


static BOOL _get_mqtt_soundbox_cfg(void)
{
	// �ͷžɵ� MQTT JSON ����
	if (l_pMqttJson != NULL)
	{
		cJSON_Delete(l_pMqttJson);
	}
	l_pMqttJson = _get_mqtt_cfg_json();
	if (NULL == l_pMqttJson)
	{
		MercuryTrace("read mqtt cfg fail 1 \n");
		return FALSE;
	}

	MercuryTrace("_get_mqtt_cfg_json SUCCESS \n");
	strcpy(l_soundbox_mqtt_cfg.ip,_get_mqtt_cfg_str(l_pMqttJson, "broker-ip"));
	l_soundbox_mqtt_cfg.port = _get_mqtt_cfg_int(l_pMqttJson, "broker-port");
	strcpy(l_soundbox_mqtt_cfg.clientid,_get_mqtt_cfg_str(l_pMqttJson, "client-id"));
	strcpy(l_soundbox_mqtt_cfg.username,_get_mqtt_cfg_str(l_pMqttJson, "user-name"));
	strcpy(l_soundbox_mqtt_cfg.password,_get_mqtt_cfg_str(l_pMqttJson, "password"));
	strcpy(l_soundbox_mqtt_cfg.subtopic,_get_mqtt_cfg_str(l_pMqttJson, "subscribe-topic"));
	strcpy(l_soundbox_mqtt_cfg.pubtopic,_get_mqtt_cfg_str(l_pMqttJson, "publish-topic"));
	l_soundbox_mqtt_cfg.keep_alive = _get_mqtt_cfg_int(l_pMqttJson, "keep-alive");


	MercuryTrace("1.............$...........%s\r\n",l_soundbox_mqtt_cfg.ip);
	MercuryTrace("2..............$..........%d\r\n",l_soundbox_mqtt_cfg.port);
	MercuryTrace("3...............$.........%s\r\n",l_soundbox_mqtt_cfg.clientid);
	MercuryTrace("4................$........%s\r\n",l_soundbox_mqtt_cfg.username);
	MercuryTrace("5.................$.......%s\r\n",l_soundbox_mqtt_cfg.password);
	return TRUE;
}
*/
void mqtt_soundbox_lost_callback( void *context, const char *cause )
{
	is_soundbox_mqtt_close = TRUE;
	set_soundbox_network_success_event();
	MercuryTrace("mqtt lost\r\n");
	MercuryTrace("reconnecting......\r\n");

	// MercuryTrace("reconnecting......%d\r\n",soundbox_mqtt_lost);
	// soundbox_mqtt_lost++;
	// reget_mqtt_soundbox_info();
}

void reget_mqtt_soundbox_info()
{
	if (soundbox_mqtt_lost > 10)
	{
		if (0 == get_dev_config_info())
		{
			soundbox_mqtt_lost = 0;
		}
	}
}



HANDLE handle_bcs_clear_timer = NULL;
static void bcs_clear_timer_cb(ULONG arg)
{
	handle_bcs_clear_timer = NULL;
	
	sdk_bcs_number(0, 0);
	qyy_bcs_set_speaker(FALSE);
	// anfu_led_off();

	DeleteTimer(handle_bcs_clear_timer);
	handle_bcs_clear_timer = NULL;

}

void bcs_clear_timer_reset(void)
{
	if(IsTimerActive(handle_bcs_clear_timer))
		DeactiveTimer(handle_bcs_clear_timer);

    ChangeTimer(handle_bcs_clear_timer, bcs_clear_timer_cb, 5000);
	ActiveTimer(handle_bcs_clear_timer);
}

// static void start_bcs_clear_timer(void)
void start_bcs_clear_timer(void)
{
	TIMER_CONFIG_S bcs_cfg_timer;

	// if (0 == sdk_audio_playing())
    // 	trans_record_play(0);

	MercuryTrace("start_bcs_clear_timer----------------------------------------\r\n");

	if(NULL != handle_bcs_clear_timer)
	{
		bcs_clear_timer_reset();
		return;
	}

	bcs_cfg_timer.timer_name = "BCS_CLEAR_TIMER";
	bcs_cfg_timer.timer_fun = bcs_clear_timer_cb;
	bcs_cfg_timer.input = 0;
	bcs_cfg_timer.timer_expire = 5 * 1000;
	bcs_cfg_timer.period = TIMER_ONCE_TIME;	// once mode
	bcs_cfg_timer.auto_activate = TIMER_AUTO_ACTIVATE;

	handle_bcs_clear_timer = CreateTimer(&bcs_cfg_timer);
}


// msg_count = 0;
void message_callback_soundbox(MessageData* data)
{
	int tlen = 0;	
	char *topic = NULL;
	char *payload = NULL;
	int payloadlen = 0;
	MQTTClient *mqtt_client = NULL;

	if (data == NULL || data->topicName == NULL || data->message == NULL || data->message->payloadlen == 0)
	{
		MercuryTrace("messageCallback error\n ");
		return;
	}

	play_trans_mode = 0;		// Switch off last 10 txn mode
	network_select_mode = 0;	// Switch off N/W select mode

	anfu_led_off();
	
	payload = LocalAlloc(LMEM_ZEROINIT, data->message->payloadlen+1 );
	memcpy(payload, data->message->payload, data->message->payloadlen);
	payloadlen = data->message->payloadlen;
	payload[payloadlen] = 0;
	
	MercuryTrace("payload: len(%d), content:%s\n", payloadlen, payload);

	if(ERROR == parseTxnDetails_new(payload))
	{
		LocalFree(payload);
		return;
	}

	if(mqtt_txn_details.notifType == '4') 
	{
		LocalFree(payload);

		PowerOffRebootDevice(0);
		return;
	}

	if(mqtt_txn_details.notifType == '5') 
	{
		LocalFree(payload);
		
		PowerOffRebootDevice(1);
		return;
	}

	if(mqtt_txn_details.notifType == '0')
		trans_record_write(payload);
		
	LocalFree(payload);

	anfu_led_on();

	if(mqtt_txn_details.notifType != '2')
		play_notification_sound();
	
	start_bcs_clear_timer();
	play_amount_mosambee((unsigned char)0);

	anfu_led_off();
}

void message_callback_soundbox_p2p(MessageData* data)
{
	// int tlen = 0;	
	// // char *topic = NULL;
	// char *payload = NULL;
	// int payloadlen = 0;
	// MQTTClient *mqtt_client = NULL;

	// if (data == NULL || data->message == NULL || data->message->payloadlen == 0)
	// {
	// 	MercuryTrace("messageCallback error\n ");
	// 	return;
	// }
	
	// payload = data->message->payload;
	// payloadlen = data->message->payloadlen;
	// payload[payloadlen] = 0;
	
	// // MercuryTrace("topic(%s)\n", topic);
	// MercuryTrace("payload, len(%d), data:%s\n", payloadlen, payload);
	// qyy_mqtt_parse_payload(payload, payloadlen);

	// // LocalFree(topic);
}



int app_mqtt_soundbox_publish(unsigned long handle, const char *topic, unsigned long qos, void *payload, int payloadlen)
{
    if(handle)
        return sdk_MQTT_publish(handle,topic,qos,payload,payloadlen);
}

int app_mqtt_soundbox_subcribe(unsigned long handle, const char* topic, unsigned long qos,MSG_ARRIVED messageCallback)
{
    if(handle)
        return sdk_MQTT_subcribe(handle, topic, qos, messageCallback);
}

// char ota_topic[128];
// extern void aliyun_ota_cb(MessageData* data);

// bool _isAliIot() {
// 	MercuryTrace("\n........5............. %s\r\n",l_soundbox_mqtt_cfg.username+13);
// 	MercuryTrace("\n........9............. %s\r\n",l_soundbox_mqtt_cfg.productkey);

// 	return (0 == strcmp(l_soundbox_mqtt_cfg.productkey,l_soundbox_mqtt_cfg.username+13));
	
// }

int app_mqtt_soundbox_connect(void)
{
	int ret;
    char ipaddr[64] = {0};
	
	// // read MQTT 
	// if (0 != NV_Read(NV_MQTT_CFG, sizeof(AppMqttCfgStruct), &l_soundbox_mqtt_cfg))
	// {
	// 	MercuryTrace("read mqtt cfg info fail \n");
	// 	return -1;
	// }

	char snData[32] = {0};
    NV_Read(NV_SN, 32, snData);
    MercuryTrace("NV-SN: %s\r\n", snData);

    memset(&l_soundbox_mqtt_cfg, 0x00, sizeof(AppMqttCfgStruct));
    

	#if defined(DEVELOPMENT_BUILD)
	strcpy(l_soundbox_mqtt_cfg.ip, MQTT_ADDRESS_UAT);
	l_soundbox_mqtt_cfg.port = MQTT_PORT_UAT;
	#elif defined(PRODUCTION_BUILD)
	strcpy(l_soundbox_mqtt_cfg.ip, MQTT_ADDRESS_PROD);
	l_soundbox_mqtt_cfg.port = MQTT_PORT_PROD;
	#endif
	
	// strcpy(l_soundbox_mqtt_cfg.password, generatePassword(snData));
	strcpy(l_soundbox_mqtt_cfg.password, generatePassword(RegisterDevice.clientID));


	// strcpy(l_soundbox_mqtt_cfg.clientid, snData);
    // strcpy(l_soundbox_mqtt_cfg.username, snData);
	strcpy(l_soundbox_mqtt_cfg.clientid, RegisterDevice.clientID);
    strcpy(l_soundbox_mqtt_cfg.username, RegisterDevice.userName);

	// strcpy(l_soundbox_mqtt_cfg.subtopic, "notification/9029686961");
	strcpy(l_soundbox_mqtt_cfg.subtopic, RegisterDevice.subscriptionTopic);
    l_soundbox_mqtt_cfg.keep_alive = 60;
 
	// #if defined(DEVELOPMENT_BUILD)
	MercuryTrace("\n........1.Host............ %s\r\n",l_soundbox_mqtt_cfg.ip);
	MercuryTrace("\n........2.Port............ %d\r\n",l_soundbox_mqtt_cfg.port);
	MercuryTrace("\n........3.ClientID........ %s\r\n",l_soundbox_mqtt_cfg.clientid);
	MercuryTrace("\n........4.UserName........ %s\r\n",l_soundbox_mqtt_cfg.username);
	MercuryTrace("\n........5.Password........ %s\r\n",l_soundbox_mqtt_cfg.password);
	MercuryTrace("\n........7.Topic........... %s\r\n",l_soundbox_mqtt_cfg.subtopic);
	MercuryTrace("\n........8.CleanSession.... %d\r\n",RegisterDevice.cleanSession);
	MercuryTrace("\n........9.QOS............. %d\r\n",RegisterDevice.QOS);
	MercuryTrace("\n.......10.Keep-Alive...... %d\r\n",RegisterDevice.keepAlive);
	// MercuryTrace("\n.......10.RetainFlag...... %d\r\n",(int)RegisterDevice.retainFlag);
	


	// #endif

	// MercuryTrace("\n........7............. %s\r\n",l_soundbox_mqtt_cfg.pubtopic);
	// MercuryTrace("\n........8............. %s\r\n",l_soundbox_mqtt_cfg.productkey);

	hosttoip(l_soundbox_mqtt_cfg.ip, ipaddr);
	MercuryTrace("hostname:%s, ipaddr:%s, id:%s, name:%s, pwd:%s\n",l_soundbox_mqtt_cfg.ip, ipaddr, l_soundbox_mqtt_cfg.clientid, l_soundbox_mqtt_cfg.username,l_soundbox_mqtt_cfg.password);
	ret = sdk_MQTT_connect(ipaddr, l_soundbox_mqtt_cfg.port,
			(l_soundbox_mqtt_cfg.port==8883)?1:0, 60,
			l_soundbox_mqtt_cfg.clientid,
			l_soundbox_mqtt_cfg.username,
			l_soundbox_mqtt_cfg.password,
			RegisterDevice.keepAlive /*60*/,
			mqtt_soundbox_lost_callback,&l_soundbox_mqtt_handle, 0, NULL, NULL, 60, RegisterDevice.cleanSession);

	MercuryTrace("MQTT_connect ret = %d\r\n",ret);

	// // if ( 11 == strlen(l_soundbox_mqtt_cfg.productkey)) //�������productkey����Ϊ�ǰ������豸
	// if ( _isAliIot() ) //�ж��Ƿ������豸
	// {
	// 	char devicename[64] = {0};
	// 	char *p = NULL;
		
	// 	strcpy(devicename, l_soundbox_mqtt_cfg.username);
	// 	p = strchr(devicename, '&');
	// 	if (p)
	// 		*p = 0;
		
	// 	//username�ĸ�ʽΪdevicename&productkey

	// 	//OTA����
	// 	// /ota/device/upgrade/${YourProductKey}/${YourDeviceName}
	// 	sprintf(ota_topic, "/ota/device/upgrade/%s/%s", l_soundbox_mqtt_cfg.productkey, devicename);
	// 	ret = sdk_MQTT_subcribe(l_soundbox_mqtt_handle, ota_topic, 0, aliyun_ota_cb);

	// 	MercuryTrace("subscribe OTA topic:%d, %s\n",ret, ota_topic);
	// }
    
	return ret;
}

int app_publish_soundbox_message(unsigned long handle, const char *topic, unsigned long qos)
{
	int ret;
	char *sreq = NULL;
	cJSON *root = NULL,*json_header = NULL,*json_data = NULL;

	root = cJSON_CreateObject();
	json_header = cJSON_CreateObject();
	json_data = cJSON_CreateObject();

	if (root == NULL || json_header == NULL || json_data == NULL)
	{
		MercuryTrace("app_publish_soundbox_message create json object fail\n");
		return -1;
	}

	// header
	cJSON_AddItemToObject(json_header, "id", cJSON_CreateNumber(1234));
	cJSON_AddItemToObject(json_header, "version", cJSON_CreateNumber(1));
	cJSON_AddItemToObject(json_header, "category", cJSON_CreateNumber(1001));
	// root
	cJSON_AddItemToObject(root, "header", json_header);


	// data
	cJSON_AddItemToObject(json_data, "dev-id", cJSON_CreateNumber(DEV_CLINET_ID));
	cJSON_AddItemToObject(json_data, "audio-type", cJSON_CreateNumber(app_GetAudioType()));
	// root
	cJSON_AddItemToObject(root, "data", json_data);

	// sreq = cJSON_Print(root);
	sreq = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);

	MercuryTrace("report lbs req:%s\n", sreq);

	ret = app_mqtt_soundbox_publish(handle,topic,qos,sreq,strlen(sreq));
	LocalFree(sreq);

	return 0;
}


// static void mqtt_close_connection_cb(ULONG arg)
// {
// 	MercuryTrace("-- Closing MQTT Connection --\r\n");
// 	sdk_MQTT_close(l_soundbox_mqtt_handle);
// 	// return MQTT_RECONNECT;
// }


int mqtt_soundbox_processing(char *name)
{
	int ret;
	MercuryTrace("mqtt_soundbox_processing name = %s\r\n",name);
	
	// sdk_audio_play_nv(AUDIO_CONNECTING+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CONNECTING));
	play_audio_by_name("conn", (unsigned char)0);

	// mqtt connect
	ret = app_mqtt_soundbox_connect();
	if (0 != ret)
	{
		int i = 0;
		
		// // app_audio_play(AUDIO_MQTT_SERVER_FAIL);
		// sdk_audio_play_nv(AUDIO_SERVER_CONNECT_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SERVER_CONNECT_FAILED));
		play_audio_by_name("servf", (unsigned char)0);

		MercuryTrace("MQTT_connect fail\r\n");
		return MQTT_RECONNECT;
	}
	//// app_audio_play(AUDIO_MQTT_CONNECT);
	
    // commented for checking purpose
	// sdk_MQTT_SetDefault_handler(l_soundbox_mqtt_handle, message_callback_soundbox_p2p);
	
	MercuryTrace("Subscribing to: %s\r\n", l_soundbox_mqtt_cfg.subtopic);
    //MQTT subcribe
	if (strlen(l_soundbox_mqtt_cfg.subtopic))
	{
		ret = app_mqtt_soundbox_subcribe(l_soundbox_mqtt_handle, l_soundbox_mqtt_cfg.subtopic, (unsigned long)RegisterDevice.QOS, message_callback_soundbox );
	    MercuryTrace("MQTT_subscribe topic==%s\r\n", l_soundbox_mqtt_cfg.subtopic);

		if(0 != ret)
		{
			sdk_MQTT_close(l_soundbox_mqtt_handle);
			// app_audio_play(AUDIO_MQTT_SUBSCRIBE_FAIL);
			return MQTT_RECONNECT;
		}
		
		
		//// app_audio_play(AUDIO_MQTT_SUBSCRIBE_SUCCESS);
		// sdk_audio_play_nv(AUDIO_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SUCCESS));
		// sdk_audio_play_nv(AUDIO_SERVER_CONNECT_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SERVER_CONNECT_SUCCESS));
		play_audio_by_name("pairCmp", (unsigned char)0);
		play_audio_by_name("servs", (unsigned char)0);

	}
	// sdk_MQTT_SetDefault_handler(l_soundbox_mqtt_handle,message_callback_p2p);

	// // app_audio_play(AUDIO_MQTT_SERVER_CONNECT);





	// // 1-hr Re-Connect Timer
	// TIMER_CONFIG_S mq_recon_timer;
	// volatile HANDLE mq_recon_handle = NULL;
	
	// if(NULL != mq_recon_handle)
	// {
	// 	if(IsTimerActive(mq_recon_handle))
	// 	{
	// 		DeactiveTimer(mq_recon_handle);
	// 	}
	// 	DeleteTimer(mq_recon_handle);
	// 	mq_recon_handle = NULL;
	// }

	// mq_recon_timer.timer_name = "MQTT_RECON_TIMER";
	// mq_recon_timer.timer_fun = mqtt_close_connection_cb;
	// mq_recon_timer.input = 0;
	// // mq_recon_timer.timer_expire = 60 * 60 * 1000;	// 1hr - ms
	// mq_recon_timer.timer_expire = 30 * 1000;	// 1min
	// mq_recon_timer.period = TIMER_PERIOD;	// cycle mode
	// mq_recon_timer.auto_activate = TIMER_AUTO_ACTIVATE;
	// mq_recon_handle = CreateTimer(&mq_recon_timer);

	// mqtt yield
	is_soundbox_mqtt_close = FALSE;
	while(!is_soundbox_mqtt_close )
	{
		ret = sdk_MQTT_yield( l_soundbox_mqtt_handle, 1000);

		// Low power mode - Only for 4G network
		struct st_qyy_power_save cfg = {1, 0, 400};
		if(NETWORK_MODE_GPRS == sdk_get_network_mode())
		{
			if(ret == 0 && is_lpm == 0)
			{
				qyy_power_save(&cfg);
				is_lpm = 1;
			}
		}

		if(0 != ret)
		{
			MercuryTrace("sdk_MQTT_yield() ret = %d\r\n", ret);
			sdk_MQTT_close(l_soundbox_mqtt_handle);
			return MQTT_RECONNECT;
		}

		// Sleep(2*1000);
		// if(NULL != mq_recon_handle)
		// {
		// 	if(IsTimerActive(mq_recon_handle))
		// 	{
		// 		MercuryTrace("-- MQTT re-connect timer ACTIVE --\r\n");
		// 	}
		// 	else
		// 	{
		// 		MercuryTrace("-- MQTT re-connect timer EXHAUSTED --\r\n");
		// 	}	
		// }
	}

	sdk_MQTT_close(l_soundbox_mqtt_handle);

	// sdk_audio_play_nv(AUDIO_SERVER_DISCONNECTED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SERVER_DISCONNECTED));
	// sdk_audio_play_nv(AUDIO_RECONNECTING+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_RECONNECTING));
	play_audio_by_name("servd", (unsigned char)0);
	play_audio_by_name("reconn", (unsigned char)0);

	return MQTT_RECONNECT;
}

void set_soundbox_network_success_event(void)
{
    SetEvent(l_soundbox_event_network_success);
}

BOOL is_soundbox_network_connected()
{
	int network_mode;
	int gprs_network_status;
	int wifi_network_status;
	int pingtime = 5 * 1000;
	int ret = 0;
	network_mode = sdk_get_network_mode();
	wifi_network_status = sdk_get_wifi_init_OK();
	if (network_mode == NETWORK_MODE_GPRS)
	{
        gprs_network_status = sdk_get_gprs_network_status();
        if(GPRS_NETWORK_STATUS_OK == gprs_network_status)
        {
			ret = 1;
			return ret;
		}else{
			ret = 0;
			return ret;
		}
	}
	else if (network_mode == NETWORK_MODE_WIFI)
	{
		// if (TRUE == wifi_network_status  && 0 == sdk_wifi_ping("114.114.114.114", &pingtime) )
		if (TRUE == wifi_network_status  && 0 == sdk_wifi_ping("8.8.8.8", &pingtime) )
		{
			ret = 1;
			return ret;
		}
		else
		{
			ret = 0;
			return ret;
		}
	}
	return ret;
}

extern unsigned long l_soundbox_mqtt_handle;
void mqtt_soundbox_close_thread()
{
    sdk_tcp_close(l_soundbox_mqtt_handle);        //Ҳ�������̶Ͽ�mqtt����������Ӱ��δ֪��
    // sdk_MQTT_close(l_soundbox_mqtt_handle);    //����������£��ضϿ��ܻ�����ʱ��
    MercuryTrace("sdk_tcp_closeing..\n");
}

extern g_event_ota_update;
void mqtt_soundbox_thread(ULONG argc, LPVOID lpThreadParameter)
{
	int network_mode;
	l_soundbox_event_network_success = CreateEvent(NULL, TRUE, FALSE, "NETWORK-SUCCESS");

	// AppInfoStruct app_info = {0};
	// if (0 == NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info))
	// {
	// 	cfg_done = app_info.cfg_done;
	// }

	// app_qrcode_cfg_init();

	while (1)
	{
		if (!is_soundbox_network_connected())
		{
			if (0 != WaitForSingleObject(l_soundbox_event_network_success, 3 * 1000))
				continue;
			ResetEvent(l_soundbox_event_network_success);
		}

		// sdk_light_control(LED_BLUE);

		network_mode = sdk_get_network_mode();
		if (network_mode == NETWORK_MODE_GPRS)
		{
			// network online
			qyy_bcs_set_rssi(GPRS_ICON, FALSE);

		}
		else if (network_mode == NETWORK_MODE_WIFI)
		{
			// network online
			sdk_wifi_set_dns_server(TRUE, 1, "8.8.8.8");
			qyy_bcs_set_rssi(WIFI_ICON, FALSE);
		}

		// app_get_update_info();

        // if (cfg_done == 0) //
        // {
        //     // // app_audio_play(AUDIO_GET_SERV_CONFIG);
        //     if (0 == get_dev_config_info())
        //     {
        //         cfg_done = 1;

        //         if (0 == NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info))
        //         {
        //             app_info.cfg_done = 1;
        //             NV_Write(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);
        //         }
        //         // // app_audio_play(AUDIO_GET_SERVCFG_SUCC);
        //         report_cfg_update_result();
        //         mqtt_processing(lpThreadParameter);
        //     }
        //     else
        //     {
        //         // app_audio_play(AUDIO_GET_SERVCFG_FAIL);
        //         Sleep(1000);
        //     }
        // }
        // else
        //     mqtt_processing(lpThreadParameter);


        mqtt_soundbox_processing(lpThreadParameter);
		Sleep(3000);
	}
}
