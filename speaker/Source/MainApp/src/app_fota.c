#include "MercuryDef.h"
#include "MercuryPlat.h"

#include "SDK_API_FOTA.h"
#include "SDK_API_CONFIG.h"
#include "SDK_API_POWER.h"
#include "SDK_API_AUDIO.h"

#include "app_fota.h"
#include "cJSON.h"
#include "speakerapp.h"
#include "nv.h"
#include "audio.h"
#include "device.h"

OtaInfoStruct ota_info;
extern HANDLE keypad_thread_handle;

volatile int ota_thread_finished = 1;

VOID ota_thread(ULONG argc, LPVOID lpThreadParameter)
{
	ota_thread_finished = 0;

	int ret = 0;
	cJSON *root = NULL;
	char *sreq;
	AppInfoStruct app_info = {0};

	TerminateThread(keypad_thread_handle, NULL);

	if (0 != NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info))
	{
		MercuryTrace("ota_thread, fs_read_deviceinfo fail\n");
		ota_thread_finished = 1;
		return;
	}

	MercuryTrace("ota_thread, ota_info.fota_type = %d\r\n", ota_info.fota_type);

	if (ota_info.fota_type)
		sdk_set_ota_mode(1, QYY_FOTA_TYPE_DIFF);
	else
		sdk_set_ota_mode(1, QYY_FOTA_TYPE_DEFAULT);

	// // sdk_voiceplay(1, "�̼�������,���Ժ�"); //Please wait while firmware download
	// sdk_audio_play_nv(AUDIO_START_UPDATE+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_START_UPDATE));
	play_audio_by_name("sUpd", (unsigned char)0);

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "dev-sn", app_info.SN);
	// cJSON_AddStringToObject(root, "model", app_info.model);
	// cJSON_AddStringToObject(root, "dev-sn", "H1009000302");
	// sreq = cJSON_Print(root);
	sreq = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);


	MercuryTrace("----------------------------------------------\r\n");
	MercuryTrace("ota_info.ota_url: %s\r\n", ota_info.ota_url);
	MercuryTrace("sreq: %s\r\n", sreq);
	MercuryTrace("ota_info.file_len: %d\r\n", ota_info.file_len);
	MercuryTrace("----------------------------------------------\r\n");
	

	//�̼�����
	//ret = sdk_fota_download_http_ex(ota_info.ota_url, sreq, ota_info.file_len );
	//ret = sdk_fota_download_http_plain(ota_info.ota_url,NULL, ota_info.file_len );
	
	ret = sdk_fota_download_http_plain(ota_info.ota_url, sreq, ota_info.file_len );
	MercuryTrace("sdk_fota_download_http_plain() ret: %d\r\n", ret);


	// ret = sdk_fota_download_http_plain("http://101.132.112.165:10073/downFile?download-url=29.img", sreq, 449376);
	// MercuryTrace("sdk_fota_download_http_plain() ret: %d\r\n", ret);
	
	LocalFree(sreq);

	if (ret == 0)
	{
		// // sdk_voiceplay(1, "�̼����سɹ�"); //Firmware downloaded successfully
		// // sdk_voiceplay(1, "�̼�������,���Ժ�"); //Please wait while firmware is being updated

		ret = qyy_fota_update(ota_info.check_value, ALGORITHMS_TYPE_SHA256);
		MercuryTrace("qyy_fota_update() ret: %d\r\n", ret);

		if (ret != 0)
		{
			MercuryTrace("%s,%d Error! ret(0x%x) \n", __func__, __LINE__, ret);
			// // sdk_voiceplay(1, "�̼�����ʧ��"); //Firmware update failed
			// sdk_audio_play_nv(AUDIO_UPDATE_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_UPDATE_FAILED));
			play_audio_by_name("sUpdF", (unsigned char)0);

			// ota_thread_finished = 1;

			MercuryTrace("\n update_failed \n");
			report_update_result(UPDATE_FAILED);
			
			set_restart_reason( E_RST_REBOOT );
			Sleep(9000);
			PowerReboot();
		}
		else
		{
			// // sdk_voiceplay(1, "�̼����³ɹ�"); //Firmware upgrade successful
			// sdk_audio_play_nv(AUDIO_UPDATE_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_UPDATE_SUCCESS));
			play_audio_by_name("sUpdS", (unsigned char)0);
			

			MercuryTrace("\nupdate_success\n");
			report_update_result(UPDATE_SUCCESS);
			// report_update_result_aliyun();
			MercuryTrace("\nupdate_success\n");

			// if (0 == get_dev_config_info())
			// {
			// 	AppInfoStruct data = {0};

			// 	if (0 != NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), &data))
			// 	{
			// 		MercuryTrace("get app info fail\n");
			// 		ota_thread_finished = 1;
			// 		return -1;
			// 	}
			// 	data.cfg_done = 1;
			// 	NV_Write(NV_SYS_INFO, sizeof(AppInfoStruct), &data);
			// 	report_cfg_update_result(); 
			// }


			set_restart_reason( E_RST_REBOOT );
			Sleep(9000);
			PowerReboot();
		}

	}
	else
	{
		MercuryTrace("Error! ret(0x%x) \n", ret);
		// // sdk_voiceplay(1, "�̼�����ʧ��"); //Firmware download failure
		// sdk_audio_play_nv(AUDIO_DOWNLOAD_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_DOWNLOAD_FAILED));
		play_audio_by_name("dFail", (unsigned char)0);

		// ota_thread_finished = 1;
		
		MercuryTrace("\n download_failed \n");
		report_update_result(DOWNLOAD_FAILED);
		
		Sleep(9000);
		// qyyft_set_restart_reason(E_RST_REBOOT);
		PowerReboot();
	}

	ota_thread_finished = 1;
	ExitThread(NULL);
}

static int _app_check_update_rsp_parse(unsigned char *info)
{
	int i, ret = -1;
	cJSON *json_rsp = NULL, *json_result = NULL, *json_ver = NULL, *json_build = NULL;
	cJSON *json_len = NULL, *json_hash = NULL, *json_url = NULL;
	DWORD ota_threadid;
	int outlen;
	// MercuryTrace("\n info is %s\n",info);
	json_rsp = cJSON_Parse(info);
	if (json_rsp == NULL)
	{
		MercuryTrace("check update json fail\n");
		ota_thread_finished = 1;
		return -1;
	}

	json_result = cJSON_GetObjectItem(json_rsp, "error-code");

	if (json_result->valueint != 0)
	{
		MercuryTrace("No need to update\n");
		// ota_thread_finished = 1;
		ret = ERR_DATA_PARSE_FAIL;
		goto exit;
	}

	json_ver = cJSON_GetObjectItem(json_rsp, "version");
	json_build = cJSON_GetObjectItem(json_rsp, "build");
	json_len = cJSON_GetObjectItem(json_rsp, "file-length");
	json_hash = cJSON_GetObjectItem(json_rsp, "file-hash");
	json_url = cJSON_GetObjectItem(json_rsp, "download-url");

	MercuryTrace("\n error-code is %d \n",json_result->valueint);
	MercuryTrace("\n json_ver is %s \n",json_ver->valuestring);
	MercuryTrace("\n json_build is %d \n",json_build->valueint);
	MercuryTrace("\n json_len is %d \n",json_len->valueint);
	MercuryTrace("\n json_hash is %s \n",json_hash->valuestring);
	MercuryTrace("\n json_url is %s \n",json_url->valuestring);


	if (json_result == NULL || json_ver == NULL || json_build == NULL || json_len == NULL || json_hash == NULL || json_url == NULL)
	{
		MercuryTrace("update json no enough items\n ");
		ret = ERR_DATA_PARSE_FAIL;
		// ota_thread_finished = 1;
		goto exit;
	}
	if (json_result->valueint != 0)
	{
		MercuryTrace("update json error code:%d\n ", json_result->valueint);
		ret = ERR_DATA_PARSE_FAIL;
		// ota_thread_finished = 1;
		goto exit;
	}

	if (json_build->valueint <= FW_BUILD_VERSION) //The server version is earlier than the device version
	{
		MercuryTrace("no new fw version \n ");
		ret = OTA_NOT_UPDATE;
		// ota_thread_finished = 1;
		goto exit;
	}

	memset(&ota_info,0, sizeof(OtaInfoStruct));
	strcpy(ota_info.ota_url, json_url->valuestring); //Get the upgrade URL and save it in OTA_URL
	strcpy(ota_info.New_Version, json_ver->valuestring); //Get the new version number in the New_Version buff
	ota_info.build_no = json_build->valueint;
	ota_info.file_len = json_len->valueint;

	MercuryTrace("json_hash->valuestring = %s\r\n", json_hash->valuestring);

	memset(ota_info.check_value, 0, sizeof(ota_info.check_value));
	mbedtls_base64_decode(ota_info.check_value, sizeof(ota_info.check_value), &outlen,  json_hash->valuestring, strlen(json_hash->valuestring));

	//Create a thread for OTA upgrades
	if (NULL == CreateThread(NULL, 60 * 1024, ota_thread, 0, NULL, STACK_SIZE_RESERVATION, &ota_threadid))
	{
		MercuryTrace("%s,%d errno=%lu\n", __func__, __LINE__, GetLastError());
		// ota_thread_finished = 1;
		ret = -1;
		goto exit;
	}

	ota_thread_finished = 0; // needs update
	cJSON_Delete(json_rsp);
	return OTA_NEED_UPDATE;

exit:
	if (json_rsp)
		cJSON_Delete(json_rsp);


	ota_thread_finished = 1;
	return ret;
}


int _app_http_reqest(char *serv_addr, int port, char *point, char *sendbuff, int type, http_callback p_callback)
{
	int ret = -1;
	char *mqtt_cfg_buff = NULL;
	char url[128] = {0};
	int recv_len = 0;

	mqtt_cfg_buff = LocalAlloc(LMEM_ZEROINIT, 3072);

	sprintf(url, "http://%s:%d%s", serv_addr, port, point);
	
	MercuryTrace("\n...............................%s\n",url);

	if (type == 0) //Get
		ret = sdk_http_get(url, sendbuff,NULL,mqtt_cfg_buff, 3072,&recv_len);
	else
		ret = sdk_http_post_auto(url, sendbuff, NULL, mqtt_cfg_buff,3072,&recv_len);

	// MercuryTrace("\n mqtt_cfg_buff is %s\n",mqtt_cfg_buff);
	if (ret == 0)
	{
		MercuryTrace("recv len: %d\n", strlen(mqtt_cfg_buff));
		if (p_callback)
			ret = p_callback((unsigned char *)mqtt_cfg_buff);
	}
	else
	{
		MercuryTrace("not recv rsp \n");
	}
 	LocalFree(mqtt_cfg_buff);
	return ret;
}


int _app_http_reqest2(char *serv_addr, int port, char *point, char *sendbuff, int type, http_callback p_callback)
{
	int ret = -1;
	char *mqtt_cfg_buff = NULL;
	char url[128] = {0};
	int recv_len = 0;

	mqtt_cfg_buff = LocalAlloc(LMEM_ZEROINIT, 3072);

	sprintf(url, "%s:%d%s", serv_addr, port, point);
	
	MercuryTrace("\n...............................%s\n",url);

	if (type == 0) //Get
		ret = sdk_http_get(url, sendbuff,NULL,mqtt_cfg_buff, 3072,&recv_len);
	else
		ret = sdk_http_post_auto(url, sendbuff, NULL, mqtt_cfg_buff,3072,&recv_len);

	// MercuryTrace("\n mqtt_cfg_buff is %s\n",mqtt_cfg_buff);
	if (ret == 0)
	{
		MercuryTrace("recv len: %d\n", strlen(mqtt_cfg_buff));
		if (p_callback)
			ret = p_callback((unsigned char *)mqtt_cfg_buff);
	}
	else
	{
		MercuryTrace("not recv rsp \n");
	}
 	LocalFree(mqtt_cfg_buff);
	return ret;
}


int app_get_update_info(void)
{
	char *sreq = NULL;
	cJSON *root = NULL;
	AppInfoStruct app_info = {0};
	int ret = -1;

	if (0 != NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info))
	{
		MercuryTrace("app_get_update_info, fs_read_deviceinfo fail\n");
		ota_thread_finished = 1;
		return -1;
	}

	root = cJSON_CreateObject();

	//root
	cJSON_AddItemToObject(root, "model", cJSON_CreateString(app_info.model));
	cJSON_AddItemToObject(root, "dev-sn", cJSON_CreateString(app_info.SN));
	cJSON_AddItemToObject(root, "fw-version", cJSON_CreateString(APP_VERSION));
	cJSON_AddItemToObject(root, "fw-build", cJSON_CreateNumber(FW_BUILD_VERSION));

	// sreq = cJSON_Print(root);
	sreq = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);

	memset(app_info.cfg_serv_host, 0x00, sizeof(app_info.cfg_serv_host));
	// strcpy(app_info.cfg_serv_host, "8.210.154.99");
	// app_info.cfg_serv_port = 8083;
	
	// strcpy(app_info.cfg_serv_host, "192.168.2.33");
	// app_info.cfg_serv_port = 10066;

	// strcpy(app_info.cfg_serv_host, "114.143.22.141");
	// app_info.cfg_serv_port = 8083;

	strcpy(app_info.cfg_serv_host, "https://test.mosambee.in");
	app_info.cfg_serv_port = 443;
	// app_info.cfg_serv_port = 8083;

	MercuryTrace("get update host:%s\n", app_info.cfg_serv_host);
	MercuryTrace("get update port:%d\n", app_info.cfg_serv_port);
	MercuryTrace("get update req:%s\n", sreq);
	ret = _app_http_reqest2(app_info.cfg_serv_host, app_info.cfg_serv_port, SERVER_CHECK_UPDATE, sreq, 0, _app_check_update_rsp_parse);

	// ota_thread_finished = 1;
	MercuryTrace("_app_http_reqest2() ret: %d \n", ret);
	LocalFree(sreq);
	return ret;
}


void check_upgrade_thread(void)
{
	app_get_update_info();
}

void aliyun_ota_cb(MessageData* data)
{
	int i, ret = -1;
	cJSON *json_rsp = NULL, *json_data = NULL, *json_ver = NULL, *json_type = NULL;
	cJSON *json_len = NULL, *json_hash = NULL, *json_url = NULL, *json_signmode = NULL;
	DWORD ota_threadid;
	int outlen;
	
	// MercuryTrace("\n info is %s\n",info);
	json_rsp = cJSON_Parse(data->message->payload);
	if (json_rsp == NULL)
	{
		MercuryTrace("ota json fail\n");
		return -1;
	}

	json_data = cJSON_GetObjectItem(json_rsp, "data");

	if (json_data == NULL)
	{
		MercuryTrace("No json data\n");
		goto exit;
	}

	json_ver = cJSON_GetObjectItem(json_data, "version");
	json_type = cJSON_GetObjectItem(json_data, "isDiff");
	json_len = cJSON_GetObjectItem(json_data, "size");
	json_hash = cJSON_GetObjectItem(json_data, "sign");
	json_url = cJSON_GetObjectItem(json_data, "url");
	json_signmode = cJSON_GetObjectItem(json_data, "signMethod");
	
	MercuryTrace("\n json_ver is %s \n",json_ver->valuestring);
	MercuryTrace("\n json_len is %d \n",json_len->valueint);
	MercuryTrace("\n json_hash is %s \n",json_hash->valuestring);
	MercuryTrace("\n json_url is %s \n",json_url->valuestring);


	if (json_ver == NULL || 
		json_signmode == NULL || json_len == NULL ||
		json_hash == NULL || json_url == NULL)
	{
		MercuryTrace("ota json no enough items\n ");
		goto exit;
	}

	memset(&ota_info,0, sizeof(OtaInfoStruct));

	strcpy(ota_info.ota_url, json_url->valuestring); //Get the upgrade URL and save it in OTA_URL
	strcpy(ota_info.New_Version, json_ver->valuestring); //Get the new version number in the New_Version buff
	strcpy(ota_info.check_value, json_hash->valuestring);
	if (json_type)
		ota_info.fota_type = json_type->valueint;
	else
		ota_info.fota_type = 0;
	ota_info.file_len = json_len->valueint;
	if (strcmp(json_signmode->valuestring, "Md5") == 0)
		ota_info.sign_method = 1;
	if (strcmp(json_signmode->valuestring, "SHA256") == 0)
		ota_info.sign_method = 2;

	MercuryTrace("json_hash->valuestring = %s\r\n", json_hash->valuestring);

	//Create a thread for OTA upgrades
	if (NULL == CreateThread(NULL, 60 * 1024, ota_thread, 0, NULL, STACK_SIZE_RESERVATION, &ota_threadid))
	{
		MercuryTrace("%s,%d errno=%lu\n", __func__, __LINE__, GetLastError());
		goto exit;
	}

exit:
	if (json_rsp)
		cJSON_Delete(json_rsp);
}

extern AppMqttCfgStruct l_soundbox_mqtt_cfg;

int msg_id;
int report_update_result_aliyun(void)			//�ϱ��̼��������
{
	cJSON *root = NULL, *json_param = NULL;
	int ret = -1;
	char str_id[16] = {0};
	char ver_topic[128] = {0};
	char *sout = NULL;
	char devicename[64] = {0};
	char *p = NULL;

	root = cJSON_CreateObject();

	sprintf(str_id, "%d", msg_id++);
	cJSON_AddItemToObject(root, "id", cJSON_CreateString(str_id));
	json_param = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "params", json_param);
	
	cJSON_AddItemToObject(json_param, "version", cJSON_CreateString(ota_info.New_Version));

	// sout = cJSON_Print(root);
	sout = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);

	strcpy(devicename, l_soundbox_mqtt_cfg.username);
	p = strchr(devicename, '&');
	if (p)
		*p = 0;

	if (strlen(l_soundbox_mqtt_cfg.productkey))
		// sprintf(ver_topic, "/ota/device/inform/%s/%s", l_mqtt_cfg.productkey, devicename);
		sprintf(ver_topic, "/ota/device/inform/%s/%s", l_soundbox_mqtt_cfg.productkey, devicename);
		
	else
		sprintf(ver_topic, "/ota/device/inform/%s", devicename);
	
	MercuryTrace("\n UPDATE_SUCCESS \n");
	// ret = sdk_MQTT_publish(app_mqtt_get_handle(), ver_topic, 0, sout, strlen(sout));
	ret = sdk_MQTT_publish(app_mqtt_soundbox_get_handle(), ver_topic, 0, sout, strlen(sout));
	
	LocalFree(sout);
	return ret;
}