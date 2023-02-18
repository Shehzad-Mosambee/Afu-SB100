#ifndef _QYY_API_POWER_H
#define _QYY_API_POWER_H

#include "MercuryDef.h"
#define BAT_CAPACITY_STEP  11

/*****************************************************
 *function  : system shutdown
 *parameter :
 *return    :
 * ***************************************************/
void sdk_power_off( const char *param );

/*****************************************************
 *function  : system restart
 *patameter :
 *return    :
 * ***************************************************/
int sdk_system_restart( const char *param );

/*****************************************************
 *function  : get poweron type
 *patameter :
 *return    :
 * ***************************************************/
typedef enum
{
	POWERON_TYPE_USB = 0,                  //poweron by USB
	POWERON_TYPE_POWERKEY = 1,             //poweron by powerkey
}POWERON_TYPE_E;
POWERON_TYPE_E sdk_get_poweron_type( void );

/***************************************************
 *function  : register powerkey short press callback
 *parameter : 
 *return    :
 * *************************************************/
void sdk_reg_powerkey_shortpress_callback( int powerkey_shortpress_callback( void ) );

/***************************************************
 *function  : register poweroff callback, function poweroff_callback() will be called before poweroff. if poweroff_callback() return 0 then poweroff, else not poweroff
 *parameter : 
 *return    :
 * *************************************************/
void sdk_reg_poweroff_callback( int poweroff_callback( void ) );

#endif
