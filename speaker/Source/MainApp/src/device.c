#include "MercuryNv.h"
#include "MercuryPlat.h"
#include "MercuryBattery.h"
#include "MercuryThread.h"

#include "device.h"
#include "nv.h"

extern BOOL g_status_fake_off;

int g_mercury_version = MERCURY_VERSION_UNKNOWN;

void set_restart_reason(int reset_reason)
{
	int ret = 0;

	if (0 != NV_Write(NV_RESET_REASON, sizeof(int), &reset_reason))
	{
		NV_Delete(NV_RESET_REASON);	// diff size
		ret = NV_Write(NV_RESET_REASON, sizeof(int), &reset_reason);
		MercuryTrace("write software reset fail... retry=%d \n", ret);
	}
}

void check_usb(void)
{
	SYSTEM_POWER_STATUS_EX2 battery_status = {0};

    while (1)
    {
		BatteryGetStatus(&battery_status,NULL);
		if( 0 == battery_status.ACLineStatus )// no usb
		{
			if(g_status_fake_off)
				PowerOff();
			else
				break;
		}
        
		Sleep(1000);
    }
	ExitThread(NULL);
}

void get_mercury_version(void)
{
    BYTE *mercury_version_compat = NULL;

    mercury_version_compat = GetMercuryVersion();
	MercuryTrace("mercury version %s\r\n", __FUNCTION__, mercury_version_compat);
    if (strstr(mercury_version_compat,AL35V300))
    {
        g_mercury_version = MERCURY_VERSION_AL35V300;
    }
    else if (strstr(mercury_version_compat,AL36V200))
    {
        g_mercury_version = MERCURY_VERSION_AL36V200;
    }
	else if (strstr(mercury_version_compat,AL36V203))
    {
        g_mercury_version = MERCURY_VERSION_AL36V203;
    }
	else if (strstr(mercury_version_compat,AL35V303))
    {
        g_mercury_version = MERCURY_VERSION_AL35V303;
    }
    else
    {
        g_mercury_version = MERCURY_VERSION_UNKNOWN;
	    MercuryTrace("Mercury Version Is not correct \r\n",__FUNCTION__);
    }
}

void app_keypad_init(void)
{
	if(g_mercury_version == MERCURY_VERSION_AL35V300)
    {
        MercuryTrace("Mercury Version - MERCURY_VERSION_AL35V300\r\n");
    }
    else if (g_mercury_version == MERCURY_VERSION_AL36V200)
    {
        MercuryTrace("Mercury Version - MERCURY_VERSION_AL36V200\r\n");
		keypad_init_io();
    }
    else if (g_mercury_version == MERCURY_VERSION_AL36V203)
    {
        MercuryTrace("Mercury Version - MERCURY_VERSION_AL36V203\r\n");
        keypad_init_io();
    }
	else if (g_mercury_version == MERCURY_VERSION_AL35V303)
    {
        MercuryTrace("Mercury Version - MERCURY_VERSION_AL35V303\r\n");
		keypad_init_io();
    }
    else
    {
        MercuryTrace("Keypad Init Matrix\r\n");
        keypad_init_matrix();

	    MercuryTrace("Mercury Version Is not correct \r\n",__FUNCTION__);
    }
}