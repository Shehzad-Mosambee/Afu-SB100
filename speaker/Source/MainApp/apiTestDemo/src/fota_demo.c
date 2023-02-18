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

OtaInfoStruct test_ota_info;

VOID test_ota_thread( ULONG argc, LPVOID lpThreadParameter )
{
	int ret = 0;
	cJSON *root = NULL;
	char *sreq;

	// Setting the Upgrade Mode
	// 4 indicates the differential upgrade. 8 Indicates the full upgrade.
	sdk_set_ota_mode(1, test_ota_info.fota_type);

	// // Please wait while firmware download
    // // app_audio_play(AUDIO_START_UPGRADE);
	// sdk_audio_play_nv(AUDIO_START_UPDATE+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_START_UPDATE));
	play_audio_by_name("sUpd", (unsigned char)0);

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "dev-sn", "cs_1201");
	sreq = cJSON_Print(root);
	cJSON_Delete(root);

	ret = sdk_fota_download_http_ex(test_ota_info.ota_url, sreq, test_ota_info.file_len );
	LocalFree(sreq);
	
	if(ret != 0) 
	{
		MercuryTrace("Error! ret(0x%x) \n",ret);
		// // Failed to download the upgrade file. Procedure
        // // app_audio_play(AUDIO_UPGRADE_DL_FAIL);
		// sdk_audio_play_nv(AUDIO_UPDATE_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_UPDATE_FAILED));
		play_audio_by_name("sUpdF", (unsigned char)0);
	}
	else
	{
		// The upgrade file is successfully downloaded. Procedure
        // app_audio_play(AUDIO_UPGRADE_FILE_DL_SUC);
		// sdk_audio_play_nv(AUDIO_UPDATE_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_UPDATE_SUCCESS));

		// To upgrade
        // app_audio_play(AUDIO_WAIT_UPGRADING);

        if(test_ota_info.fota_type == QYY_FOTA_TYPE_FULL)
		    ret = sdk_fota_update(test_ota_info.check_value, ALGORITHMS_TYPE_SHA256);
        else if(test_ota_info.fota_type == QYY_FOTA_TYPE_DIFF)
            ret = FotaUpgrade_Start();
		
		if(ret != 0)
		{
			MercuryTrace("%s,%d Error! ret(0x%x) \n",__func__,__LINE__, ret);
			// Upgrade failed
            // // app_audio_play(AUDIO_UPDATE_FAIL);
			// sdk_audio_play_nv(AUDIO_UPDATE_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_UPDATE_FAILED));
			play_audio_by_name("sUpdF", (unsigned char)0);
		}	
		else
		{
			// The upgrade succeeded. The device is restarted
            // // app_audio_play(AUDIO_UPDATE_SUC);
			// sdk_audio_play_nv(AUDIO_UPDATE_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_UPDATE_SUCCESS));
			play_audio_by_name("sUpdS", (unsigned char)0);
            
            set_restart_reason(E_RST_REBOOT);
            appInfoSave();

			Sleep(10 * 1000);
			PowerReboot();
		}
	}

	ExitThread(NULL);
}

static int _test_app_check_update_rsp_parse(unsigned char *info)
{
	int  i, ret =-1;
	cJSON *json_rsp = NULL, *json_result = NULL, *json_ver = NULL,*json_build = NULL;
	cJSON *json_len = NULL, *json_hash = NULL, *json_url = NULL,*json_fota_type = NULL;
	DWORD ota_threadid;
	int outlen;
    unsigned char sha256_result[32] = {0};

	json_rsp = cJSON_Parse(info);
	if (json_rsp == NULL)
	{
		MercuryTrace("check update json fail\n");
		return -1;
	}

	json_result = cJSON_GetObjectItem(json_rsp, "error-code");
	json_ver = cJSON_GetObjectItem(json_rsp, "version");
	json_build = cJSON_GetObjectItem(json_rsp, "build");
	json_len = cJSON_GetObjectItem(json_rsp, "file-length");
	json_hash = cJSON_GetObjectItem(json_rsp, "file-hash");
	json_url = cJSON_GetObjectItem(json_rsp, "download-url");
	json_fota_type = cJSON_GetObjectItem(json_rsp, "fota-type");
	

	if (json_result == NULL || json_ver == NULL || json_build == NULL 
		|| json_len == NULL || json_hash == NULL || json_url == NULL
        || json_fota_type == NULL)
	{
		MercuryTrace("update json no enough items\n ");
		ret = ERR_DATA_PARSE_FAIL;
		goto exit;
	}
	if (json_result->valueint != 0)
	{
		MercuryTrace("update json error code:%d\n ", json_result->valueint);
		ret = ERR_DATA_PARSE_FAIL;
		goto exit;
	}	

	if (json_build->valueint < FW_BUILD_VERSION) //The server version is earlier than the device version
	{
		MercuryTrace("no new fw version \n ");
		ret = OTA_NOT_UPDATE;
		goto exit;
	}	

	//The server has a new version
	strcpy(test_ota_info.ota_url,json_url->valuestring);	//Get the upgrade URL and save it in OTA_URL
	strcpy(test_ota_info.New_Version,json_ver->valuestring);   //Get the new version number in the New_Version buff
	test_ota_info.build_no = json_build->valueint;
	test_ota_info.file_len = json_len->valueint;
    test_ota_info.fota_type = json_fota_type->valueint;	//4 indicates the differential upgrade. 8 Indicates the full upgrade.

	memset(test_ota_info.check_value, 0, sizeof(test_ota_info.check_value));
	mbedtls_base64_decode(sha256_result, sizeof(sha256_result), &outlen,  json_hash->valuestring, strlen(json_hash->valuestring));
    app_hex2str(sha256_result,sizeof(sha256_result), test_ota_info.check_value);
	
	//Create a thread for OTA upgrades
	if( NULL == CreateThread(NULL,60*1024,test_ota_thread,0,NULL,STACK_SIZE_RESERVATION,&ota_threadid) )
	{
		MercuryTrace("%s,%d errno=%lu\n",__func__,__LINE__,GetLastError());
		ret = -1;
		goto exit;
	}

	cJSON_Delete(json_rsp);
	return OTA_NEED_UPDATE;
	
exit:
	if (json_rsp)
		cJSON_Delete(json_rsp);

	return ret;
}

int _test_app_http_reqest(char *serv_addr, int port, char *point, char *sendbuff, int type, http_callback p_callback)
{
	// unsigned long handle;
	int ret = 0;
	char mqtt_cfg_buff[3 * 1024];
	int recv_len = 0;
	char url[256] = {0};

	memset(mqtt_cfg_buff, 0, 3 * 1024);
	
    sprintf(url, "http://%s:%d%s", serv_addr, port, point);
    MercuryTrace("url = %s\r\n",url);

	if (type == 0) //Get
	    ret = sdk_http_get(url, sendbuff, NULL, mqtt_cfg_buff, 3 * 1024, &recv_len);
	
    MercuryTrace("sdk_http_get ret = %d\r\n",ret);
	if (ret == 0)
	{
		MercuryTrace("recv len: %d\n%s\n", strlen(mqtt_cfg_buff), mqtt_cfg_buff);
		if (p_callback)
			ret = p_callback((unsigned char*) mqtt_cfg_buff);
	}
	else 
	{
		MercuryTrace("not recv rsp \n");		
	}

	return ret;

}

int test_app_get_update_info(void)
{
	char *sreq = NULL;
	cJSON *root = NULL;
	int ret = -1;

	root = cJSON_CreateObject();
	
	//root
	cJSON_AddItemToObject(root, "dev-sn", cJSON_CreateString("cs_1201"));
	cJSON_AddItemToObject(root, "fw-version", cJSON_CreateString("1.0.0"));
	cJSON_AddItemToObject(root, "fw-build", cJSON_CreateNumber(1));

	sreq = cJSON_Print(root);
	cJSON_Delete(root);
	
	MercuryTrace("get update req:%s\n", sreq);

	ret = _test_app_http_reqest("152.136.213.55", 10501, "/speaker/check-update", sreq, 0, _test_app_check_update_rsp_parse);

	MercuryTrace("_test_app_http_reqest ret: %d \n", ret);
	LocalFree(sreq);
	return ret;
}

void test_upgrade_thread(void)
{
	test_app_get_update_info();
}