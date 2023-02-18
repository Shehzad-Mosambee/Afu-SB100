#include "MercuryDef.h"
#include "MercuryPlat.h"

#include "SDK_API_SYS.h"

#include <string.h>

#include "nv.h"
#include "speakerapp.h"
#include "cJSON.h"
#include "global.h"


RESPONSE_DETAILS ResponseDetails;
MQTT_TXN_DETAILS mqtt_txn_details;
REGISTER_DEVICE	 RegisterDevice;


int calculate_CRC_16(char * data)
{
    int crc = 0xFFFF;           // initial crc value
    int polynomial = 0x1021;    // 0001 0000 0010 0001  (0, 5, 12)

    int loop;
    int i;
    for(loop = 0; loop < strlen(data); ++loop)
    {
	for (i = 0; i < 8; i++)
	{
	    unsigned char bit = ((data[loop]   >> (7-i) & 1) == 1);
	    unsigned char c15 = ((crc >> 15    & 1) == 1);
	    crc <<= 1;
	    if (c15 ^ bit) crc ^= polynomial;
	}
    }

    crc &= 0xffff;
    return crc;
}

int parseTxnDetails_new(const char * txn_payload)
{
    if(txn_payload == NULL)
	    return ERROR;

    int retVal = SUCCESS;
    cJSON *result = 0;
    cJSON *data = 0;
    cJSON *notifType = 0;
    cJSON *json = cJSON_Parse(txn_payload);
    if (json == 0)
    {
        MercuryTrace("NULL Parse at ROOT..\r\n");
        return ERROR;
    }

    memset(&mqtt_txn_details, 0x00, sizeof(MQTT_TXN_DETAILS));

    //[1]
    notifType = 0;
    notifType = cJSON_GetObjectItem(json,"notificationType");
    if(notifType != 0)
    {
	    MercuryTrace("[Tag] notificationType = [Value] %s\r\n",notifType->valuestring);

        if(!strcmp(notifType->valuestring,"transaction"))
            mqtt_txn_details.notifType = '0';
        else if(!strcmp(notifType->valuestring,"confirmationCode"))
            mqtt_txn_details.notifType = '1';
        else if(!strcmp(notifType->valuestring,"confirmation"))
            mqtt_txn_details.notifType = '2';
        else if(!strcmp(notifType->valuestring,"testTransaction"))
            mqtt_txn_details.notifType = '3';
        else if(!strcmp(notifType->valuestring,"powerOff"))
            mqtt_txn_details.notifType = '4';
        else if(!strcmp(notifType->valuestring,"restart"))
            mqtt_txn_details.notifType = '5';
    }
    else
    {
        MercuryTrace("NULL Parse at notificationType..\r\n");

        cJSON_Delete(json);
        return ERROR;
    }

    //[2]
    data = 0;
    data = cJSON_GetObjectItem(json,"data");
    if(data == 0)
    {
        MercuryTrace("NULL Parse at data..\r\n");

        cJSON_Delete(json);
    	return ERROR;
    }

    
    //[3]
    if(mqtt_txn_details.notifType == '0' || mqtt_txn_details.notifType == '3') // transaction OR testTransaction
    {
        result = 0;
        result = cJSON_GetObjectItem(data,"transactionAmount");
        if(result != 0)
        {

            MercuryTrace("[Tag] transactionAmount = [Value] %s\r\n",result->valuestring);
            memcpy(mqtt_txn_details.amt, result->valuestring, strlen(result->valuestring));
	    }
	    else
	    {
            cJSON_Delete(json);
    	    return ERROR;
	    }
    }
    else if(mqtt_txn_details.notifType == '1')
    {
        result = 0;
        result = cJSON_GetObjectItem(data,"text");
        if(result != 0)
        {
            MercuryTrace("[Tag] otp = [Value] %s\r\n",result->valuestring);
            strncpy(mqtt_txn_details.otp, result->valuestring, 12);
        }
        else
        {
            cJSON_Delete(json);
            return ERROR;
        }
    }


    //[4]
    if(mqtt_txn_details.notifType != '2' && mqtt_txn_details.notifType != '4')
    {
        result = 0;
        result = cJSON_GetObjectItem(data,"lang");
        if(result != 0)
        {
            MercuryTrace("[Tag] lang = [Value] %s\r\n",result->valuestring);
            strncpy(mqtt_txn_details.ln, result->valuestring, 2);
        }
        else
        {
            MercuryTrace("Default [Tag] lang = [Value] en\r\n");
            strncpy(mqtt_txn_details.ln, "en", 2);
        }
    }

    cJSON_Delete(json);
    return SUCCESS;
}

int parseRegisterDeviceResponse(const char * payload)
{
    int loop;
    int retVal = SUCCESS;
    cJSON *result = 0;
    cJSON *notifDetails = 0;
    cJSON *json = cJSON_Parse(payload);
    if (json == 0)
    {
        MercuryTrace("Register-Device - NULL Parse at ROOT..\n");
        // return ERROR;
        return ERROR;
    }

    memset(&ResponseDetails, 0x00, sizeof(RESPONSE_DETAILS));
    memcpy(ResponseDetails.result, "err", strlen("err"));

    //[1] Get "result"
    result = cJSON_GetObjectItem(json,"result");
    if (result!=0)
    {
        MercuryTrace("Register-Device Root Success!\n");
        MercuryTrace("[Tag] result = [Value] %s\n",result->valuestring);
        if(result->type!=cJSON_NULL && result->valuestring!=NULL && result->valuestring[0]!=0 && strcmp(result->valuestring, "NA"))
            memcpy(ResponseDetails.result, result->valuestring, strlen(result->valuestring));
    }
    else
    {
        cJSON_Delete(json);
        return ERROR;
    }

    MercuryTrace("result = %s\n",ResponseDetails.result);

    //[3] if "result" is "success" then get other tags
    if(strcmp(ResponseDetails.result, "failed") == 0)
    {
        // retVal = ERROR;
        // goto CLEANUP;

        cJSON_Delete(json);
        return ERROR;
    }


    memset(&RegisterDevice, 0x00, sizeof(REGISTER_DEVICE));

    notifDetails = 0;
    notifDetails = cJSON_GetObjectItem(json,"notificationDetails");
    if(notifDetails != 0)
    {
	    result = 0;
        result = cJSON_GetObjectItem(notifDetails,"mqttQos");
        if(result != 0)
        {
            MercuryTrace("[Tag] mqttQos = [Value] %s\n",result->valuestring);
            if(result->type!=cJSON_NULL && result->valuestring!=NULL && result->valuestring[0]!=0 && strcmp(result->valuestring, "NA"))
            {
                int qos = 0;
                sscanf(result->valuestring, "%d", &qos);
                // RegisterDevice.QOS = (unsigned char)qos;
                RegisterDevice.QOS = qos;
            }
            else
            {
                // RegisterDevice.QOS = (unsigned char)2;
                RegisterDevice.QOS = 1;
            }
        }

	    /*
        result = 0;
        result = cJSON_GetObjectItem(notifDetails,"mqttRetainFlag");
        if(result != 0)
        {
            MercuryTrace("[Tag] mqttRetainFlag = [Value] %s\n",result->valuestring);
            if(result->type!=cJSON_NULL && result->valuestring!=NULL && result->valuestring[0]!=0 && strcmp(result->valuestring, "NA"))
            {
                // int retainFlag = 0;
                // sscanf(result->valuestring, "%d", &retainFlag);
                RegisterDevice.retainFlag =  (strcmp(result->valuestring,"false")==0)?(unsigned char)0:(unsigned char)1;
            }
            else
            {
                RegisterDevice.retainFlag = (unsigned char)0;
            }
        }
	    */

        result = 0;
        result = cJSON_GetObjectItem(notifDetails,"mqttCleanSession");
        if(result != 0)
        {
            MercuryTrace("[Tag] mqttCleanSession = [Value] %s\n",result->valuestring);
            if(result->type!=cJSON_NULL && result->valuestring!=NULL && result->valuestring[0]!=0 && strcmp(result->valuestring, "NA"))
            {
                // RegisterDevice.cleanSession =  (strcmp(result->valuestring,"true")==0)?(unsigned char)1:(unsigned char)0;

                int cleanSession = 0;
                sscanf(result->valuestring, "%d", &cleanSession);
                // RegisterDevice.cleanSession = (unsigned char)cleanSession;
                RegisterDevice.cleanSession = cleanSession;
            }
            else
            {
                // RegisterDevice.cleanSession = (unsigned char)1;
                RegisterDevice.cleanSession = 1;
            }
        }

        result = 0;
        result = cJSON_GetObjectItem(notifDetails,"mqttKeepAlive");
        if(result != 0)
        {
            MercuryTrace("[Tag] mqttKeepAlive = [Value] %s\n",result->valuestring);
            if(result->type!=cJSON_NULL && result->valuestring!=NULL && result->valuestring[0]!=0 && strcmp(result->valuestring, "NA"))
            {
                // RegisterDevice.cleanSession =  (strcmp(result->valuestring,"true")==0)?(unsigned char)1:(unsigned char)0;

                int keepAlive = 0;
                sscanf(result->valuestring, "%d", &keepAlive);
                // RegisterDevice.cleanSession = (unsigned char)cleanSession;
                RegisterDevice.keepAlive = keepAlive;
            }
            else
            {
                // RegisterDevice.cleanSession = (unsigned char)1;
                RegisterDevice.keepAlive = 60;
            }
        }

        result = 0;
        result = cJSON_GetObjectItem(notifDetails,"mqttUserName");
        if(result != 0)
        {
            MercuryTrace("[Tag] mqttUserName = [Value] %s\n",result->valuestring);
            if(result->type!=cJSON_NULL && result->valuestring!=NULL && result->valuestring[0]!=0 && strcmp(result->valuestring, "NA"))
                memcpy(RegisterDevice.userName, result->valuestring, strlen(result->valuestring));
        }

        result = 0;
        result = cJSON_GetObjectItem(notifDetails,"mqttClientId");
        if(result != 0)
        {
            MercuryTrace("[Tag] mqttClientId = [Value] %s\n",result->valuestring);
            if(result->type!=cJSON_NULL && result->valuestring!=NULL && result->valuestring[0]!=0 && strcmp(result->valuestring, "NA"))
                memcpy(RegisterDevice.clientID, result->valuestring, strlen(result->valuestring));
        }

        result = 0;
        result = cJSON_GetObjectItem(notifDetails,"mqttSubscribeTopic");
        if(result != 0)
        {
            MercuryTrace("[Tag] mqttSubscribeTopic = [Value] %s\n",result->valuestring);
            if(result->type!=cJSON_NULL && result->valuestring!=NULL && result->valuestring[0]!=0 && strcmp(result->valuestring, "NA"))
                memcpy(RegisterDevice.subscriptionTopic, result->valuestring, strlen(result->valuestring));
        }
    }
    else
    {
    	cJSON_Delete(json);
	    return ERROR;
    }
    
    cJSON_Delete(json);
    return SUCCESS; 
}


char * createJsonForDeviceRegistration(void)
{
    cJSON *root = 0;
    cJSON *location = 0;
    cJSON *cgiDetail = 0;

    char lac[10+1] = {0};
    char cellId[10+1] = {0};

    root = cJSON_CreateObject();
    location = cJSON_CreateObject();
    cgiDetail = cJSON_CreateObject();

    //[1]
    char snData[32+1] = {0};
    char hash[32+1]   = {0};

    AppInfoStruct app_info = {0};
    SIM_IMSI_T imsi = {0};
    char tempBuf[128+1] = {0};

	NV_Read(NV_SN, 32, snData);
    NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);
    GetSimInfo(SIM_IMSI_E, &imsi, NULL);

    // if(app_info.model[0]==0)
    //     strncpy(app_info.model, "QF100", 5);

    cJSON_AddStringToObject(root, "deviceSerialNo", snData);
    cJSON_AddStringToObject(root, "deviceModel", app_info.model);
    cJSON_AddStringToObject(root, "deviceMake", "Anfu");
    cJSON_AddStringToObject(root, "devicePlatform", "Linux");
    cJSON_AddStringToObject(root, "deviceAppVersion", APP_VERSION);
    // cJSON_AddStringToObject(root, "deviceToken", "");
    // cJSON_AddStringToObject(root, "deviceCategory", "2");
    // cJSON_AddStringToObject(root, "deviceRegistrationMode", "0");
    // cJSON_AddNumberToObject(root, "serverIdentifier", (int)0);
    // cJSON_AddNumberToObject(root, "serverApiVersion", (int)1);
    
    
    // #if defined(DEVELOPMENT_BUILD)
    // cJSON_AddStringToObject(root, "partnerCode", PARTNER_CODE_2);
    // #elif defined(PRODUCTION_BUILD)
    // cJSON_AddStringToObject(root, "partnerCode", PARTNER_CODE_1);
    // #endif
    cJSON_AddStringToObject(root, "partnerCode", PARTNER_CODE);
    
    // #if defined(DEVELOPMENT_BUILD)
    // sprintf(tempBuf, "%s%s%s%s%s",snData, DEVICE_CATEGORY, DEVICE_REG_MODE, PARTNER_CODE_2, KEY);
    // sprintf(tempBuf, "%s%s%s%s%s",snData, DEVICE_CATEGORY, app_info.model, PARTNER_CODE_2, KEY);
    // #elif defined(PRODUCTION_BUILD)
    // sprintf(tempBuf, "%s%s%s%s%s",snData, DEVICE_CATEGORY, app_info.model, PARTNER_CODE_1, KEY);
    // #endif

    // if(app_info.model[0]==0)
    //     strncpy(app_info.model, "QF100", 5);
    sprintf(tempBuf, "%s%s%s%s%s",snData, DEVICE_CATEGORY, app_info.model, PARTNER_CODE, KEY);

    //sprintf(hash, "%04X", calculate_CRC_16(tempBuf));
    sprintf(hash, "%X", calculate_CRC_16(tempBuf));
    cJSON_AddStringToObject(root, "hash", hash);

    // cJSON_AddStringToObject(root, "deviceIdentifier", SystemParameters.mercDetails.userName);
    // cJSON_AddStringToObject(root, "deviceCategory", "0");

    int ret_mode= sdk_get_network_mode();
    if (ret_mode == NETWORK_MODE_GPRS)
    {
        sprintf(lac, "%d", g_pStatus.lac);
        sprintf(cellId, "%d", g_pStatus.cell_id);
        cJSON_AddStringToObject(cgiDetail, "lac", lac);
        cJSON_AddStringToObject(cgiDetail, "cid", cellId);
        cJSON_AddStringToObject(cgiDetail, "imsi", imsi.imsi_val);
        
        // char * dev_imei = GetImei(); // No neeed to free the output
        // cJSON_AddStringToObject(cgiDetail, "imei", dev_imei);
        
        cJSON_AddStringToObject(location, "detailCategory", "1");
        cJSON_AddItemToObject(location, "cgiDetail", cgiDetail);
        cJSON_AddItemToObject(root, "locationDetail", location);
    }

    char * out = cJSON_PrintUnformatted(root);
    // MercuryTrace("add-device json = %s\r\n", out);

    char * final_out = LocalAlloc(LMEM_ZEROINIT, strlen(out)+1 );
    memset(final_out, 0x00, strlen(out)+1);
    memcpy(final_out, out, strlen(out));
    free(out);

    cJSON_Delete(root);

    return final_out;
}


