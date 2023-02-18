#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryNv.h"
#include "MercuryBattery.h"
#include "MercuryThread.h"
#include "MercuryTts.h"

#include "SDK_API_AUDIO.h"
#include "SDK_API_SYS.h"
#include "SDK_API_WIFI.h"
#include "SDK_API_LED.h"
#include "SDK_API_BCS_DISPLAY.h"

#include "vendor.h"
#include "nv.h"
#include "device.h"

// ��ǰ����ģʽ��¼
#define SPEAKER_TEST_MODE        1
#define KEY_TEST_MODE            2
#define NETWORK_TEST_MODE        3
#define LED_TEST_MODE            4
#define USB_TEST_MODE            5
#define BCS_TEST_MODE            6
#define TEST_MODE_NULL           0

// �������Խ����¼
#define UPKEY_RESULTS_VALUE         0B0001
#define DOWNKEY_RESULTS_VALUE       0B0010
#define REPLAYKEY_RESULTS_VALUE     0B0100
#define POWERKEY_RESULTS_VALUE      0B1000
#define CHECK_KEY_RESULTS_VALUE     0B1111

// ��¼����ģʽ���Խ��
typedef struct
{
    BOOL speakerTestResults;
    BOOL keyTestResults;
    BOOL gprsTestResults;
    BOOL wifiTestResults;
    BOOL ledTestResults;
    BOOL usbTestResults;
    BOOL bcsTestResults;
} VENDORTESTRESULTS;

VENDORTESTRESULTS l_testResults;        // ��¼��������Ƿ�ɹ�
int l_nVendorTestEvent = 0;             // �����¼�
int l_nTestMode = TEST_MODE_NULL;       // ����ģʽ
int l_nKeyTestRecord = 0;               // �����������
int l_whetherHWconfigwritten = 0;       // �Ƿ�д��Ӳ��������Ϣ
HANDLE l_vendor_keypad_thread_handle;   //�����߳�handle

const char vendor_amr_didi[294]={
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

// ���ò���ģʽ
void _vendor_set_test_mode(int nTestMode)
{
    l_nTestMode = nTestMode;
}

int vendor_amr_complete_callback(void)
{
    if (l_nKeyTestRecord == CHECK_KEY_RESULTS_VALUE)
        l_nVendorTestEvent = 1;
}

int vendor_tts_complete_callback(void)
{
    if (l_nTestMode == KEY_TEST_MODE)
        if (l_nKeyTestRecord == CHECK_KEY_RESULTS_VALUE)
            l_nVendorTestEvent = 1;
}

// ���ȼ��
int vendor_speaker_test(void)
{
    int volume = 3;

    // sdk_voiceplay(1,"���Ȳ���");
    Sleep(2 * 1000);

    _vendor_set_test_mode(SPEAKER_TEST_MODE);
    l_nVendorTestEvent = 0;

	volume = 9;
    while (!l_nVendorTestEvent)
    {
     	if (volume == 9)
            volume = 3;
		else if(volume == 3)
			volume = 9;
		
        AudioSetVolume(volume);
		
        MercuryTrace("l_nVendorTestEvent = [%d] l_nTestMode = [%d]\r\n",l_nVendorTestEvent,l_nTestMode);
        // sdk_voiceplay(1,"֧�������������Ԫ");

        Sleep(4 * 1000);
    }
    _vendor_set_test_mode(TEST_MODE_NULL);
    
    AudioSetVolume(5);

    if(l_nVendorTestEvent > 0) {
        // sdk_voiceplay(1,"���Ȳ���ͨ��");
        l_testResults.speakerTestResults = TRUE;
    }else {
        // sdk_voiceplay(1,"���Ȳ���ʧ��");
        l_testResults.speakerTestResults = FALSE;
    }

    Sleep(2 * 1000);
    l_nVendorTestEvent = 0;

    return 0;
}
void replay(void)
{
    for (int i = 0; i < 2; i++)
    {
        AudioSetVolume(3);
        // sdk_voiceplay(1, "֧�������������Ԫ");
        Sleep(4 * 1000);

        AudioSetVolume(4);
        // sdk_voiceplay(1, "֧�������������Ԫ");
        Sleep(4 * 1000);
    }
}
//���Ȳ���2
int vendor_speaker_test2(void)
{
    // sdk_voiceplay(1,"���Ȳ���");
    Sleep(2 * 1000);
    _vendor_set_test_mode(SPEAKER_TEST_MODE);
    l_nVendorTestEvent = 0;
    int j = 1;
    while (!l_nVendorTestEvent)
    {
        while(j)
        {
                j = 0;
                replay();
                MercuryTrace("................2.............");
        }
        Sleep(3000);
        MercuryTrace("................3.............");
    }
    MercuryTrace("................5.............");
    _vendor_set_test_mode(TEST_MODE_NULL);

    AudioSetVolume(5);

    if(l_nVendorTestEvent > 0) {
        // sdk_voiceplay(1,"���Ȳ���ͨ��");
        l_testResults.speakerTestResults = TRUE;
    }else {
        // sdk_voiceplay(1,"���Ȳ���ʧ��");
        l_testResults.speakerTestResults = FALSE;
    }

    Sleep(2 * 1000);
    l_nVendorTestEvent = 0;

    return 0;
}

// �������
int vendor_key_test(void)
{
    int count = 0;
    // sdk_voiceplay(1,"��������");
    Sleep(2 * 1000);

    _vendor_set_test_mode(KEY_TEST_MODE);
    l_nVendorTestEvent = 0;
    while (!l_nVendorTestEvent)
    {
        // time out
        if (count > 30) {
            // sdk_voiceplay(1,"�������Գ�ʱ");
            Sleep(2 * 1024);
            l_nVendorTestEvent = -1;
        }
        Sleep(500);
        count++;
    }
    _vendor_set_test_mode(TEST_MODE_NULL);

    sdk_light_control(LED_OFF);
    sdk_light_control(LED_RED);

    if(l_nVendorTestEvent > 0) {
        // sdk_voiceplay(1,"��������ͨ��");
        l_testResults.keyTestResults = TRUE;
    }else {
        // sdk_voiceplay(1,"��������ʧ��");
        l_testResults.keyTestResults = FALSE;
    }

    Sleep(2 * 1000);
    l_nVendorTestEvent = 0;

    return 0;
}

// �ƶ�������
int vendor_gprs_test(void)
{
    int count = 0;
    int gprs_network_status;

    // sdk_voiceplay(1,"�ƶ��������");
    Sleep(2 * 1000);

network_reinit:
    // set network to GPRS mode
    // sdk_set_network_mode(NETWORK_MODE_GPRS);

    // GPRS initialization    
    // if(0 != sdk_network_gprs_init())
        // goto network_reinit;

    while(count < 15)
    {
        gprs_network_status = sdk_get_gprs_network_status();
        if(GPRS_NETWORK_STATUS_OK == gprs_network_status) {
            break;
        } else {
            count++;
        }
        Sleep(1000);
    }

    // _vendor_set_test_mode(NETWORK_TEST_MODE);
    // l_nVendorTestEvent = 0;
    // while (1)
    // {
    //     if (count > 15) {
    //         // sdk_voiceplay(1,"�����쳣");
    //     } else {
    //         break;
    //     }
    //     Sleep(2 * 1000);

    //     if (l_nVendorTestEvent == 1) {
    //         _vendor_set_test_mode(TEST_MODE_NULL);
    //         goto network_reinit;
    //     } else if (l_nVendorTestEvent == -1) {
    //         break;
    //     }
    // }
    // _vendor_set_test_mode(TEST_MODE_NULL);

    if (count < 15) {
        // sdk_voiceplay(1,"�ƶ��������ͨ��");
        l_testResults.gprsTestResults = TRUE;
    } else {
        // sdk_voiceplay(1,"�ƶ��������ʧ��");
        l_testResults.gprsTestResults = FALSE;
    }

    Sleep(2 * 1000);
    // l_nVendorTestEvent = 0;

    return 0;
}

int vendor_wifi_connect(char *ssid, char *pwd)
{
	int ret = -1;
	int count = 0;
	BOOL wifi_init_OK;
	
	 while(count < 10)
    {
        wifi_init_OK = sdk_get_wifi_init_OK();
        if(TRUE == wifi_init_OK) {
            //wifi init OK
            MercuryTrace("wifi init OK\r\n");
            ret = sdk_start_connect(ssid, pwd, NULL, 30);
            break;
        } else {
            count++;
        }
        Sleep(1000);
    }

	 if (count < 10 && ret == 0) 
	 	return 0;

	 return -1;
}

// ����������
int vendor_wifi_test(void)
{
    int ret = -1;
    int count = 0;
	BOOL wifi_init_OK;

    // sdk_voiceplay(1,"�����������");
    Sleep(2 * 1000);

wifi_reinit:
    // set network to WIFI mode
    sdk_set_network_mode(NETWORK_MODE_WIFI);

    // WIFI initialization
    if(0 != sdk_network_wifi_init())
        goto wifi_reinit;

	ret = vendor_wifi_connect("JBX", "ld123456@");
    // ret = vendor_wifi_connect("lindu2021", "ld123456@");
	if (ret != 0)
	{
		ret = vendor_wifi_connect("lindu2021", "ld123456@");
		if (ret != 0)
			ret = vendor_wifi_connect("Yulaba", "ld123456@");
	}
	
   
    if (ret == 0) {
        // sdk_voiceplay(1,"�����������ͨ��");
        l_testResults.wifiTestResults = TRUE;
    } else {
        // sdk_voiceplay(1,"�����������ʧ��");
        l_testResults.wifiTestResults = FALSE;
    }

    Sleep(2 * 1000);
    // l_nVendorTestEvent = 0;

    return 0;
}

// LED�����߳�
void vendor_let_test_thread(void)
{
    int gap = 200;

    while (!l_nVendorTestEvent)
    {
        sdk_light_control(LED_RED);
		Sleep(gap);

        anfu_led_on();
		sdk_light_control(LED_GREEN);
		Sleep(gap);
        anfu_led_off();

		sdk_light_control(LED_BLUE);
		Sleep(gap);
    }
    
    Sleep(1000);

    sdk_light_control(LED_RED);
    anfu_led_off();

    ExitThread(NULL);
}

// LED���
int vendor_led_test(void)
{
    // sdk_voiceplay(1,"LED����");
    Sleep(2 * 1000);

    _vendor_set_test_mode(LED_TEST_MODE);
    l_nVendorTestEvent = 0;
    CreateThread(NULL,4*1024,vendor_let_test_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);
    while (!l_nVendorTestEvent)
    {
        Sleep(500);
    }
    _vendor_set_test_mode(TEST_MODE_NULL);

    if(l_nVendorTestEvent > 0) {
        // sdk_voiceplay(1,"LED����ͨ��");
        l_testResults.ledTestResults = TRUE;
    }else {
        // sdk_voiceplay(1,"LED����ʧ��");
        l_testResults.ledTestResults = FALSE;
    }

    Sleep(2 * 1000);
    l_nVendorTestEvent = 0;

    return 0;
}

// usb���
int vendor_usb_test(void)
{
    int count = 0;
	SYSTEM_POWER_STATUS_EX2 battery_status = {0};

    // sdk_voiceplay(1,"USB����");
    Sleep(2 * 1000);

    _vendor_set_test_mode(USB_TEST_MODE);
    l_nVendorTestEvent = 0;
    while (!l_nVendorTestEvent)
    {
        BatteryGetStatus(&battery_status,NULL);

        if (battery_status.ACLineStatus == 1) {
            l_nVendorTestEvent = 1;
            break;
        }

        if (count > 30) {
            // sdk_voiceplay(1,"USB���Գ�ʱ");
            Sleep(2 * 1000);
            l_nVendorTestEvent = -1;
        }
        count++;

        Sleep(500);
    }
    _vendor_set_test_mode(TEST_MODE_NULL);

    if(l_nVendorTestEvent > 0) {
        // sdk_voiceplay(1,"USB����ͨ��");
        l_testResults.usbTestResults = TRUE;
    }else {
        // sdk_voiceplay(1,"USB����ʧ��");
        l_testResults.usbTestResults = FALSE;
    }

    Sleep(2 * 1000);
    l_nVendorTestEvent = 0;

    return 0;
}

// �����������߳� one
void vendor_bcs_test_thread(void)
{
#define VENDOR_BCS_GPRS_RSSI_ICON              0x01
#define VENDOR_BCS_GPRS_RSSI_INVALID_ICON      0x02
#define VENDOR_BCS_WIFI_RSSI_ICON              0x04
#define VENDOR_BCS_WIFI_RSSI_INVALID_ICON      0x08
#define VENDOR_BCS_BLUETOOTH_RSSI_ICON         0x10
#define VENDOR_BCS_BLUETOOTH_RSSI_INVALID_ICON 0x20
#define VENDOR_BCS_SPEAKER_ICON                0x40
#define VENDOR_BCS_YUAN                        0x80

	sdk_bcs_init();
	while (!l_nVendorTestEvent)
	{
        sdk_bcs_number(888888,2);
        qyy_bcs_set_icon(VENDOR_BCS_GPRS_RSSI_ICON | VENDOR_BCS_GPRS_RSSI_INVALID_ICON | VENDOR_BCS_WIFI_RSSI_ICON | VENDOR_BCS_WIFI_RSSI_INVALID_ICON |
            VENDOR_BCS_BLUETOOTH_RSSI_ICON | VENDOR_BCS_BLUETOOTH_RSSI_INVALID_ICON | VENDOR_BCS_SPEAKER_ICON, TRUE);

        Sleep(2000);

        sdk_bcs_number(888888,1);
        qyy_bcs_set_icon(VENDOR_BCS_GPRS_RSSI_ICON | VENDOR_BCS_GPRS_RSSI_INVALID_ICON | VENDOR_BCS_WIFI_RSSI_ICON | VENDOR_BCS_WIFI_RSSI_INVALID_ICON |
            VENDOR_BCS_BLUETOOTH_RSSI_ICON | VENDOR_BCS_BLUETOOTH_RSSI_INVALID_ICON | VENDOR_BCS_SPEAKER_ICON, FALSE);

        Sleep(2000);
	}

    Sleep(2000);
    sdk_bcs_number(0,0);
    qyy_bcs_set_icon(VENDOR_BCS_GPRS_RSSI_ICON | VENDOR_BCS_GPRS_RSSI_INVALID_ICON | VENDOR_BCS_WIFI_RSSI_ICON | VENDOR_BCS_WIFI_RSSI_INVALID_ICON |
        VENDOR_BCS_BLUETOOTH_RSSI_ICON | VENDOR_BCS_BLUETOOTH_RSSI_INVALID_ICON | VENDOR_BCS_SPEAKER_ICON, FALSE);
	
    ExitThread(NULL);
}

// bcs���
int vendor_bcs_test(void)
{
    int count = 0;
    // sdk_voiceplay(1,"����������");
    Sleep(2 * 1000);

    _vendor_set_test_mode(BCS_TEST_MODE);
    l_nVendorTestEvent = 0;
    CreateThread(NULL,4*1024,vendor_bcs_test_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);
    while (!l_nVendorTestEvent)
    {
        if (count > 40) {
            // sdk_voiceplay(1,"���������Գ�ʱ");
            Sleep(3 * 1000);
            l_nVendorTestEvent = -1;
        }
        count++;
        Sleep(500);
    }
    _vendor_set_test_mode(TEST_MODE_NULL);

    if(l_nVendorTestEvent > 0) {
        // sdk_voiceplay(1,"����������ͨ��");
        l_testResults.bcsTestResults = TRUE;
    }else {
        // sdk_voiceplay(1,"����������ʧ��");
        l_testResults.bcsTestResults = FALSE;
    }

    Sleep(2 * 1000);
    l_nVendorTestEvent = 0;

    return 0;
}

void _set_key_test_state(int nValue, int nLedNum)
{
    if (l_nTestMode == KEY_TEST_MODE) {
        l_nKeyTestRecord |= nValue;
        sdk_light_control(nLedNum);
    }
    
    // if (l_nKeyTestRecord == CHECK_KEY_RESULTS_VALUE)
    //     l_nVendorTestEvent = 1;
}

void powerkey_amr_thread(void)
{
    //sdk_audio_play_data_sync(vendor_amr_didi, sizeof(vendor_amr_didi), AUDIO_TYPE_AMR);
    if (0 == sdk_audio_playing())
    {
        // // sdk_voiceplay(1, "��Դ��");
        // sdk_voiceplay(1, "Power Key AMR Thread");
    }
    ExitThread(NULL);
}

int vendor_powerkey_short_callback(void)
{
    if (l_nTestMode == KEY_TEST_MODE)
        CreateThread(NULL,4*1024,powerkey_amr_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);

    _set_key_test_state(POWERKEY_RESULTS_VALUE, LED_RED | LED_GREEN | LED_BLUE);
    return 0;
}

int vendor_powerkey_long_callback(void)
{
	// // sdk_voiceplay(1, "���ڹػ�");
    // sdk_voiceplay(1, "Power Key Long Press");
	Sleep(3000);
	return 0;
}

void *vendor_volume_up_short_press(void *key)
{
    MercuryTrace("volume_up_short_press--------------------------->\r\n");
    switch (l_nTestMode)
    {
        case SPEAKER_TEST_MODE:
            l_nVendorTestEvent = 1;
            break;
        case KEY_TEST_MODE:
            _set_key_test_state(UPKEY_RESULTS_VALUE,LED_RED);
            if (0 == sdk_audio_playing())
            {
            // sdk_voiceplay(1, "������");
            }
            //sdk_audio_play_data_sync(vendor_amr_didi, sizeof(vendor_amr_didi), AUDIO_TYPE_AMR);
            break;
        case NETWORK_TEST_MODE:
            break;
        case LED_TEST_MODE:
            l_nVendorTestEvent = 1;
            break;
        case USB_TEST_MODE:
            break;
        case BCS_TEST_MODE:
            l_nVendorTestEvent = 1;
            break;
        default:
            break;
    }
}

void *vendor_volume_down_short_press(void *key)
{
    MercuryTrace("volume_down_short_press---------------------------->\r\n");
    switch (l_nTestMode)
    {
        case SPEAKER_TEST_MODE:
            l_nVendorTestEvent = -1;
            break;
        case KEY_TEST_MODE:
            _set_key_test_state(DOWNKEY_RESULTS_VALUE,LED_GREEN);
            if (0 == sdk_audio_playing()){
			// sdk_voiceplay(1, "������");
            }
            //sdk_audio_play_data_sync(vendor_amr_didi, sizeof(vendor_amr_didi), AUDIO_TYPE_AMR);
            break;
        case NETWORK_TEST_MODE:
            break;
        case LED_TEST_MODE:
            l_nVendorTestEvent = -1;
            break;
        case USB_TEST_MODE:
            break;
        case BCS_TEST_MODE:
            l_nVendorTestEvent = -1;
            break;
        default:
            break;
    }
}

void *vendor_replay_short_press(void *key)
{
    MercuryTrace("replay_short_press---------------------------->\r\n");
    _set_key_test_state(REPLAYKEY_RESULTS_VALUE,LED_BLUE);
    if (l_nTestMode == KEY_TEST_MODE){
        if (0 == sdk_audio_playing()){
        //sdk_audio_play_data_sync(vendor_amr_didi, sizeof(vendor_amr_didi), AUDIO_TYPE_AMR);
        // sdk_voiceplay(1, "���ܼ�");
        }
    }else if(l_nTestMode == SPEAKER_TEST_MODE){
        MercuryTrace("................4.............");
        replay();
    }
}


void *vendor_volume_down_long_press(void *key)
{
    return;
}
void *vendor_volume_up_long_press(void *key){
    return;
}
void *vendor_replay_long_press(void *key){
    return;
}

// keypad scan thread
void vendor_keypad_scan_thread( ULONG argc, LPVOID lpThreadParameter )
{
    while(1){
        sdk_keypad_scan_handle();
    }
}

int vendor_keypad_init_io(void)
{
    MercuryTrace("vendor_keypad_init_io-------------------------->\r\n");

    sdk_reg_powerkey_shortpress_callback(vendor_powerkey_short_callback);
	sdk_reg_poweroff_callback(vendor_powerkey_long_callback);
    sdk_reg_amr_complete_callback(vendor_amr_complete_callback);
	qyy_reg_tts_complete_callback_event(vendor_tts_complete_callback);

    sdk_reg_key_short_cb(vendor_volume_down_short_press
                    ,vendor_volume_up_short_press
                    ,vendor_replay_short_press);
    
    sdk_reg_key_long_cb(vendor_volume_down_long_press,
                        vendor_volume_up_long_press,
                        vendor_replay_long_press);

    //Create keypad thread
    l_vendor_keypad_thread_handle = CreateThread(NULL,8*1024,vendor_keypad_scan_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);
    SetThreadPriority(l_vendor_keypad_thread_handle,OSI_PRIORITY_ABOVE_NORMAL);

    return 0;
}

int set_vendor_test_results(void)
{
    NV_Delete(NV_VENDOR_TEST_RESULTS);
    return NV_Write(NV_VENDOR_TEST_RESULTS,sizeof(VENDORTESTRESULTS),&l_testResults);
}

int read_vendor_test_results(void)
{
    return NV_Read(NV_VENDOR_TEST_RESULTS,sizeof(VENDORTESTRESULTS),&l_testResults);
}

int check_vendor_test_results(void)
{
    if (l_testResults.speakerTestResults == TRUE
        && l_testResults.keyTestResults == TRUE
        && l_testResults.gprsTestResults == TRUE
        && l_testResults.wifiTestResults == TRUE
        && l_testResults.ledTestResults == TRUE
        && l_testResults.usbTestResults == TRUE
        && l_testResults.bcsTestResults == TRUE
        ) {
        set_vendor_test_results();
        // // sdk_voiceplay(1, "��������ͨ��������Ը�λ����");
        return 0;
    } else {
        // // sdk_voiceplay(1, "��������δͨ������̰���λ�����²���");
        return -1;
    }
}

void vendor_mobile_network_thread(void)
{
    int gprs_network_status;
    while (1)
    {
        gprs_network_status = sdk_get_gprs_network_status();
        if(GPRS_NETWORK_STATUS_OK != gprs_network_status) {
            // set network to GPRS mode
            sdk_set_network_mode(NETWORK_MODE_GPRS);

            // GPRS initialization    
            sdk_network_gprs_init();
        }

        Sleep(3 * 1024);
    }
    
}

// void vendor_wifi_network_thread(void)
// {

// }

void vendor_usb_check(void)
{
    int ret;
    SYSTEM_POWER_STATUS_EX2 battery_status = {0};

    while (1)
    {
        ret = BatteryGetStatus(&battery_status,NULL);

        if( 0 != battery_status.ACLineStatus ){	// USB online
            break;
        } else {
            // sdk_voiceplay(1, "δ��⵽USB,�����USB");
        }

        Sleep(5000);
    }
}

int vendor_main(void)
{
    // vendor test

	if(LIFECYCLE_VENDOR == get_lifecycle())	// vendor mode
	{
        MercuryTrace("Inside LIFECYCLE_VENDOR----------------------------->\r\n");

        // mobile network
        CreateThread(NULL,4 * 1024,vendor_mobile_network_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);

        // wifi network
        // CreateThread(NULL,8 * 1024,vendor_wifi_network_thread,0,NULL,STACK_SIZE_RESERVATION,NULL);

        // read vendor test results
        read_vendor_test_results();
        if (0 != check_vendor_test_results()) 
        {
            //Set the factory mode volume
            AudioSetVolume(5);

            
            MercuryTrace("check_vendor_test_results() != 0 ----------------------------->\r\n");
            

            // sdk_voiceplay(1, "��ǰ���ڹ���ģʽ,����й������");//Serial number not written, please write serial number
            Sleep(5000);

            // create keypad thread
            MercuryTrace("Vendor keypad init io----------------------------->\r\n");
            vendor_keypad_init_io();
            
            // 1.���ȼ��
            MercuryTrace("Vendor speaker test 2----------------------------->\r\n");
            vendor_speaker_test2();

            // 2.�������
            MercuryTrace("Vendor key test----------------------------->\r\n");
            vendor_key_test();

            // 3.������
            // (1)GPRS���
            MercuryTrace("Vendor gprs test----------------------------->\r\n");
            vendor_gprs_test();
            // (2)WIFI���
            MercuryTrace("Vendor wifi test----------------------------->\r\n");
            vendor_wifi_test();

            // 4.LED���
            MercuryTrace("Vendor led test----------------------------->\r\n");
            vendor_led_test();

            // 5.USB���
            MercuryTrace("Vendor usb test----------------------------->\r\n");
            vendor_usb_test();

            // 6.bcs���
            MercuryTrace("Vendor bcs test----------------------------->\r\n");
            vendor_bcs_test();


            // �������Ƿ�ɹ�
            if (0 == check_vendor_test_results()) {
				set_restart_reason( E_RST_APPLY );
                // sdk_voiceplay(1, "��������ͨ��,����Ը�λ����");
            }
            else {
				set_restart_reason( E_RST_APPLY );
                // sdk_voiceplay(1, "��������δͨ��,��̰���λ�����²���");
            }

            MercuryTrace("Sleeping Forever Now----------------------------->\r\n");
            while(1)
            {
                Sleep(5 * 1000);
            }
        }
        
        // sdk_voiceplay(1, "����������");

        // 7.check USB
        // vendor_usb_check();

        // 8.д��SN��
        app_vendor_proc();
           
        MercuryTrace("Sleeping Forever Now 2----------------------------->\r\n");        

        while(1)
        {
            Sleep(5 * 1000);
        }
    }
    else
    {
        MercuryTrace("Inside LIFECYCLE_APP----------------------------->\r\n"); 
    }

	return 0;
}