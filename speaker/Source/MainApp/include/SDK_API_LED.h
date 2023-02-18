#ifndef _SDK_API_LED_H
#define _SDK_API_LED_H

#include "MercuryDef.h"

typedef enum
{
	LED_OFF   = 0,
	LED_RED   = 1 << 0,
	LED_GREEN = 1 << 1,
	LED_BLUE  = 1 << 2,
}LED_NUM_E;

int sdk_light_control( LED_NUM_E led_num );
int sdk_blink_control( LED_NUM_E ledNum, DWORD nBrightTime,DWORD nDestroyTime);

#endif
