#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryNet.h"
#include "MercuryCSTD.h"
#include "MercuryKeypad.h"
#include "MercuryThread.h"
#include "MercuryTimer.h"
#include "MercuryBattery.h"
#include "MercuryNv.h"
#include "MercuryFile.h"
#include "MercuryMem.h"

#include "SDK_API_AUDIO.h"
#include "SDK_API_POWER.h"
#include "SDK_API_WIFI.h"
#include "SDK_API_SYS.h"
#include "SDK_API_KEYPAD.h"

#include "keypad.h"
#include "audio.h"
#include "wifi.h"
#include "device.h"
#include "NV.h"
#include "app_config.h"
#include "speakerapp.h"

#include "global.h"

extern int g_pay_amount;
HANDLE keypad_thread_handle;

const char amr_didi[]={
  0x23, 0x21, 0x41, 0x4d, 0x52, 0x0a, 0x3c, 0x55, 0x12, 0x79, 0xbc, 0x4e, 0x79, 0xe1, 0xc4, 0x21
, 0x67, 0x2a, 0xf2, 0x00, 0x31, 0x02, 0xc0, 0x00, 0x00, 0x00, 0x45, 0xbd, 0xbf, 0x84, 0x00, 0x00
, 0x00, 0x0c, 0xc4, 0xfc, 0x87, 0x40, 0x3c, 0x11, 0x6b, 0x9a, 0x18, 0x08, 0x2e, 0x02, 0x8c, 0xfc
, 0x6d, 0xb5, 0x8e, 0x0e, 0x91, 0x7a, 0x7e, 0xd7, 0x93, 0xff, 0xe0, 0x32, 0x01, 0x85, 0x01, 0x3f
, 0xf6, 0xf0, 0x06, 0x82, 0x6b, 0x70, 0x3c, 0x38, 0x51, 0xc4, 0x88, 0x04, 0x10, 0x53, 0xe7, 0x9a
, 0x10, 0x5a, 0x89, 0x53, 0x6a, 0x9a, 0xc2, 0xbe, 0xdc, 0xa4, 0xe8, 0x0f, 0xfc, 0xac, 0x29, 0x26
, 0x6b, 0xb4, 0x4f, 0xbf, 0x6e, 0x60, 0x3c, 0x38, 0x0e, 0x42, 0x68, 0x49, 0xcc, 0x25, 0xfe, 0x00
, 0x74, 0x6a, 0x8b, 0x07, 0xc5, 0xdf, 0x3d, 0xe3, 0x83, 0xd9, 0xdd, 0x2a, 0x25, 0x2d, 0x5c, 0x28
, 0xd0, 0xfb, 0x83, 0x23, 0xde, 0x90, 0x3c, 0x94, 0xfb, 0xc1, 0x62, 0x09, 0x46, 0x19, 0x3e, 0x1a
, 0x20, 0x3c, 0x2b, 0xd5, 0xc6, 0x75, 0xc5, 0x6c, 0x47, 0xe6, 0x5c, 0xe8, 0x70, 0x4b, 0x53, 0xf0
, 0xf4, 0x1e, 0x58, 0x0f, 0x96, 0x50, 0x3c, 0x53, 0x22, 0xa9, 0x2d, 0xd6, 0x5c, 0x89, 0x5e, 0x3d
, 0xa5, 0xe1, 0x5a, 0xa9, 0x43, 0x0c, 0x4d, 0x4d, 0xde, 0xf3, 0x56, 0xff, 0xd7, 0xe7, 0xe2, 0x2c
, 0x85, 0x71, 0xb2, 0x38, 0x5c, 0xe0, 0x3c, 0x48, 0x7e, 0xb4, 0xd0, 0x81, 0x1c, 0x00, 0x7f, 0x0f
, 0x1f, 0xdf, 0x84, 0x17, 0x1d, 0x38, 0xce, 0x85, 0x9b, 0xb5, 0xd3, 0xed, 0x5a, 0x41, 0x7a, 0xb6
, 0x0c, 0x7e, 0x10, 0x12, 0xb1, 0x00, 0x3c, 0x02, 0xf9, 0x5f, 0x44, 0x03, 0xce, 0x01, 0x10, 0x84
, 0x02, 0xba, 0x23, 0x64, 0xa4, 0x44, 0x09, 0x2e, 0xb4, 0x53, 0x15, 0x98, 0x00, 0x41, 0x75, 0x38
, 0xa0, 0x18, 0x05, 0x0f, 0xe2, 0x80, 0x3c, 0x52, 0x5b, 0x67, 0xba, 0x32, 0x68, 0x00, 0x00, 0x11
, 0xe7, 0xb9, 0x40, 0x08, 0x00, 0x80, 0xeb, 0xbb, 0x02, 0x34, 0x00, 0x00, 0x00, 0x01, 0xc9, 0x9f
, 0xb7, 0x90, 0x00, 0x00, 0x00, 0x00
};

int get_audio_file_data(char *filepath,char *buffer_data, int buf_len, int offset, int *read_len)
{
	char FilePath[256];
	HANDLE fp;
	LONG nmove;
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
    	return -1;
    }

	//get file size 
	SetFilePointer(fp, 0, &nmove, MCFILE_SEEK_BEGIN);
	file_len = GetFileSize(fp, NULL);
	if (offset >= file_len)
	{
		CloseHandle(fp);
		MercuryTrace("audio offset exceed file length \n");
    	return -1;
	}
	if(buf_len < file_len - offset)
	{
		CloseHandle(fp);
		MercuryTrace("audio_buffer not enough \n");
    	return -1;
	}
	
	SetFilePointer(fp, offset, &nmove, MCFILE_SEEK_BEGIN);
	// read audio file
	ReadFile(fp, buffer_data, file_len-offset, read_len, NULL);
		
	CloseHandle(fp);
	return 0;
}

int get_amr_data_from_list(char * strList, char * sSep, char *buf, int buf_len)
{
    char * str;
    char * p = strList;
	
	char *play_data = NULL;
	int read_len = 0, offset = 0;
	
    while (1)
    {
        str = strtok(p, sSep);
        MercuryTrace("strtok str = [%s] \r\n", str);
        if ((str == NULL) || (strlen(str) == 0))
            break;
         	
		read_len = 0;
		if (offset == 0)
			get_audio_file_data(str, buf, buf_len, 0, &read_len);
		else
			get_audio_file_data(str, buf+offset, buf_len, 6, &read_len);
		offset += read_len;
		buf_len -= read_len;
		
        p = NULL;
    }
	return offset;
}

void get_str_from_list_also_broadcast(char * strList, char * sSep) {
    char * str;
    char * p = strList;
    while (1)
    {
        str = strtok(p, sSep);
        MercuryTrace("strtok str = [%s] \r\n", str);
        if ((str == NULL) || (strlen(str) == 0))
            break;
        sdk_audio_play_file(str);
        p = NULL;
    }
}

void play_network_mode(void)
{
    int ret;

    ret = sdk_get_network_mode();
    if (ret == NETWORK_MODE_GPRS){
		// app_audio_play(AUDIO_MOBILE_NETWORK);
        
        // sdk_audio_play_nv(AUDIO_NETWORK_MODE_SIM+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_NETWORK_MODE_SIM));
        play_audio_by_name("netMSim", (unsigned char)0);

    }
    else if (ret == NETWORK_MODE_WIFI)
    {
        // app_audio_play(AUDIO_WIRELESS_NETWORK);
        // sdk_audio_play_nv(AUDIO_NETWORK_MODE_WIFI+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_NETWORK_MODE_WIFI));
        play_audio_by_name("netMWifi", (unsigned char)0);   
    }
    else
    {
        MercuryTrace("get network fail\r\n");
    }
}

void play_notification_sound(void)
{
    if (0 == sdk_audio_playing())
        sdk_audio_play_data_sync(amr_didi, sizeof(amr_didi), AUDIO_TYPE_AMR);
}

// unsigned short charge_bat_capacity_table2000[BAT_CAPACITY_STEP][2] = {
//     {4110, 100}, 
//     {4048, 90}, 
//     {3997, 80}, 
//     {3938, 70}, 
//     {3860, 60}, 
//     {3796, 50}, 
//     {3753, 40}, 
//     {3719, 30}, 
//     {3656, 20}, 
//     {3596, 10}, 
//     {3330, 0}, 
// };
// unsigned short discharge_bat_capacity_table2000[BAT_CAPACITY_STEP][2] = {
//     {4003, 100}, 
//     {3851, 90}, 
//     {3779, 80}, 
//     {3710, 70}, 
//     {3644, 60}, 
//     {3574, 50}, 
//     {3486, 40}, 
//     {3416, 30}, 
//     {3359, 20}, 
//     {3294, 10}, 
//     {3191, 0}, 
// };

unsigned short discharge_bat_capacity_table2000[BAT_CAPACITY_STEP][2] = {
    {4020, 100}, 
    {3932, 90}, 
    {3851, 80}, 
    {3771, 70}, 
    {3696, 60}, 
    {3637, 50}, 
    {3595, 40}, 
    {3566, 30}, 
    {3542, 20}, 
    {3515, 10}, 
    {3481, 0}, 
};

unsigned short charge_bat_capacity_table2000[BAT_CAPACITY_STEP][2] = {
    {4109, 100}, 
    {4019, 90}, 
    {3935, 80}, 
    {3860, 70}, 
    {3800, 60}, 
    {3754, 50}, 
    {3714, 40}, 
    {3689, 30}, 
    {3668, 20}, 
    {3640, 10}, 
    {3580, 0}, 
};

// extern void mqtt_close_thread();
extern void mqtt_soundbox_close_thread();

BOOL _is_USB_online(void) {
    SYSTEM_POWER_STATUS_EX2 battery_status = {0};

    int ret = BatteryGetStatus(&battery_status,NULL);
    if (0 != ret) {
        return FALSE;
    }

    return (battery_status.ACLineStatus != 0);
}
extern uint32 app_power_percent;
uint32 app_get_battery_percent(void)
{
    uint32 percent = 0, tempPercent;
    int count = 3;

    // ȡ n ��ƽ��ֵ
    percent = 0;
    for (int i=0; i<count; i++) {
        tempPercent = get_battery_percent(charge_bat_capacity_table2000, discharge_bat_capacity_table2000);
        MercuryTrace("\n\nBattery level : %d\n", tempPercent);
        percent += tempPercent;
    }
    percent /= count;

    // ��ʼ״̬��ֱ�Ӳ��òɼ��ĵ���
    if (app_power_percent == 0) {
        app_power_percent = percent;
    }

    // ���ʱ����ֹƯ��
    if (_is_USB_online()) {
        if (percent >= app_power_percent) {
            app_power_percent = percent;
        }
    }

    // �ŵ�ʱ����ֹƯ��
    if (!_is_USB_online()) {
        if (percent <= app_power_percent) {
            app_power_percent = percent;
        }
    }
    return app_power_percent;
	// return get_battery_percent(charge_bat_capacity_table2000, discharge_bat_capacity_table2000);
}


void play_battery_percent(void)
{

    MercuryTrace("play_battery_percent------------------------------->\r\n");

    uint32 battery_percent;
    battery_percent = app_get_battery_percent();

    MercuryTrace("Battery Percent: %d\r\n", battery_percent);

    MercuryTrace("---------------------------------> app_get_audio_type() - %d\r\n", app_get_audio_type());
	if (AUDIO_TYPE_TTS == app_get_audio_type())
	{
		char strbat[128] = {0};
		
		// sprintf(strbat, "��ص����ٷ�֮%d", battery_percent);
        sprintf(strbat, "Battery %d Percent", battery_percent);
		// // sdk_voiceplay(1, strbat);
	}
   	else
   	{
        play_audio_by_name("batH", (unsigned char)0);
        if(battery_percent != 0)
            num_to_audio_999_by_name(battery_percent, (unsigned char)0);
        else
            play_audio_by_name("eNum0", (unsigned char)0);
   	}
}

// get gprs network riss
void play_network_riss(void)
{
    BYTE rssi;
    WORD rxfull;
    char bf[64] = {0};
    char *ret_buf;

    NetworkGetSignalQuality(&rssi,&rxfull);
	MercuryTrace("Network rssi = [%d]\r\n",rssi);

	if (AUDIO_TYPE_TTS == app_get_audio_type())
	{
		char str_rssi[128] = {0};

		if (rssi == 99)
        {
			// // sdk_voiceplay(1, "û���ź�");
            // // sdk_voiceplay(1, "No Signal");	
        }
		else
		{
			// sprintf(str_rssi, "�ź�ǿ��%d", rssi);
            sprintf(str_rssi, "Signal %d", rssi);
			// // sdk_voiceplay(1, str_rssi);
		}
	}
   	else
   	{
		// app_audio_play(AUDIO_SIGNAL_STRENGTH);
        // num_to_audio_files_list((int)rssi,bf);
   		// get_str_from_list_also_broadcast(bf,"|");
        
        // #if defined(AMR_AUDIO)
        
        // // sdk_audio_play_nv(AUDIO_SIGNAL_STRENGTH+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SIGNAL_STRENGTH));
        // // num_to_audio_999(rssi, (unsigned char)0);
        // // // sdk_audio_play_nv(rssi+NV_AUDIO_START_INDEX, get_nv_audio_data_len(rssi));
        
        // play_audio_by_name("sigSgt");
        // num_to_audio_999_by_name(rssi, (unsigned char)0);


        // #elif defined(MP3_AUDIO)
		// sdk_audio_play_nv_mp3(AUDIO_SIGNAL_STRENGTH);
        // num_to_audio_999(rssi, (unsigned char)0);
        // // sdk_audio_play_nv_mp3(rssi);
		// #endif
  	  	
   	}
}
  



void audio_file_open(char *filepath,char *buffer_data, int * pnLen)
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


// extern int msg_count;
// powerkey short press
int powerkey_shortpress_callback(void)
{

    if(network_select_mode)
    {
        network_select_mode = 0;
        anfu_led_off();
        // sdk_light_control(LED_BLUE);   
    }
    // return;

    if(play_trans_mode)
    {
        play_trans_mode = 0;
        anfu_led_off();
    }

    // MercuryTrace("powerkey_shortpress_callback--------------------------> %d\r\n", msg_count);
    MercuryTrace("powerkey_shortpress_callback-------------------------->\r\n");

    // play_audio_by_name("batH");
    // play_audio_by_name("eNum9");

    play_battery_percent();
    play_network_mode();

    // char * dev_imei = GetImei(); // No neeed to free the output
    // MercuryTrace("Device IMEI: %s\r\n", dev_imei);
    
    // MercuryTrace("fw-version: %s\r\n", APP_VERSION);
	// MercuryTrace("fw-build  : %d\r\n", FW_BUILD_VERSION);
    // MercuryTrace("audio ver : %d\r\n", MOS_AUDIO_VERSION);


    // MercuryTrace("AUDIO_RESOURCE_COUNT_URL : %s\r\n", AUDIO_RESOURCE_COUNT_URL);
    // MercuryTrace("AUDIO_FILE_INFO_URL : %s\r\n", AUDIO_FILE_INFO_URL    );
    // MercuryTrace("AUDIO_RESOURCE_URL : %s\r\n", AUDIO_RESOURCE_URL);
    // MercuryTrace("AUDIO_RESOURCE_LIST : %s\r\n", AUDIO_RESOURCE_LIST);
    


    // BYTE pAudioBuff[200 * 1024] = {0};
    // int nAudioLen;
    // audio_file_open("E://audio_50001.amr",pAudioBuff,&nAudioLen);
    // sdk_audio_play_data_sync(pAudioBuff,nAudioLen,AUDIO_TYPE_AMR);






    // MercuryTrace("powerkey_shortpress_callback end-------------------------->\r\n");

    // extern BOOL g_status_fake_off;
	// SYSTEM_POWER_STATUS_EX2 battery_status = {0};

	// if( g_status_fake_off )
    // {
    //     BatteryGetStatus(&battery_status,NULL);
    //     MercuryTrace("poweroff ... USB(%d) \n", battery_status.ACLineStatus);
    //     if( 0 != battery_status.ACLineStatus )// interposition USB
    //         play_battery_percent();
	// 	return 0;
	// }

	// if (sdk_audio_playing())
	// 	return 0;

    return 0;
}
// powerkey long press
int powerkey_longpress_callback(void)
{
    MercuryTrace("powerkey_longpress_callback-------------------------->\r\n");

    extern BOOL g_status_fake_off;
	SYSTEM_POWER_STATUS_EX2 battery_status = {0};

    MercuryTrace("%s,%d powerkey long press, fakeoff(%d) ...\n",__func__,__LINE__, g_status_fake_off);

    //mqtt close thread���ػ�ʱ���������ر�MQTT������
	// CreateThread(NULL, 4*1024, mqtt_close_thread, 0, NULL, STACK_SIZE_RESERVATION, NULL);
    CreateThread(NULL, 4*1024, mqtt_soundbox_close_thread, 0, NULL, STACK_SIZE_RESERVATION, NULL);

	if( g_status_fake_off )
    {
        #if 1
		extern HANDLE g_powerkey_event_handle;
		if( g_powerkey_event_handle ){
			if( (DWORD)-1 == WaitForSingleObject(g_powerkey_event_handle, 0) )
            {
				SetEvent(g_powerkey_event_handle);
				return -1;
			}
		}
        #endif
		return NULL;
	}

    // play_audio_by_name("shut", (unsigned char)0);
    // Sleep(2000);
    play_audio_by_name("shutMsg", (unsigned char)0);
    // Sleep(2000);
    
    // NLedDeinit(LED_INDICATION);
    sdk_light_control(LED_OFF);
    Sleep(3000);


    // app_play_power_audio(0);
	
    appInfoSave();
	app_save_trans_record();

    // Sleep(5000);
    // Sleep(10000);

	BatteryGetStatus(&battery_status,NULL);
	MercuryTrace("poweroff ... USB(%d) \n", battery_status.ACLineStatus);
	if( 0 == battery_status.ACLineStatus )
    {	// no USB
		set_restart_reason( E_RST_CHKUSB );
		Sleep(1 * 1000);
		PowerOff();
	}
	else
    {
		set_restart_reason( E_RST_UNKNOWN );
		Sleep(1 * 1000);
		PowerReboot();
	}
    // sdk_power_off(NULL);
    
	return 0;
}


int play_trans_mode = 0; // �������׼�¼ģʽ
HANDLE hd_replay_timer = NULL;

int network_select_mode = 0;

static void app_replay_mode_cb(ULONG arg)
{
	play_trans_mode = 0;
	hd_replay_timer = NULL;
    anfu_led_off();
}

void replay_timer_reset(void)
{
	if(IsTimerActive(hd_replay_timer))
		DeactiveTimer(hd_replay_timer);

    ChangeTimer(hd_replay_timer,app_replay_mode_cb,5000);
	// ChangeTimer(hd_replay_timer,app_replay_mode_cb,10000);
    // ChangeTimer(hd_replay_timer,app_replay_mode_cb,20000);
	ActiveTimer(hd_replay_timer);
}

static int volum_flag = 0; //�����������С����ֻ����һ��

// volume up short press
// #define GPIO_WIFI_RESET            17                 //WIFI��λ	
// #define GPIO_WIFI_VDD_EN           13                 //WIFIʹ��
void appSetGpioOutput(uint32 gpio_num, uint8 value)
{
    MERCURY_GPIO_CFG_S gpioCfg = {0};
    gpioCfg.default_val = value;
    gpioCfg.dir = OUTPUT;
    gpioCfg.int_sense = NO_INT;
    GIO_SetMode(gpio_num, &gpioCfg);
}

void wifi_power_off_af(void)
{
    if (NETWORK_MODE_WIFI == sdk_get_network_mode())
    {
        qyy_set_wifi_sta_disconn_ap(); //�Ͽ�����
    }
    
    appSetGpioOutput(13, 0); //ȡ��ʹ��
    Sleep(1000);
    appSetGpioOutput(17, 0); //��λ
    // Set_GPIO_Low(GPIO_WIFI_VDD_EN);
    // Set_GPIO_Output(13, 0);
    // Set_GPIO_Output(17, 0);
}

void wifi_power_on_af(void)
{
    // Set_GPIO_Output(13, 1);
    // Sleep(10);
	// Set_GPIO_Input(GPIO_WIFI_RESET);
    // Set_GPIO_Output(17, 1);
    appSetGpioOutput(13, 1);//ʹ��
    appSetGpioOutput(17, 1);//��λ
    Sleep(2000);

    // wifi_power_on();
}

// volume up short press
static void volume_up_short_press(void)
{
    MercuryTrace("volume_up_short_press-------------------------->\r\n");

    int volume = app_GetVolume();
    MercuryTrace("Current Volume: %d ---------------------------->\r\n", volume);

	if (play_trans_mode) // �������׼�¼ģʽ
	{
		if (0 == sdk_audio_playing())
		{
			// trans_record_play(2);
            trans_record_play(2, 1);
			replay_timer_reset();
		}
		return;
	}

    if (network_select_mode)
	{
        // airkiss distribution network
        if (app_IsExistWifi())
        {
            MercuryTrace("app_IsExistWifi() - TRUE-------------------------->\r\n");
            wifi_power_on_af();
            ap_and_airkiss_wifi_connect(WIFI_AIRKISS);
        }
        else
        {
            // // app_audio_play(AUDIO_ONLY_MOBILE_NETWORKS);
            // sdk_audio_play_nv(AUDIO_ONLY_SIM_NETWORK+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_ONLY_SIM_NETWORK));
            play_audio_by_name("onlySim", (unsigned char)0);
        }

        // app_audio_play(AUDIO_ONLY_WIRELESS_NETWORKS);

		play_trans_mode = 0;
        network_select_mode = 0;
        anfu_led_off();
		return;
	}
    else
    {
        network_select_mode = 0;
        anfu_led_off();

        if( volume < 9 )
        {
            volume++;
            AudioSetVolume(volume);
            app_SetVolume(volume);
            //// app_audio_play(AUDIO_VOLUME_UP);
            //����led
            anfu_led_on();
            if (0 == sdk_audio_playing())
                sdk_audio_play_data_sync(amr_didi, sizeof(amr_didi), AUDIO_TYPE_AMR);
            volum_flag = 0;
            anfu_led_off();
        }
        else if (volume = 9) 
        {
            //if (volum_flag == 1)
                //return;
            if (0 == sdk_audio_playing())
            {
                // // app_audio_play(AUDIO_MAXIMUM_VOLUME);
                // sdk_audio_play_nv(AUDIO_MAX_VOLUME+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_MAX_VOLUME));
                play_audio_by_name("maxv", (unsigned char)0);
            }
            volum_flag = 1;
        }
        MercuryTrace("volume up short press. \r\n");
    }

}
// volume up long press
static void volume_up_long_press(void)
{
    MercuryTrace("volume_up_long_press-------------------------->\r\n");

    if(network_select_mode)
    {
        network_select_mode = 0;
        anfu_led_off();
        // sdk_light_control(LED_BLUE);   
    }

    if(play_trans_mode)
    {
        play_trans_mode = 0;
        anfu_led_off();
    }

    /*
    // airkiss distribution network
    if (app_IsExistWifi())
    {
        MercuryTrace("app_IsExistWifi() - TRUE-------------------------->\r\n");
        wifi_power_on_af();
        ap_and_airkiss_wifi_connect(WIFI_AIRKISS);
    }
    else
    {
        // app_audio_play(AUDIO_ONLY_MOBILE_NETWORKS);
        #if defined(AMR_AUDIO)
        sdk_audio_play_nv(AUDIO_ONLY_SIM_NETWORK+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_ONLY_SIM_NETWORK));
        #elif defined(MP3_AUDIO)
		sdk_audio_play_nv_mp3(AUDIO_ONLY_SIM_NETWORK);
		#endif
        
    }

    // app_audio_play(AUDIO_ONLY_WIRELESS_NETWORKS);
    */

    

    char snData[32+1] = {0};
    NV_Read(NV_SN, 32, snData);

    // serial_number_to_audio_mosambee(snData);
    play_audio_serial_number(snData);
    
}
// volume up double click
static void volume_up_double_press(void)
{

    if(network_select_mode)
    {
        network_select_mode = 0;
        anfu_led_off();
        // sdk_light_control(LED_BLUE);   

        // return;
    }

    if(play_trans_mode)
    {
        play_trans_mode = 0;
        anfu_led_off();
    }

    start_bcs_clear_timer();

    unsigned char d1 = 0;
    unsigned char d2 = 0;
    unsigned char d3 = 0;

    sscanf(APP_VERSION, "%d.%d.%d", &d1, &d2, &d3);
    
    if(FW_BUILD_VERSION>9)
        sdk_bcs_number(d1*10000+d2*1000+d3*100+FW_BUILD_VERSION,2);
    else
        sdk_bcs_number(d1*1000+d2*100+d3*10+FW_BUILD_VERSION, 1);


    // char ver[10] = {0};
    // sprintf(ver, "%s.%d", APP_VERSION_STR, FW_BUILD_VERSION);
    // sdk_bcs_character(ver);

    return;
}


// volume down single click
static void volume_down_short_press(void)
{
    MercuryTrace("volume_down_short_press-------------------------->\r\n");

    int volume = app_GetVolume();
    MercuryTrace("Current Volume: %d ---------------------------->\r\n", volume);

	if (play_trans_mode) // �������׼�¼ģʽ
	{
		if (0 == sdk_audio_playing())
		{
			// trans_record_play(1);
            trans_record_play(1, 1);
			replay_timer_reset();
		}
		return;
	}

    if (network_select_mode)
	{
        SIM_IMSI_T simImsi = {0};
        //�ص�wifi
        wifi_power_off_af();
        // qyy_network_wifi_deinit();

        MercuryTrace("==================> siminfo ret = %d ImsiLen = %d Imsi = %s\r\n", GetSimInfo(SIM_IMSI_E, &simImsi, NULL), simImsi.imsi_len, simImsi.imsi_val);
        if (0 == GetSimInfo(SIM_IMSI_E, &simImsi, NULL) && simImsi.imsi_len == 0)
        {
            network_select_mode = 0;
            anfu_led_off();
            return;
        }
        if (0 == NV_Delete(NV_NETWORK_MODE))
        {
            int network_mode = NETWORK_MODE_GPRS;
            NV_Write(NV_NETWORK_MODE, sizeof(int), &network_mode);

            // // app_audio_play(AUDIO_REACH_THE_MOBILE_NETWORK);
            
            // sdk_audio_play_nv(AUDIO_CONNECTING_SIM+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CONNECTING_SIM));
            
            // play_audio_by_name("connSim");
            // qyy_network_wifi_deinit();
            set_network();

            // set_restart_reason( E_RST_APPLY );
            // Sleep(4 * 1000);
            // PowerReboot();
        }
		
        play_trans_mode = 0;
        network_select_mode = 0;
        anfu_led_off();
		return;
	}
    else
    {
        network_select_mode = 0;
        anfu_led_off();

        if( volume > 3 )
        {
            volume--;
            AudioSetVolume(volume);
            app_SetVolume(volume);
            //// app_audio_play(AUDIO_VOLUME_DOWN);
            //����led
            anfu_led_on();

            if (0 == sdk_audio_playing())
                sdk_audio_play_data_sync(amr_didi, sizeof(amr_didi), AUDIO_TYPE_AMR);
            volum_flag = 0;
            anfu_led_off();
        }
        else if (volume == 3){
            //if (volum_flag == 1)
                //return;
            if (0 == sdk_audio_playing())
            {
                // // app_audio_play(AUDIO_MINIMUM_VOLUME);
                // sdk_audio_play_nv(AUDIO_MIN_VOLUME+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_MIN_VOLUME));
                play_audio_by_name("minv", (unsigned char)0);
            }
            volum_flag = 1;
        }
        MercuryTrace("volume down short press. \r\n");

    }

    
}
// volume down long press
static void volume_down_long_press(void)
{
    MercuryTrace("volume_down_long_press-------------------------->\r\n");

    if(network_select_mode)
    {
        network_select_mode = 0;
        anfu_led_off();
        // sdk_light_control(LED_BLUE);
    }

    if(play_trans_mode)
    {
        play_trans_mode = 0;
        anfu_led_off();
    }

    /*
    // ap distribution network
    if (app_IsExistWifi())
    {
        MercuryTrace("app_IsExistWifi() - TRUE-------------------------->\r\n");
        wifi_power_on_af();
        ap_and_airkiss_wifi_connect(WIFI_AP);
    }
    else
    {
 		// app_audio_play(AUDIO_ONLY_MOBILE_NETWORKS);
        #if defined(AMR_AUDIO)
        sdk_audio_play_nv(AUDIO_ONLY_SIM_NETWORK+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_ONLY_SIM_NETWORK));
        #elif defined(MP3_AUDIO)
		sdk_audio_play_nv_mp3(AUDIO_ONLY_SIM_NETWORK);
		#endif
    }
    */

#if 1
	int network_mode;
    AppWifiInfoStruct wifi_info = {0};
    
    // ֧��WIFI
    if (app_IsExistWifi())
    {
        // �ж��Ƿ����ù�WIFI����
        if (0 != NV_Read(NV_WIFI_INFO, sizeof(AppWifiInfoStruct),(BYTE*)&wifi_info) || 0 == strlen(wifi_info.ssid))
        {
            MercuryTrace("WiFi Configuration - Not Present\r\n");
            // // app_audio_play(AUDIO_NO_WIRELESS_NETWORK_CONFIGURED);
            
            // sdk_audio_play_nv(AUDIO_WIFI_NOT_CONFIGURED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_NOT_CONFIGURED));
            play_audio_by_name("wifiNotConf", (unsigned char)0);
            
        }
        else
        {
            // switching network
            if(0 == NV_Read(NV_NETWORK_MODE,sizeof(int),&network_mode))
            {
                if (network_mode == NETWORK_MODE_GPRS)
                {
                    // wifi����ʹ��
                    wifi_power_on_af();
                    if(0 == NV_Delete(NV_NETWORK_MODE))
                    {

                        network_mode = NETWORK_MODE_WIFI;
                        NV_Write(NV_NETWORK_MODE,sizeof(int),&network_mode);
                        
                        // // app_audio_play(AUDIO_REACH_THE_WIRELESS_NETWORK);
                        // qyy_network_gprs_deinit();
                        
                        // sdk_audio_play_nv(AUDIO_CONNECTING_WIFI+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CONNECTING_WIFI));
                        // play_audio_by_name("connWifi");
                        
                        set_network();

                        // set_restart_reason( E_RST_APPLY );
                        // Sleep(4 * 1000);
                        // PowerReboot();
                    }
                }
                else if (network_mode == NETWORK_MODE_WIFI )
                {
                    SIM_IMSI_T simImsi = {0};
                    //�ص�wifi
                    wifi_power_off_af();
                    // qyy_network_wifi_deinit();
                    MercuryTrace("==================> siminfo ret = %d ImsiLen = %d Imsi = %s\r\n", GetSimInfo(SIM_IMSI_E, &simImsi, NULL), simImsi.imsi_len, simImsi.imsi_val);
					if (0 == GetSimInfo(SIM_IMSI_E, &simImsi, NULL) && simImsi.imsi_len == 0)
                        return;
                    if (0 == NV_Delete(NV_NETWORK_MODE))
                    {
                        network_mode = NETWORK_MODE_GPRS;
                        NV_Write(NV_NETWORK_MODE, sizeof(int), &network_mode);

                        // // app_audio_play(AUDIO_REACH_THE_MOBILE_NETWORK);
                        
                        // sdk_audio_play_nv(AUDIO_CONNECTING_SIM+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CONNECTING_SIM));
                        
                        // play_audio_by_name("connSim");
                        // qyy_network_wifi_deinit();
                        set_network();

                        // set_restart_reason( E_RST_APPLY );
                        // Sleep(4 * 1000);
                        // PowerReboot();
                    }
                }
            }
        }
    }
    else
    {
 		// // app_audio_play(AUDIO_ONLY_MOBILE_NETWORKS);
        
        // sdk_audio_play_nv(AUDIO_ONLY_SIM_NETWORK+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_ONLY_SIM_NETWORK));
        play_audio_by_name("onlySim", (unsigned char)0);
    } 
#endif



}



// replay single click
static void replay_short_press(void)
{
    if(network_select_mode)
    {
        network_select_mode = 0;
        // anfu_led_off();
        // return;
    }

    if(play_trans_mode)
    {
        play_trans_mode = 0;
        anfu_led_off();
    }

    MercuryTrace("replay_short_press-------------------------->\r\n");

    anfu_led_on();    	
    // trans_record_play(0);
    trans_record_play(0, 0);

    anfu_led_off();
}
// replay double click
static void replay_double_press(void)
{
    if(network_select_mode)
    {
        network_select_mode = 0;
        anfu_led_off();
        // return;
    }

	TIMER_CONFIG_S cfg_timer;

	// if (0 == sdk_audio_playing())
    // 	trans_record_play(0);

	MercuryTrace("replay_double_press----------------------------------------\r\n");
	
	play_trans_mode = 1;
    g_transRecord.rindex = -1;

    anfu_led_on();

	if(NULL != hd_replay_timer)
		return;

	cfg_timer.timer_name = "APP_REPLAY_RECORD";
	cfg_timer.timer_fun = app_replay_mode_cb;
	cfg_timer.input = 0;
	cfg_timer.timer_expire = 5 * 1000;
	cfg_timer.period = TIMER_ONCE_TIME;	// once mode
	cfg_timer.auto_activate = TIMER_AUTO_ACTIVATE;

	hd_replay_timer = CreateTimer(&cfg_timer);

    trans_record_play(0,1);
}
// replay long press
static void replay_long_press(void)
{
    MercuryTrace("replay_long_press----------------------------------------\r\n");

    if(!network_select_mode)
    {
        network_select_mode = 1;
        play_audio_by_name("configNet", (unsigned char)0);
        anfu_led_on();
        // sdk_light_control(LED_GREEN);
    }
    else
    {
        network_select_mode = 0;
        anfu_led_off();
        // sdk_light_control(LED_BLUE);
    }
    

    if(play_trans_mode)
    {
        play_trans_mode = 0;
        anfu_led_off();
    }

#if 0
	int network_mode;
    AppWifiInfoStruct wifi_info = {0};
    
    // ֧��WIFI
    if (app_IsExistWifi())
    {
        // �ж��Ƿ����ù�WIFI����
        if (0 != NV_Read(NV_WIFI_INFO, sizeof(AppWifiInfoStruct),(BYTE*)&wifi_info) || 0 == strlen(wifi_info.ssid))
        {
            MercuryTrace("WiFi Configuration - Not Present\r\n");
            // app_audio_play(AUDIO_NO_WIRELESS_NETWORK_CONFIGURED);
            #if defined(AMR_AUDIO)
            sdk_audio_play_nv(AUDIO_WIFI_NOT_CONFIGURED+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_WIFI_NOT_CONFIGURED));
            #elif defined(MP3_AUDIO)
            sdk_audio_play_nv_mp3(AUDIO_WIFI_NOT_CONFIGURED);
            #endif
            
        }
        else
        {
            // switching network
            if(0 == NV_Read(NV_NETWORK_MODE,sizeof(int),&network_mode))
            {
                if (network_mode == NETWORK_MODE_GPRS)
                {
                    // wifi����ʹ��
                    wifi_power_on_af();
                    if(0 == NV_Delete(NV_NETWORK_MODE))
                    {

                        network_mode = NETWORK_MODE_WIFI;
                        NV_Write(NV_NETWORK_MODE,sizeof(int),&network_mode);
                        
                        // app_audio_play(AUDIO_REACH_THE_WIRELESS_NETWORK);
                        // qyy_network_gprs_deinit();
                        #if defined(AMR_AUDIO)
                        sdk_audio_play_nv(AUDIO_CONNECTING_WIFI+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CONNECTING_WIFI));
                        #elif defined(MP3_AUDIO)
                        sdk_audio_play_nv_mp3(AUDIO_CONNECTING_WIFI);
                        #endif
                        
                        set_network();

                        // set_restart_reason( E_RST_APPLY );
                        // Sleep(4 * 1000);
                        // PowerReboot();
                    }
                }
                else if (network_mode == NETWORK_MODE_WIFI )
                {
                    SIM_IMSI_T simImsi = {0};
                    //�ص�wifi
                    wifi_power_off_af();
                    // qyy_network_wifi_deinit();
                    MercuryTrace("==================> siminfo ret = %d ImsiLen = %d Imsi = %s\r\n", GetSimInfo(SIM_IMSI_E, &simImsi, NULL), simImsi.imsi_len, simImsi.imsi_val);
					if (0 == GetSimInfo(SIM_IMSI_E, &simImsi, NULL) && simImsi.imsi_len == 0)
                        return;
                    if (0 == NV_Delete(NV_NETWORK_MODE))
                    {
                        network_mode = NETWORK_MODE_GPRS;
                        NV_Write(NV_NETWORK_MODE, sizeof(int), &network_mode);

                        // app_audio_play(AUDIO_REACH_THE_MOBILE_NETWORK);
                        #if defined(AMR_AUDIO)
                        sdk_audio_play_nv(AUDIO_CONNECTING_SIM+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CONNECTING_SIM));
                        #elif defined(MP3_AUDIO)
                        sdk_audio_play_nv_mp3(AUDIO_CONNECTING_SIM);
                        #endif

                        // qyy_network_wifi_deinit();
                        set_network();
                        // set_restart_reason( E_RST_APPLY );
                        // Sleep(4 * 1000);
                        // PowerReboot();
                    }
                }
            }
        }
    }
    else
    {
 		// app_audio_play(AUDIO_ONLY_MOBILE_NETWORKS);
        #if defined(AMR_AUDIO)
        sdk_audio_play_nv(AUDIO_ONLY_SIM_NETWORK+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_ONLY_SIM_NETWORK));
        #elif defined(MP3_AUDIO)
        sdk_audio_play_nv_mp3(AUDIO_ONLY_SIM_NETWORK);
        #endif
    } 
#endif


}



int save_device_debug_info_to_nv(char *deviceDebugInfo)
{
	int ret = 0;
	int deviceDebugInfoLen = 0;

	if (deviceDebugInfo == NULL)
		return -1;

	deviceDebugInfoLen = strlen(deviceDebugInfo);

	if (deviceDebugInfoLen > 0)
	{
		NV_Delete(NV_DEVICE_DEBUG_INFO);
		ret = NV_Write(NV_DEVICE_DEBUG_INFO, deviceDebugInfoLen, deviceDebugInfo);
		if (0 != ret)
		{
			NV_Delete(NV_DEVICE_DEBUG_INFO);
			ret = NV_Write(NV_DEVICE_DEBUG_INFO, deviceDebugInfoLen, deviceDebugInfo);
		}
	}

	return ret;
}


static void volume_down_double_press(void)
{
	// char *ptr = NULL;
	// char buff[256] = {0};

	// int i = 0, j=0;
	// char buff_imei[64] = {0};
	// char buff_ver[64] = {0};
	// char buff_CCID[64] = {0};
	// SIM_IMSI_T imsi;
	// SIM_ICCID_T ccid;

	// strcpy(buff, APP_VERSION);
	// ptr = buff;
	// for(i=0, j=0; i <strlen(ptr); i++)
	// {
	// 	if (*(ptr+i) == '.')
	// 	{
	// 		strcat(buff_ver, "��");
	// 		j += 2;
	// 	}
	// 	else
	// 	{
	// 		buff_ver[j++] = *(ptr+i);
	// 	}
	// }

	
	// // "123" >> "1 2 3 "
	// memset(buff_imei, ' ', sizeof(buff_imei) - 1);
	// ptr = GetImei();
	// for(i =0; i <strlen(ptr); i++)
	// {
	// 	buff_imei[i * 2] = *(ptr + i);
	// }
	// MercuryTrace("%s,%s \n", ptr, buff_imei);
	// memset(buff, 0, sizeof(buff));

	// if (1 == GetSimInfo(SIM_ALL, &imsi, &ccid)) //����1��ʾ�ɹ���0��ʾʧ��
	// {
	// 	MercuryTrace("get sim imsi & ccid success \n");
	// 	ptr = ccid.id_num;
	// 	for(i =0; i <strlen(ptr); i++)
	// 	{
	// 		buff_CCID[i*2] = *(ptr + i);
	// 		buff_CCID[i*2+1] = ' ';
	// 	}
	// }
	// if (strlen(buff_CCID))
    //     // sprintf(buff, "�汾��Ϊ%s,IMEI��Ϊ%s,ICCID��Ϊ%s"		,  buff_ver, buff_imei, buff_CCID);
    //     sprintf(buff, "Version %s,IMEI %s,ICCID %s"		,  buff_ver, buff_imei, buff_CCID);
	// else
    //     // sprintf(buff, "�汾��Ϊ%s,IMEI��Ϊ%s"		,  buff_ver, buff_imei);
    //     sprintf(buff, "Version %s,IMEI %s"		,  buff_ver, buff_imei);
        
    // MercuryTrace("VD DP - Buff = %s\r\n", buff);

    // // sdk_voiceplay(1, buff);
	return NULL;
}

// key
static int flag = 0;
/*--------------------------------------------------------------------
keypad call back
you can't take too much time at this callback
----------------------------------------------------------------------*/
static void _KeyPadProc(UINT uid,UINT singleCode,UINT keyCode)
{
    int keyvalue = 0;
    static ULONG before_tick = 0;
    ULONG after_tick = 0;

    if (flag == 1)
    {
        flag = 0;
        return;
    }

    MercuryTrace("_KeyPadProc: uid %d, singlecode %d, keycode %d\r\n",uid, singleCode, keyCode);

    if(singleCode == MC_KPD_KEYDOWN)
    {
        flag = 1;
        GetTickCount(&before_tick);
        // MercuryTrace("before_tick = %d\r\n",before_tick);
    }

    if(singleCode == MC_KPD_KEYUP)
    {
        flag = 1;
        GetTickCount(&after_tick);
        // MercuryTrace("after_tick = %d\r\n",after_tick);
        // MercuryTrace("after_tick - before_tick = %d\r\n", after_tick-before_tick);
        if ((after_tick - before_tick) > 2000)
        {
            // long pass
            if(keyCode == 0x03)
            {
                // volume up long press
                volume_up_long_press();
                keyvalue = 0;
            }
            else if (keyCode == 0x0D)
            {
                // replay long press
                replay_long_press();
                keyvalue = 10;
            }
            else if(keyCode == 0x04)
            {
                // volume down long press
                volume_down_long_press();
                keyvalue = 13;
            }
        }
        else
        {
            // short pass
            if(keyCode == 0x03)
            {
                // volume up short press
                volume_up_short_press();
                keyvalue = 0;
            }
            else if (keyCode == 0x0D)
            {
                // replay short press
                replay_short_press();
                keyvalue = 10;
            }
            else if(keyCode == 0x04)
            {
                // volume down short press
                volume_down_short_press();
                keyvalue = 13;
            }
        }
        MercuryTrace("keycode: %d, key value:%d\r\n", keyCode, keyvalue);
    }
}

// keypad init
void keypad_init_matrix(void)
{
    KP_Init();
    KP_RegisterApp(UID_1, _KeyPadProc);
    KP_SetFocus(UID_1);
}

// keypad scan thread
void keypad_scan_thread( ULONG argc, LPVOID lpThreadParameter )
{
    while(1)
	{
        sdk_keypad_scan_handle();
    }
}

int keypad_init_io(void)
{
    // MercuryTrace("keypad_init_io-------------------------->\r\n");
    
    char debug_buff[256] = {0};
    strcpy(debug_buff, "keypad_init_io-------------------------->\r\n");
    MercuryDebug(debug_buff, strlen(debug_buff));

    sdk_reg_key_short_cb(volume_down_short_press
                    ,volume_up_short_press
                    ,replay_short_press);
	
	sdk_reg_key_long_cb(volume_down_long_press
                    ,volume_up_long_press
                    ,replay_long_press
                    ,NULL);

	sdk_reg_key_double_cb(volume_down_double_press,
		                  volume_up_double_press,
		                  replay_double_press);

    //Create keypad thread
    keypad_thread_handle = CreateThread(NULL,8*1024,keypad_scan_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);
    SetThreadPriority(keypad_thread_handle,OSI_PRIORITY_ABOVE_NORMAL);

    return 0;
}
