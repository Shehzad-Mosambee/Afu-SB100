#include "MercuryDef.h"
#include "MercuryPlat.h"
#include "MercuryCSTD.h"
#include "MercuryKeypad.h"
#include "MercuryThread.h"
#include "MercuryTimer.h"
#include "MercuryBattery.h"

#include "SDK_API_AUDIO.h"
#include "SDK_API_POWER.h"
#include "SDK_API_SYS.h"
#include "SDK_API_KEYPAD.h"

#include "nv.h"

#include "keypad.h"
#include "audio.h"
//#include "gpio.h"

#include "device.h"
#include "speakerapp.h"

#include "global.h"


uint32 app_power_percent = 0;
int low_power_voice_warning = 0;
HANDLE g_event_batlow_poweroff;
HANDLE g_event_batlow_warning;

extern BOOL g_status_fake_off;

extern void mqtt_soundbox_close_thread();

static void *_app_charger_in_notify(void *arg)
{
	// // app_audio_play(AUDIO_CHARGER_IN);
	// sdk_audio_play_nv(AUDIO_CHARGING+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_CHARGING));
	play_audio_by_name("char", (unsigned char)0);

	sdk_light_control(LED_RED);
}
static void _ACTION_CHARGING_OFF(void)
{
	sdk_light_control(LED_BLUE);
}

//������γ�
static void *_app_charger_out_notify(void *arg)
{
	sdk_light_control(LED_BLUE);
	// app_audio_play(AUDIO_CHARGER_OUT);
	
	return NULL;
}

void qyy_led_charge_control(int percent)
{
	if( g_status_fake_off ){
		if(percent > 95){
			sdk_light_control(LED_GREEN);
		}else{
			sdk_light_control(LED_RED);

		}
	}
}

////��ص���̫�ͣ��ػ�
void batlow_poweroff(void)
{
	// app_audio_play(AUDIO_BAT_POWEROFF);
	// app_audio_play(AUDIO_BAT_POWEROFF2);

	
	// sdk_audio_play_nv(AUDIO_BAT_LOW_SD+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_BAT_LOW_SD));
	// sdk_audio_play_nv(AUDIO_SHUT_MESSAGE+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SHUT_MESSAGE));
	play_audio_by_name("lows", (unsigned char)0);
	// play_audio_by_name("shutMsg", (unsigned char)0);

	// sdk_audio_play_nv(AUDIO_SHUTTING_DOWN+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_SHUTTING_DOWN));
	set_restart_reason( E_RST_UNKNOWN );

	Sleep(8000);
	PowerOff();
}

//��ص����ͣ���������
void batlow_warning(void)
{
	// // app_audio_play(AUDIO_BAT_WARNING);
	// sdk_audio_play_nv(AUDIO_BAT_LOW_PC+NV_AUDIO_START_INDEX, get_nv_audio_data_len(AUDIO_BAT_LOW_PC));
	play_audio_by_name("lowc", (unsigned char)0);
}

//�����ϵ磬��ѯ��ص���
void app_poweron_check_battery(void)
{
	/* Battery Power On Detection */
	SYSTEM_POWER_STATUS_EX2 battery_status = {0};
	int val = app_get_battery_percent();

	BatteryGetStatus(&battery_status,NULL);
	if(battery_status.ACLineStatus == 0)
	{
		if (val < BAT_POWER_OFF)
		{
			batlow_poweroff();
			return;
		}
		else if( val < BAT_AUDIO_WARNING )
		{
			batlow_warning();
		}
	}
	else
	{
		qyy_led_charge_control(val);
	}
}

//��ѯ��ص�����״̬
void app_check_low_power(void)
{
	/* Battery Power On Detection */
	SYSTEM_POWER_STATUS_EX2 battery_status = {0};
	int val = app_get_battery_percent();
	char need_warn = 0;
	
	if( 0 == BatteryGetStatus(&battery_status,NULL) )
	{		
		if(battery_status.ACLineStatus == 0)
		{	// USB off
			if (val < BAT_POWER_OFF)
			{
				MercuryTrace("bat low to power off\n ");
				SetEvent(g_event_batlow_poweroff);
				return;
			}
			else if( val < BAT_AUDIO_WARNING15 )
			{
				if( 0 == low_power_voice_warning & 0x4 )
				{
					low_power_voice_warning |= 0x4;
					need_warn = 1;
				}
			}
			else if( val < BAT_AUDIO_WARNING20 )
			{
				if( 0 == low_power_voice_warning & 0x2 )
				{
					low_power_voice_warning |= 0x2;
					need_warn = 1;
				}
			}
			else if( val < BAT_AUDIO_WARNING )
			{
				if( 0 == low_power_voice_warning & 0x1 )
				{
					low_power_voice_warning |= 0x1;
					need_warn = 1;
				}
			}

		}
		else
		{					// USB on
			qyy_led_charge_control(val);
		}
	}

	if(need_warn)
	{
		MercuryTrace("bat low to warn\n ");
		SetEvent(g_event_batlow_warning);
	}
}

static void app_power_monitor_cb(ULONG arg)
{
	SYSTEM_POWER_STATUS_EX2 battery_status = {0};

	if (0 == BatteryGetStatus(&battery_status, NULL))
	{
		if (1 == battery_status.ACLineStatus)
		{ // USB online
			qyy_led_charge_control(app_get_battery_percent());
		}
		else
		{
			app_check_low_power();
		}
	}
}

void app_start_power_monitor(void *arg)
{
	TIMER_CONFIG_S cfg_timer;
	HANDLE hd_power_monitor = NULL;

	app_poweron_check_battery();

	g_event_batlow_poweroff = CreateEvent(NULL,TRUE,FALSE, "G_EVENT_BATLOW_QUEUE");
	g_event_batlow_warning = CreateEvent(NULL,TRUE,FALSE, "G_EVENT_BATWARN_QUEUE");

	app_power_percent = app_get_battery_percent();

	if(NULL != hd_power_monitor)
	{
		if(IsTimerActive(hd_power_monitor))
		{
			DeactiveTimer(hd_power_monitor);
		}

		DeleteTimer(hd_power_monitor);

		hd_power_monitor = NULL;
	}

	cfg_timer.timer_name = "APP_POWER_MONITOR";
	cfg_timer.timer_fun = app_power_monitor_cb;
	cfg_timer.input = (ULONG)arg;
	cfg_timer.timer_expire = 30 * 1000;	// ms
	cfg_timer.period = TIMER_PERIOD;	// cycle mode
	cfg_timer.auto_activate = TIMER_AUTO_ACTIVATE;

	hd_power_monitor = CreateTimer(&cfg_timer);    

	
	struct st_sdk_ps_notify notify={NULL, 
		                            _app_charger_in_notify, 
		                            _app_charger_out_notify};
	/* Charge notify */
	sdk_powersave_notify_reg_cb(&notify);
}



void PowerOffRebootDevice(int bPower)
{
	MercuryTrace("PowerOffRebootDevice-------------------------->\r\n");
    CreateThread(NULL, 4*1024, mqtt_soundbox_close_thread, 0, NULL, STACK_SIZE_RESERVATION, NULL);

    play_audio_by_name("shut", (unsigned char)0);
    Sleep(2000);

    // NLedDeinit(LED_INDICATION);
    sdk_light_control(LED_OFF);
    Sleep(3000);
	
    appInfoSave();	
	app_save_trans_record();

	// set_restart_reason( E_RST_UNKNOWN );

	if(!bPower)
	{
		set_restart_reason( E_RST_UNKNOWN );

		Sleep(1 * 1000);
		PowerOff();
	}
	else
	{
		set_restart_reason(E_RST_REBOOT);

		Sleep(1 * 1000);
		PowerReboot();
	}

    // sdk_power_off(NULL);
    
	return;
}
