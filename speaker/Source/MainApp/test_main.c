#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryThread.h"
#include "MercuryAudio.h"
#include "MercuryNV.h"
#include "MercuryFile.h"
#include "MercurySpi.h"
#include "MercuryBattery.h"
#include "MercuryTts.h"
#include "MercuryNLed.h"
#include "MercuryGpio.h"

#include "SDK_API_SYS.h"
#include "SDK_API_AUDIO.h"
#include "SDK_API_WIFI.h"
#include "SDK_API_POWER.h"
#include "SDK_API_LED.h"
#include "SDK_API_DISPLAY.h"
#include "SDK_API_BCS_DISPLAY.h"

#include "keypad.h"
#include "led.h"
#include "audio.h"
#include "nv.h"
#include "device.h"
// #include "connect_mqtt_server.h"
#include "connect_mqtt_soundbox_server.h"
#include "app_config.h"
#include "speakerapp.h"
#include "app_fota.h"
#include "app_at.h"
#include "vendor.h"

#include "global.h"

BOOL g_status_fake_off = FALSE;
int l_reset_reason = E_RST_UNKNOWN;
HANDLE g_event_ota_update;
extern HANDLE g_event_batlow_poweroff;
extern HANDLE g_event_batlow_warning;
// extern HANDLE g_event_update_qr;

BOOL g_wifi_enable = TRUE; //wifi is supported ?
BOOL g_gprs_enable = TRUE;  //Support 2G or not
BOOL g_wifi_config = FALSE;
extern int cfg_done;

MERCURY_NETWORK_STATUS_T g_pStatus;

extern int app_connect_mqtt(void);

void connect_wifi_network(void)
{
    int ret; 
    unsigned long count = 0;
	BOOL wifi_init_OK;

    play_audio_by_name("connWifi", (unsigned char)0);

    // set network to WIFI mode
    sdk_set_network_mode(NETWORK_MODE_WIFI);

    sdk_network_wifi_init();

    // query whether the WIFI AT instructions are executed successfully (test WIFI)
    for (count = 0; count < 10; count++)
    {
        wifi_init_OK = sdk_get_wifi_init_OK(); 
        if (TRUE == wifi_init_OK)
        {
            //wifi init OK
            MercuryTrace("wifi init OK\r\n");
            start_connect_network();
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
        set_network();
        // // app_audio_play(AUDIO_WIFI_NETWORK_INIT_FAIL);

    }
}

void connect_gprs_network(void)
{
    int ret;
    int network_mode = NETWORK_MODE_GPRS;
    int gprs_network_status;

network_reinit:
	// // app_audio_play(AUDIO_MOBLILE_NETWORK_CONNECTING);
    play_audio_by_name("connSim", (unsigned char)0);

    // set network to GPRS mode
    sdk_set_network_mode(NETWORK_MODE_GPRS);

    // // GPRS initialization    
    // ret = sdk_network_gprs_init();
    // MercuryTrace("network_gprs_init ret = %d\r\n",ret);
    // if( 0 != ret )
    // {
    //     goto network_reinit;
    // }

    
    NetworkGetState(&g_pStatus);
    MercuryTrace("Network Status: %d\r\n", g_pStatus.mnc);
    MercuryTrace("Network Status: %d\r\n", g_pStatus.mcc);
    
    int loop = 0;
    while(apn_list[loop].plmn_details.mnc != 0)
    {
        MercuryTrace("Checking - %d -> mcc1 = %d \t mcc2 = %d\r\n", loop, apn_list[loop].plmn_details.mcc, g_pStatus.mcc);
        MercuryTrace("Checking - %d -> mnc1 = %d \t mnc2 = %d\r\n", loop, apn_list[loop].plmn_details.mnc, g_pStatus.mnc);
        if(apn_list[loop].plmn_details.mnc == g_pStatus.mnc && apn_list[loop].plmn_details.mcc == g_pStatus.mcc)
        {
            MercuryTrace("Setting APN: %s\r\n",apn_list[loop].apn);
            NetworkSetAPN(PDP_ID0, apn_list[loop].apn, NULL, NULL);
            break;
        }

        ++loop;
    }

    // MercuryTrace("NetworkSetAPN - ret = %d\r\n", NetworkSetAPN(PDP_ID0, "mosambee", NULL, NULL));
    // MercuryTrace("NetworkSetAPN - ret = %d\r\n", NetworkSetAPN(PDP_ID0, "IOT.MOSAMBEE.CO.IN", NULL, NULL));

    // GPRS initialization    
    ret = sdk_network_gprs_init();
    MercuryTrace("network_gprs_init ret = %d\r\n",ret);
    if( 0 != ret )
    {
        goto network_reinit;
    }

    unsigned char retry = (unsigned char)10;
    unsigned char onlyOnce = (unsigned char)1;

    while(1)
    {
        gprs_network_status = sdk_get_gprs_network_status();
        MercuryTrace("get_gprs_network_status ret = %d\r\n",gprs_network_status);
        if(GPRS_NETWORK_STATUS_OK == gprs_network_status){
            ret = gprs_network_status;
            break;
        }
        switch(gprs_network_status)
        {
        case GPRS_NETWORK_STATUS_DISCONN:
        case GPRS_NETWORK_STATUS_INITIAL:
            {
                if(retry <= 0)
                {
                    if(onlyOnce)
                    {
                        play_audio_by_name("connSimF", (unsigned char)0);
                        onlyOnce = (unsigned char)0;
                    }
                }
                else         
                    --retry;
                Sleep(1000);
            }
            break;
        case GPRS_NETWORK_STATUS_NEEDRESET:        
			// // app_audio_play(AUDIO_DEVICE_REBOOTING);
            play_audio_by_name("shut", (unsigned char)0);
            sdk_system_restart(NULL);
            break;
        }
    }

    // GPRS attach successfully
    MercuryTrace("GPRS attach successfully\r\n");
	// // app_audio_play(AUDIO_MOBLILE_NETWORK_CONNECTED);
    play_audio_by_name("connSimS", (unsigned char)0);

    set_soundbox_network_success_event();

    NV_Delete(NV_NETWORK_MODE);
    NV_Write(NV_NETWORK_MODE,sizeof(int),&network_mode);

    // ExitThread(NULL);
}

void set_network(void)
{
    int network_mode;
    if(0 == NV_Read(NV_NETWORK_MODE,sizeof(int),&network_mode)){
        if(network_mode == NETWORK_MODE_GPRS){
            // no network
            qyy_bcs_set_rssi(GPRS_ICON,TRUE);
            CreateThread(NULL,4*1024,connect_gprs_network,0,NULL,STACK_SIZE_RESERVATION,NULL);
        }else if(network_mode == NETWORK_MODE_WIFI){
            // no wifi
            qyy_bcs_set_rssi(WIFI_ICON,TRUE);
            CreateThread(NULL,8*1024,connect_wifi_network,0,NULL,STACK_SIZE_RESERVATION,NULL);
        }
    }else{
        // no network
        qyy_bcs_set_rssi(GPRS_ICON,TRUE);
        CreateThread(NULL,4*1024,connect_gprs_network,0,NULL,STACK_SIZE_RESERVATION,NULL);
    }
}

int app_play_power_audio(int poweron)
{
	NvPowerCfgStruct cfg = {0};
	int cfg_index, data_index, default_audio;

	if (poweron)
	{
		cfg_index = NV_POWERON_CFG;
		data_index = NV_POWERON_DATA;
		// default_audio = AUDIO_DEVICE_STARTING_UP;
	}
	else
	{
		cfg_index = NV_POWEROFF_CFG;
		data_index = NV_POWEROFF_DATA;
		// default_audio = AUDIO_DEVICE_SHUTDOWN;
	}
    
	if (0 != NV_Read(cfg_index, sizeof(NvPowerCfgStruct), &cfg))
	{
		// // app_audio_play(default_audio);
		return 0;
	}
	if (cfg.mode == 0) // ����������
		return 0;
	else if (cfg.mode == 1) // ����Ĭ������
	{
		// // app_audio_play(default_audio);
		return 0;
	}
	else
	{
		// TTS��1����AMR��2����PCM��3����MP3��4����
		int len;
		char *data;

		len = NV_GetItemidDataLen(data_index);

		data = LocalAlloc(LMEM_ZEROINIT, len);
		
		if (0 != NV_Read(data_index, len, data))
		{
			// // app_audio_play(default_audio);
			LocalFree(data);
			return 0;
		}
		if (cfg.format == 1)
		{
			// sdk_voiceplay(1,data);
			LocalFree(data);
		}
		else if(cfg.format == 2)
			sdk_audio_play_data(data, len, AUDIO_TYPE_AMR);
		else if(cfg.format == 3)
			sdk_audio_play_data(data, len, AUDIO_TYPE_PCM);
		else if(cfg.format == 4)
			sdk_audio_play_data(data, len, AUDIO_TYPE_MP3);
	}

	return 0;
}
void boot_mode_init(uint32 bootmode)
{
    /* Wait Power Key */
    int ret;
	BOOL sw_fake_off = FALSE;
	BOOL sw_play_welcome = FALSE;
	SYSTEM_POWER_STATUS_EX2 battery_status = {0};

    char debug_buff[256] = {0};

    g_status_fake_off = TRUE;

    ret = BatteryGetStatus(&battery_status,NULL);

	NV_Read(NV_RESET_REASON, sizeof(int), (BYTE*)&l_reset_reason);
	MercuryTrace("reset reason:%d\n", l_reset_reason);
	MercuryTrace("Boot mode : %d \n", bootmode);
	MercuryTrace("USB Status : %d \n", battery_status.ACLineStatus);

	// bootmode & 0x1
	// ==0 : usb
	// !=0 : reset_key or power_key
	//bootmode1,Press the reset button to POWER on or POWER on
	if (1 == bootmode)
	{
        // if(l_reset_reason == E_RST_REBOOT)
        //     sw_fake_off = FALSE;
        // else
        sw_fake_off = FALSE;
		sw_play_welcome = TRUE;
	}
	else
    {			// USB
		switch( l_reset_reason ){
			case E_RST_REBOOT:
		        sw_play_welcome = TRUE;
				sw_fake_off = FALSE;
				set_restart_reason( E_RST_UNKNOWN );
				break;
			case E_RST_APPLY:
			case E_RST_CONFIG: //No longer use
				sw_fake_off = FALSE;
		        sw_play_welcome = FALSE;
				set_restart_reason( E_RST_UNKNOWN );
				break;
			default:
		        sw_play_welcome = TRUE;
				sw_fake_off = TRUE;
				break;
		}
	}


    memset(debug_buff, 0x00, sizeof(debug_buff));
    sprintf(debug_buff, "sw_fake_off : %d----------------------->\r\n", sw_fake_off);
    MercuryDebug(debug_buff, strlen(debug_buff));
    
    memset(debug_buff, 0x00, sizeof(debug_buff));
    sprintf(debug_buff, "sw_play_welcome : %d----------------------->\r\n", sw_play_welcome);
    MercuryDebug(debug_buff, strlen(debug_buff));

    memset(debug_buff, 0x00, sizeof(debug_buff));
    sprintf(debug_buff, "battery_status.ACLineStatus : %d----------------------->\r\n", battery_status.ACLineStatus);
    MercuryDebug(debug_buff, strlen(debug_buff));


	if( sw_fake_off )
    {
#if 1
		//DLFT_ACTION_SYSTEM_POWER_ON_BEFORE();
		if( 0 == ret )
        {
			if( 0 != battery_status.ACLineStatus )  // USB online
            {	
                // led
                led_close();
                // check_usb
                CreateThread(NULL,2*1024,check_usb,0,NULL,STACK_SIZE_RESERVATION,NULL);
			}
		}
#endif

        memset(debug_buff, 0x00, sizeof(debug_buff));
        sprintf(debug_buff, "Going for fake poweroff----------------------->\r\n");
        MercuryDebug(debug_buff, strlen(debug_buff));

		qyy_fake_power_off();//�ٹػ�
	}
	g_status_fake_off = FALSE;

    if (sw_play_welcome)
    {    
        // Welcome to use
        MercuryTrace("Playing PowerOn Audio--------------------------------->\r\n");
        // // app_audio_play(AUDIO_DEVICE_STARTING_UP);
        
        // MOGO
        // play_audio_by_name("mogo", (unsigned char)0);
        
        // WELCOME
        // sdk_audio_play_nv(AUDIO_DEVICE_STARTING_UP+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_DEVICE_STARTING_UP));
        
        #if (ASYNC_AUDIO)
        play_audio_by_name("welc", (unsigned char)0);
        #endif
        MercuryTrace("Welcome to use\r\n");
    }
}

void noework_icon_maintenance_thread(void)
{
	int gprs_network_status;
	BOOL wifi_network_status;
    int network_mode;
    int pingtime = 5 * 1000;

    BOOL network_reconnect = FALSE;
    int wifi_ping_count = 0;

	while (1)
	{
        network_mode = sdk_get_network_mode();
        if (NETWORK_MODE_GPRS == network_mode)
        {
            gprs_network_status = sdk_get_gprs_network_status();
            if(GPRS_NETWORK_STATUS_OK == gprs_network_status)
            {
                if (TRUE == network_reconnect)
                {
                    // network online
                    qyy_bcs_set_rssi(GPRS_ICON,FALSE);
                    set_soundbox_network_success_event();
                    network_reconnect = FALSE;
                }
            }
            else
            {
                // no network
                qyy_bcs_set_rssi(GPRS_ICON,TRUE);
                network_reconnect = TRUE;
            }
        }
        else if (NETWORK_MODE_WIFI == network_mode)
        {
            wifi_network_status = sdk_get_wifi_init_OK(); 
            if (TRUE == wifi_network_status)
            {
                // if (0 == sdk_wifi_ping("114.114.114.114",&pingtime)) 
                if (0 == sdk_wifi_ping("8.8.8.8",&pingtime)) 
                {
                    wifi_ping_count = 0;
                    if (TRUE == network_reconnect)
                    {
                        // wifi online
                        qyy_bcs_set_rssi(WIFI_ICON,FALSE);
                        set_soundbox_network_success_event();
                        network_reconnect = FALSE;
                    }
                }
                else 
                {
                    wifi_ping_count++;
                    if (wifi_ping_count > 3)
                    {
                        // no wifi
                        qyy_bcs_set_rssi(WIFI_ICON,TRUE);
                        network_reconnect = TRUE;
                    }
                }
            }
            else
            {
                // no wifi
                qyy_bcs_set_rssi(WIFI_ICON,TRUE);
                network_reconnect = TRUE;
            }

        }
		Sleep(5000);
	}
}

void battery_icon_thread(void)
{
    uint32 tempPerc = 10;
    uint32 battery_percent;
    SYSTEM_POWER_STATUS_EX2 battery_status = {0};

	while (1)
	{
        memset(&battery_status, 0x00, sizeof(SYSTEM_POWER_STATUS_EX2));
        BatteryGetStatus(&battery_status,NULL);
        
        MercuryTrace(">> From Battery Icon Thread: USB: %d \n", battery_status.ACLineStatus);
        if( 1 == battery_status.ACLineStatus &&  BATTERY_FLAG_CHARGING == battery_status.BatteryFlag )  // usb
        {

            MercuryTrace(">> From Battery Icon Thread: Charging: %d \n", battery_status.ACLineStatus);
            sdk_bcs_set_battery(tempPerc);
            Sleep(2*1000);

            tempPerc+=40;
            if(tempPerc > 100)
                tempPerc = 10;
        }
        else                                                                                            // no usb
        {
            battery_percent = app_get_battery_percent();
            MercuryTrace(">> From Battery Icon Thread: No USB: %d \n", battery_percent);

            sdk_bcs_set_battery(battery_percent);
            Sleep(30*1000);
        }
	}
}




void appInfoInit(void)
{
    int ret = appInfoLoad();

    //ȡ��ǿ�Ƹ�λ
    PmuSet7sReset(FALSE);

    // init volume
    MercuryTrace("Volume Level On Restart: %d\r\n", app_GetVolume());
    AudioSetVolume(app_GetVolume());

    // speakspeed init
    int Speedinti;
    if( 0 != NV_Read(NV_PLAY_SPEED,sizeof(Speedinti), &Speedinti)){
        app_set_tts_param(0);
    }else{
        app_set_tts_param( Speedinti);    //-4500(L1),-2500(L2),0(L3),2500(L4),4500(L5),7000,(L6)
    }
	return 0;
}

void app_set_tts_param( int Speed )
{
	TTS_PARAM_S mode;
	mode.SpeakSpeed = Speed;
	mode.Pitch = 0;
	TTS_SetParams(&mode);
}

            
void Mercury_Main( uint32 bootmode )
{
    HANDLE at_thread_handle = NULL;
    // int mqtt_thread_buffer_size = (64 * 1024);
    // char *mqtt_thread_buffer = malloc(mqtt_thread_buffer_size);

    char debug_buff[512] = {0};

    // System init
    MercuryTrace("System Init----------------------------->\r\n");
	sdk_System_Init();
    // MercurySetTrace(MC_TRACE_TYPE_CP, MC_TRACE_STATE_USB);      //usb port4ץcp log��cp traceĬ���ǹرգ���Ҫ���ú�������,������Ч

    MercuryTrace("Registring short Key presses----------------------------->\r\n");
	sdk_reg_powerkey_shortpress_callback(powerkey_shortpress_callback);
	sdk_reg_poweroff_callback(powerkey_longpress_callback);
	// qyy_reg_tts_complete_callback_event(app_audio_tts_callback);
    sdk_reg_amr_complete_callback(app_audio_amr_callback);
    sdk_reg_mp3_complete_callback(app_audio_mp3_callback);


    // // System init
	// sdk_System_Init();
    // MercurySetTrace(MC_TRACE_TYPE_CP, MC_TRACE_STATE_USB);      //usb port4ץcp log��cp traceĬ���ǹرգ���Ҫ���ú�������,������Ч

    // app info init
    MercuryTrace("App Info Init----------------------------->\r\n");
    appInfoInit();
    // app_set_tts_param( 0);  

   // get mercury version
    MercuryTrace("Get Mercury Version----------------------------->\r\n");
    get_mercury_version();

    /* Timer - Battery Monitor */
    MercuryTrace("Battery Monitor----------------------------->\r\n");
    app_start_power_monitor(NULL);

    // set boot mode
    MercuryTrace("Set Boot Mode - %d----------------------->\r\n", bootmode);
    boot_mode_init(bootmode);

    
    sprintf(debug_buff, "Set Boot Mode - Completed----------------------->\r\n");

    // is vendor mode?
    // MercuryTrace("Vendor Mode----------------------------->\r\n");
    // vendor_main();

    // change vendor lifecycle to app lifecycle?
    // if(LIFECYCLE_VENDOR == get_lifecycle()) // vendor mode
    // {
    //     MercuryTrace("VENDOR Lifecycle----------------------------->\r\n");
    //     // _vendor_at_SN ("1010000180", 10);
        
    //     MercuryTrace("Changing to App Lifecycle----------------------------->\r\n");
    //     set_Lifecycle(LIFECYCLE_APP);
    // }

    // support flash
    app_SetExFlash(TRUE, 4);

    //flash init
    int hwpad = 0, vol = 1800;
    QspiFlashInit(hwpad,vol);
    FILE_SPI_CONFIG_T config;
    config.type = FILESYS_SPI_EXT;
    config.buad = FILESYS_SPI_EXT_BOAD_DEFAULT;
    FileSpi_Config(&config);
	if ( 0 != InitFileSystemPlus(0, 4, FILESYS_CAPACITY_4M))
    {
    	MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxx init file system fail xxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n");
    }

    //ȡ����ȫоƬ����
	appSetGpioOutput(22, 0);
    // support wifi
    app_SetExistWifi(TRUE);

    //BCS init
    // MercuryTrace("BCS Init----------------------------->\r\n");
    // sdk_bcs_init();
    qyy_bcs_init();

    #if (ASYNC_AUDIO == 0)
    play_audio_by_name("welc", (unsigned char)0);
    #endif



    // // --------------

    // #include "wifi.h"
    // int mode = NETWORK_MODE_WIFI;
    // NV_Write(NV_NETWORK_MODE, sizeof(int), &mode);

    // AppWifiInfoStruct wifi_info = {0};

    // strcpy(wifi_info.ssid, "GalaxyA20");
    // strcpy(wifi_info.pwd, "1122334455");
    // if (0 != NV_Write(NV_WIFI_INFO, sizeof(AppWifiInfoStruct), (BYTE *)&wifi_info))
    // {
    //     MercuryTrace("read nv fail\r\n");
    // }


    // // --------------


    // Network
    set_network();

    //�����
    // SetLedIndication(0);
    
    //���ڱ��������
    // NLedSetCurrentLedlevel(LED_INDICATION, 5);
    // Boost���ţ�7��
    
    // display speaker
    // qyy_bcs_set_speaker(TRUE);

    //������̬��ά�����ó�ʼ��--û��flash--SDKӦ��Ҳ���˷���ĳ�ʼ��
    // MercuryTrace("Display Init----------------------------->\r\n");
    // sdk_disp_init(LCD_TYPE_ST7789);
    // DisplaySetDirection(DMDO_270);
    // app_display_statusbar();
    // DisplaySetBrightness(14);       //��������


    // Keypad init
    MercuryTrace("Keypad Init----------------------------->\r\n");
    app_keypad_init();

    // Audio Enable - Shehzad
    sdk_PA_ENABLE(TRUE);

    #if defined(AMR_AUDIO)
    app_SetAudioType(AUDIO_TYPE_AMR);
    #else
    app_SetAudioType(AUDIO_TYPE_MP3);
    #endif

    char aud_ver[8] = {0};
    if(0 != NV_Read(NV_AUDIO_VERSION, 8, aud_ver))
    {
        MercuryTrace("NV Read NV_AUDIO_VERSION error: %x\r\n", GetLastError());
        NV_Delete(NV_AUDIO_VERSION);
        if(0 != NV_Write(NV_AUDIO_VERSION, 8, MOS_AUDIO_VERSION))
        {
            MercuryTrace("NV Write NV_AUDIO_VERSION error: %x\r\n", GetLastError());
        }
    }
    else
    {
        NV_Read(NV_AUDIO_VERSION, 8, aud_ver);
        MercuryTrace("Current Audio Version: %s\r\n", aud_ver);
    }
	
    // Correct the Model Name of Rupee Symbol device
    MercuryTrace(">> sdk_bcs_flag() ret: %d\r\n", sdk_bcs_flag());
    if(sdk_bcs_flag())
    {
        AppInfoStruct app_info = {0};
        NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);

        if(strcmp(app_info.model, "SB100"))
        {
            MercuryTrace("\r\n>> Writing New Model-Name\r\n");
            strcpy(app_info.model, "SB100");
            NV_Write(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&app_info);
        }

    }



	// ���ؽ��׼�¼
	app_load_trans_record();
	
    // // Network
    // set_network();

    // AT Thread    // ���߳����Ż�  20mA
    // at_thread_handle = CreateThread(NULL, 4 * 1024, app_at_thread, 0, NULL, STACK_SIZE_RESERVATION, NULL);
    // VirCom_SetCb(at_thread_handle, PORT_ID, TRUE);

    // connect server
    // CreateThreadWithStack(NULL,mqtt_thread_buffer,mqtt_thread_buffer_size,mqtt_thread,0,"Test Thread",STACK_SIZE_RESERVATION,NULL);
    // CreateThreadWithStack(NULL,mqtt_thread_buffer,mqtt_thread_buffer_size, mqtt_soundbox_thread,0,"MqTeStThReAd",STACK_SIZE_RESERVATION,NULL);

    // network and icon maintenance thread
	CreateThread(NULL, 4*1024, noework_icon_maintenance_thread, 0, NULL, STACK_SIZE_RESERVATION, NULL);
    
    // battery icon thread
    MercuryTrace(">> sdk_bcs_flag() ret: %d\r\n", sdk_bcs_flag());
    if(sdk_bcs_flag())
        CreateThread(NULL, 4*1024, battery_icon_thread, 0, NULL, STACK_SIZE_RESERVATION, NULL);
    
    // g_event_ota_update = CreateEvent(NULL, TRUE, FALSE, "OTA-UPDATE");
	// //// g_event_update_qr = CreateEvent(NULL,TRUE,FALSE, "G_EVENT_UPDATE_QR");
    // SetEvent(g_event_ota_update);

    // Sleep(3000);

    /*
    int retry = 3;
    while(retry > 0)
    {
        if (!is_soundbox_network_connected())
        {
            MercuryTrace("------------------ Not Connected To Internet ------------------\r\n");
            Sleep(2000);
            continue;
        }

        int ret = Trans_RegisterDevice_Process();
        if(SUCCESS == ret)
            break;

        Sleep(3000);
        --retry;
    }
    */


    // // Check for updates:
    // {
        

    //     MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Checking for application updates\r\n");
    //     app_get_update_info();

        
    //     MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Waiting for application update to finish\r\n");
    //     // MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx ota_thread_finished : %d\r\n", ota_thread_finished);
    //     // while(ota_thread_finished == (unsigned char)0);    // wait until app update is not finished
        
    //     // while(!ota_thread_finished);    // wait until app update is not finished
    //     while(!ota_thread_finished)    // wait until app update is not finished
    //     {
    //         Sleep(1000);
    //     }

    //     MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Application update finished\r\n");
    //     MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Checking for audio updates\r\n");
        
    //     char ver[8] = {0};
    //     NV_Read(NV_AUDIO_VERSION, 8, ver);
    //     MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Current audio version: %s\r\n", ver);
    //     app_audio_check_update();
    //     ResetEvent(g_event_ota_update);
    // }



    int retry = 3;
    while(retry > 0)
    {
        int ret = Trans_RegisterDevice_Process();
        if(SUCCESS == ret)
            break;
        MercuryTrace("----------------------------------------------\r\n");

        Sleep(3000);
        --retry;
    }


    if(!retry)
    {
        play_audio_by_name("setupf", (unsigned char)0);
        sdk_light_control(LED_RED);
        
        // sdk_audio_play_nv(AUDIO_SHUTTING_DOWN+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SHUTTING_DOWN));
        // play_audio_by_name("shut", (unsigned char)0);

        // Sleep(5000);
        // set_restart_reason( E_RST_UNKNOWN );
        // PowerOff();
        // PowerReboot();
    }

    Sleep(5000);
    // app_get_update_info();
    
    // Check if Application Update is available
    g_event_ota_update = CreateEvent(NULL, TRUE, FALSE, "OTA-UPDATE");
    SetEvent(g_event_ota_update);
    
    while (1)
    {
    	if (-1 != WaitForSingleObject(g_event_batlow_poweroff, 0))
		{
			MercuryTrace("get g_event_batlow_poweroff\r\n");
			batlow_poweroff();
		}
		
        if (-1 != WaitForSingleObject(g_event_batlow_warning, 0))
		{
			MercuryTrace("get g_event_batlow_warning\r\n");
			
			batlow_warning();
			ResetEvent(g_event_batlow_warning);
		}
        
        if (0 == WaitForSingleObject(g_event_ota_update, 0))
		{
            MercuryTrace("get g_event_ota_update\r\n");

            MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Checking for application updates\r\n");
            app_get_update_info();

            
            MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Waiting for application update to finish\r\n");
            // MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx ota_thread_finished : %d\r\n", ota_thread_finished);
            // while(ota_thread_finished == (unsigned char)0);    // wait until app update is not finished
            
            // while(!ota_thread_finished);    // wait until app update is not finished
            while(!ota_thread_finished)    // wait until app update is not finished
            {
                Sleep(1000);
            }

            MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Application update finished\r\n");
            MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Checking for audio updates\r\n");
            
            char ver[8] = {0};
            NV_Read(NV_AUDIO_VERSION, 8, ver);
            MercuryTrace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx Current audio version: %s\r\n", ver);
            app_audio_check_update();
			ResetEvent(g_event_ota_update);
		}

		Sleep(2000);
        MercuryTrace("Test Main - run app...\r\n");
    }
}
