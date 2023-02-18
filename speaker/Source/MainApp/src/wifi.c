#include "MercuryDef.h"
#include "MercuryThread.h"
#include "MercuryNv.h"

#include "SDK_API_WIFI.h"
#include "SDK_API_SYS.h"

#include "speakerapp.h"
#include "audio.h"
#include "wifi.h"
#include "nv.h"
// #include "connect_mqtt_server.h"
#include "global.h"

extern void connect_gprs_network(void);

int l_wifi_flag = 0;

//wifi connect Callback
void connectCallback( unsigned char *ssid, unsigned char *pwd, int state )
{
    int network_mode = NETWORK_MODE_WIFI;
    AppWifiInfoStruct wifi_info = {0};

	MercuryTrace("ssid = %s\r\n",ssid);
	MercuryTrace("pwd = %s\r\n",pwd);
	MercuryTrace("state = %d\r\n",state);

	if (strlen((char*)ssid))
	{
		if(state){
            NV_Delete(NV_WIFI_INFO);
            NV_Delete(NV_NETWORK_MODE);

			strcpy(wifi_info.pwd, (char*)pwd);
			strcpy(wifi_info.ssid, (char*)ssid);
			NV_Write(NV_WIFI_INFO, sizeof(AppWifiInfoStruct), (BYTE*)&wifi_info);
            NV_Write(NV_NETWORK_MODE,sizeof(int),&network_mode);
		}
		MercuryTrace("wifi config ok \n");
    }
}

void start_connect_network(void)
{
    int ret;
    AppWifiInfoStruct wifi_info = {0};

	//// app_audio_play(AUDIO_WIFI_NETWORK_CHECK_COMPLETED);
    if (0 != NV_Read(NV_WIFI_INFO, sizeof(AppWifiInfoStruct),(BYTE*)&wifi_info)){
        MercuryTrace("read nv fail\r\n");
    }

    MercuryTrace("read nv ssid = %s pwd = %s\r\n",wifi_info.ssid,wifi_info.pwd);

    ret = sdk_start_connect(wifi_info.ssid, wifi_info.pwd, NULL, 30);
	
    if(0 == ret)
    {
        MercuryTrace("ap mode config wifi success\r\n");
        
        //// app_audio_play(AUDIO_CONFIG_COMPLETE);
        //// app_audio_play(AUDIO_WIFI_NETWORK_CONNETC);
        
        // // app_audio_play(AUDIO_WIFI_CONNECT_SUCCESS);
        // sdk_audio_play_nv(AUDIO_WIFI_CONNECT_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_CONNECT_SUCCESS));
        play_audio_by_name("wifiConnS", (unsigned char)0);
        
        Sleep(2000);
        set_soundbox_network_success_event();
    }
    else
    {
        MercuryTrace("ap mode config wifi fail\r\n");
        // // app_audio_play(AUDIO_NETWORK_CONFIG_FAIL);
        
        
        // // sdk_audio_play_nv(AUDIO_WIFI_CONNECT_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_CONNECT_FAILED));
        // sdk_audio_play_nv(AUDIO_WIFI_CONFIG_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_CONFIG_FAILED));
        play_audio_by_name("wifiConfigF", (unsigned char)0);

        Sleep(2000);
        CreateThread(NULL,4*1024,connect_gprs_network,0,NULL,STACK_SIZE_RESERVATION,NULL);
    }
}

void ap_connect_network(void)
{
    int ret; 
	AppInfoStruct app_info = {0};
	char ssid[100] = {0};
	int len = 0;
	
	NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);

    l_wifi_flag = 1;

    
    MercuryTrace("%d, %s, Playing WiFi Mode AP--------\r\n", __LINE__, __func__);
    // // app_audio_play(AUDIO_WIFI_APCFG);
    // sdk_audio_play_nv(AUDIO_WIFI_AP_MODE+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_AP_MODE));
    play_audio_by_name("wifiAPMode", (unsigned char)0);

    Sleep(2000);
    
	len = strlen(app_info.SN);
	if (len>=8)
		sprintf(ssid, "SZAF%s", &app_info.SN[len-7]);
	else
		sprintf(ssid, "%s", app_info.SN);
	
    ret = sdk_start_AP(connectCallback,ssid,120,NULL);//WiFi_AP_SSID
    
    if(0 == ret)
    {
        MercuryTrace("ap mode config wifi success\r\n");
        //// app_audio_play(AUDIO_CONFIG_COMPLETE);
        //// app_audio_play(AUDIO_WIFI_NETWORK_CONNETC);
		// app_audio_play(AUDIO_WIFI_CONNECT_SUCCESS);
        
        MercuryTrace("%d, %s, Playing WiFi Connect Success--------\r\n", __LINE__, __func__);
        // sdk_audio_play_nv(AUDIO_WIFI_CONNECT_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_CONNECT_SUCCESS));
        play_audio_by_name("wifiConnS", (unsigned char)0);
        Sleep(2000);

        l_wifi_flag = 0;
		set_soundbox_network_success_event();
    }
    else
    {
        MercuryTrace("ap mode config wifi fail\r\n");
        
        MercuryTrace("%d, %s, Playing WiFi Config Failed--------\r\n", __LINE__, __func__);
		// // app_audio_play(AUDIO_NETWORK_CONFIG_FAIL);
        // sdk_audio_play_nv(AUDIO_WIFI_CONFIG_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_CONFIG_FAILED));
        play_audio_by_name("wifiConfigF", (unsigned char)0);
        Sleep(2000);
        
        l_wifi_flag = 0;
        set_network();
    }
}

void airkiss_connect_network(void)
{
    int ret; 

    l_wifi_flag = 1;

    // // app_audio_play(AUDIO_WIFI_AKCFG);
    
    // sdk_audio_play_nv(AUDIO_WIFI_AK_MODE+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_AK_MODE));
    play_audio_by_name("wifiAKMode", (unsigned char)0);
    Sleep(2000);

    ret = sdk_start_airkiss(connectCallback,120);
    
    if(0 == ret)
    {
        MercuryTrace("airkiss mode config wifi success\r\n");
        //// app_audio_play(AUDIO_CONFIG_COMPLETE);
        //// app_audio_play(AUDIO_WIFI_NETWORK_CONNETC);
        
        // // app_audio_play(AUDIO_WIFI_CONNECT_SUCCESS);
        // sdk_audio_play_nv(AUDIO_WIFI_CONNECT_SUCCESS+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_CONNECT_SUCCESS));
        play_audio_by_name("wifiConnS", (unsigned char)0);
        Sleep(2000);

        l_wifi_flag = 0;
		set_soundbox_network_success_event();
    }
    else
    {
        MercuryTrace("airkiss mode config wifi fail\r\n");
        // // app_audio_play(AUDIO_NETWORK_CONFIG_FAIL);
        // sdk_audio_play_nv(AUDIO_WIFI_CONFIG_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_CONFIG_FAILED));
        play_audio_by_name("wifiConfigF", (unsigned char)0);
        Sleep(2000);
        
        l_wifi_flag = 0;
        set_network();
    }
}

void ap_network_thread(void)
{
    ap_connect_network();
    ExitThread(NULL);
}

void airkiss_network_thread(void)
{
    airkiss_connect_network();
    ExitThread(NULL);
}

void ap_and_airkiss_wifi_connect(int wifiType)
{
    int ret; 
    unsigned long count = 0;
	BOOL wifi_init_OK;

    MercuryTrace("ap_and_airkiss_wifi_connect l_wifi_flag = [%d]\r\n",l_wifi_flag);

    if (l_wifi_flag)
    {
        if(wifiType ==WIFI_AIRKISS){
            sdk_abort_airkiss();
        }
        else if(wifiType ==WIFI_AP){
            sdk_abort_AP();
        }
    }
    else
    {
        // set network to WIFI mode
        sdk_set_network_mode(NETWORK_MODE_WIFI);

        sdk_network_wifi_init();

        //// app_audio_play(AUDIO_WIFI_NETWORK_INIT);
        
        // sdk_audio_play_nv(AUDIO_CONNECTING_WIFI+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CONNECTING_WIFI));
        play_audio_by_name("connWifi", (unsigned char)0);
        Sleep(2000);
		
        // query whether the WIFI AT instructions are executed successfully (test WIFI)
        for (count = 0; count < 10; count++)
        {
            wifi_init_OK = sdk_get_wifi_init_OK();
            if (TRUE == wifi_init_OK) 
            {
                //wifi init OK
                MercuryTrace("wifi init OK\r\n");
                if(wifiType == WIFI_AP)
                    CreateThread(NULL,8*1024,ap_network_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);
                else if(wifiType == WIFI_AIRKISS)
                    CreateThread(NULL,4*1024,airkiss_network_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);
                break;
            }
            else
            {
                Sleep(1000);
            }
        }

        if( (10 == count) && (FALSE == wifi_init_OK) )
        {
            //wifi init fail
            MercuryTrace("wifi init fail\r\n");
            // // app_audio_play(AUDIO_WIFI_NETWORK_INIT_FAIL);
            // sdk_audio_play_nv(AUDIO_WIFI_CONNECT_FAILED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_CONNECT_FAILED));
            play_audio_by_name("wifiConnF", (unsigned char)0);
            Sleep(2000);
            
            set_network();
        }
    }
}
