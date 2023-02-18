#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryTimer.h"

#include "SDK_API_MQTT.h"
#include "SDK_API_AUDIO.h"
#include "SDK_API_SYS.h"
#include "SDK_API_TCP.h"
#include "SDK_API_DISPLAY.h"

#include "cJSON.h"

#include "audio.h"
#include "device.h"
// #include "connect_mqtt_server.h"
#include "nv.h"

#include "speakerapp.h"
#include "string.h"

static NvQrcodeCfgStruct g_qrcode_cfg = {0};
// HANDLE g_event_update_qr;
HANDLE hd_qr_update = NULL;
HANDLE hd_bar_update = NULL;


// ��ʾͼ��--������ʾ--Ӧ���Ǻ�������ȷ�ϣ���4G��WiFiͼ���������״̬��
void app_display_statusbar(void)
{
	sdk_disp_init(LCD_TYPE_ST7789);
	DisplaySetDirection(DMDO_270);

	DisplayStatusBar statusbar = {0, 0, 24, 0, 48, 0};
	uint32 battery_percent;
	SYSTEM_POWER_STATUS_EX2 battery_status = {0};
	int gggg = sdk_get_gprs_network_status();
	int wifi = sdk_get_wifi_init_OK();
	int netstatus = sdk_get_network_mode();
	int pingtime = 5 * 1000;

	sdk_set_status_bar(&statusbar);
	// sdk_disp_clear_area(0, 0, 100, 24);
	sdk_disp_clear_area(0, 0, 320, 24);

	//��ʾʱ��
	app_display_time();

	MercuryTrace(".................app_display_statusbar................\n");
	// ��ʾ���������

	int retVal = BatteryGetStatus(&battery_status, NULL);
	MercuryTrace("BatteryGetStatus() retVal = %d\r\n", retVal);
	if (0 == retVal)
	{
		if (battery_status.ACLineStatus == 1)
		{
			sdk_disp_battery(0xFF);
		}
		else if (battery_status.ACLineStatus == 0)
		{
			battery_percent = app_get_battery_percent();
			if (battery_percent > 99)
			{
				sdk_disp_battery(4);
			}
			else
			{
				sdk_disp_battery(battery_percent / 20);
			}
		}
	}

	if (netstatus == NETWORK_MODE_GPRS)
	{
		if (GPRS_NETWORK_STATUS_OK == gggg)
		{
			// ��ʾ4G�ź�ǿ��
			BYTE rssi;
			WORD rxfull;

			NetworkGetSignalQuality(&rssi, &rxfull);
			if (rssi >= 31)
				sdk_disp_mobile_signal(4);
			else if (rssi >= 2)
				sdk_disp_mobile_signal(3);
			else
				sdk_disp_mobile_signal(rssi);
		}
		else
		{
			sdk_disp_mobile_signal(1);
		}
	}
	else if (netstatus == NETWORK_MODE_WIFI)
	{
		// if (TRUE == wifi && 0 == sdk_wifi_ping("114.114.114.114", &pingtime))
		if (TRUE == wifi && 0 == sdk_wifi_ping("8.8.8.8", &pingtime))
		{
			// ����3��WIFI�ź�
			sdk_disp_wifi_signal(3);
		}
		else
		{
			sdk_disp_wifi_signal(1);
		}
	}
}

static void app_statusbar_update_cb(ULONG arg)
{
	app_display_statusbar();
	// MercuryTrace("\n..$....app_statusbar_update_cb....\n");

}

//����״̬��ˢ�¶�ʱ��
void app_start_statusbar_update(void)
{
	TIMER_CONFIG_S cfg_timer;
	DisplayParams dispParam;

	sdk_disp_init(LCD_TYPE_ST7789);
	DisplaySetDirection(DMDO_270);

	cfg_timer.timer_name = "APP_UPDATE_BAR";
	cfg_timer.timer_fun = app_statusbar_update_cb;
	cfg_timer.input = 0;
	cfg_timer.timer_expire = 10 * 1000; // ˢ��Ƶ��
	cfg_timer.period = TIMER_PERIOD;	// cycle mode
	cfg_timer.auto_activate = TIMER_AUTO_ACTIVATE;

	hd_bar_update = CreateTimer(&cfg_timer);
}

static void app_qrcode_update_cb(ULONG arg)
{
	// SetEvent(g_event_update_qr);
	// app_qrcode_update_handle();
	MercuryTrace("\n..$....app_qrcode_update_cb....\n");

}

void app_start_qrcode_update(int gap)
{
	
	TIMER_CONFIG_S cfg_timer;

	app_qrcode_update_handle();


	cfg_timer.timer_name = "APP_UPDATE_QR";
	cfg_timer.timer_fun = app_qrcode_update_cb;
	cfg_timer.input = 0;
	cfg_timer.timer_expire = gap * 1000; // ms
	cfg_timer.period = TIMER_PERIOD;	 // cycle mode
	cfg_timer.auto_activate = TIMER_AUTO_ACTIVATE;

	hd_qr_update = CreateTimer(&cfg_timer);

}

char sDeviceName[64] = {0};
int app_qrcode_cfg_init(void)
{
	int ret = -1;
	AppInfoStruct info = {0};

	NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&info);
	strcpy(sDeviceName, info.SN);

	//��ʱ����
	// strcpy(sDeviceName, "cs_0001");

	ret = NV_Read(NV_QRCODE_CFG, sizeof(NvQrcodeCfgStruct), (BYTE*) &g_qrcode_cfg);
	// strcpy(g_qrcode_cfg.url, "https://www.qiyinyi.cn:8150/iot/devices/qr-code/dynamic");
	// g_qrcode_cfg.gap = 60;
	MercuryTrace("\n code :%s \n \n gap :%d\n",g_qrcode_cfg.url,g_qrcode_cfg.gap);

	if (NULL != hd_bar_update)
	{
		if (IsTimerActive(hd_bar_update))
		{
			DeactiveTimer(hd_bar_update);
		}
		DeleteTimer(hd_bar_update);
		hd_bar_update = NULL;
	}
	
	app_start_statusbar_update();

	if (ret == 0)
	{
		if(NULL != hd_qr_update)
		{
			if(IsTimerActive(hd_qr_update))
			{
				DeactiveTimer(hd_qr_update);
			}
			DeleteTimer(hd_qr_update);
			hd_qr_update = NULL;
		}

		app_start_qrcode_update(g_qrcode_cfg.gap);
	}
}

int app_display_time(void){

	//��ʾʱ��--ʱ�䲻�ԣ�00:00��ʼ��
	SYSTEMTIME sys_time;
	char szBuf[64] = {0};
	// POINT pt = {140, 0};
	POINT pt = {140, 10};
	GetLocalTime(&sys_time);	//SetLocalTimeʧ�� 
	sprintf(szBuf, "%.2d:%.2d", (int)(sys_time.wHour) + 8, sys_time.wMinute); //��������Ӱ��
	
	MercuryTrace("Time: %s ------------------------------->\r\n", szBuf);
	
	
	DisplayInit();
	DisplaySetColor(MERCURY_BLACK, MERCURY_WHITE);
	DisplayString(&pt, szBuf);
	DisplayPaintEnd();
}

int app_qrcode_update_handle(void)
{
	char rsp_buff[1024];
	char stime_buff[20] = {0};
	int recv_len = 0;
	// RECTL qrRect = {64, 24, 216, 256};
	RECTL qrRect = {60, 24, 212, 256};
	// RECTL qrRect = {60, 20, 220, 260};
	// RECTL qrRect = {15, 60, 220, 260}; //΢��һ��λ��
	char sSend[128] = {0};
	cJSON *root = NULL, *code = NULL;

	sprintf(sSend, "{\"deviceName\":\"%s\"}", sDeviceName);
	MercuryTrace("qr url:%s, body:%s\n", g_qrcode_cfg.url, sSend);
	sdk_http_get(g_qrcode_cfg.url, sSend, NULL, rsp_buff, 1024, &recv_len);
	MercuryTrace("...........................qrcode rsp:%d\n", recv_len);
	if (recv_len == 0)
	{
		MercuryTrace("get update qrcode fail\n");
		return -1;
	}
	MercuryTrace("...........................qrcode rsp:%s\n", rsp_buff);
	root = cJSON_Parse(rsp_buff);
	if (root == NULL)
		return -1;

	code = cJSON_GetObjectItem(root, "code");
	if (code && (code->valueint == 0))
	{
		cJSON *qr = NULL;
		cJSON *stime = NULL;
		//��ʾ������̬��ά��
		AppInfoStruct info = {0};
		NV_Read(NV_SYS_INFO, sizeof(AppInfoStruct), (BYTE *)&info);
		char URL[128] = {0};
		sprintf(URL, "http://101.132.112.165:10081/szanfu-admin/sn/check?sn=%s", info.SN);

		// sdk_disp_qrcode(&qrRect, URL, strlen(URL));
/*
		//��ȡ������ʱ��
		stime = cJSON_GetObjectItem(root,"timeStamp");
		MercuryTrace("timeStamp = [%s]\n",stime->valuestring);
		// strcpy(stime_buff,stime->valuestring);
		// MercuryTrace("stime_buff = [%s]\n",stime_buff);
		SYSTEMTIME sys_time;
		char szBuf[64] = {0};
		char hour[2] ={0};
		char minute[2] ={0};
		sys_time.wHour = (WORD)atoi(strncpy(hour,stime->valuestring+11,2));
		sys_time.wMinute = (WORD)atoi(strncpy(minute,stime->valuestring+14,2));
		MercuryTrace("sys_time.wHour:%d\n  sys_time.wMinute :%d\n", sys_time.wHour,sys_time.wMinute);
		sprintf(szBuf, "%.2d:%.2d", sys_time.wHour, sys_time.wMinute);
		MercuryTrace("time:%s\n", szBuf);
		SetLocalTime(&sys_time);
		MercuryTrace("SetLocalTime is ok ? = [%d]\n",SetLocalTime(&sys_time));

		//��ʾʱ��
		POINT pt = {140, 0};
		DisplaySetColor(MERCURY_BLACK, MERCURY_WHITE);
		DisplayString(&pt, szBuf);
		DisplayPaintEnd();
*/

		qr = cJSON_GetObjectItem(root, "payload");
		MercuryTrace("cJSON_GetObjectItem();[%s]\n",qr);
		// ���ַ�����ʾΪ��ά��
		sdk_disp_qrcode(&qrRect, qr->valuestring , strlen(qr->valuestring));
	}
	cJSON_Delete(root);

	return 0;
}
