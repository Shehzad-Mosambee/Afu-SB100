#ifndef _QYY_API_KEYPAD_H
#define _QYY_API_KEYPAD_H

#include "MercuryDef.h"
#include "SDK_API_SYS.h"

typedef enum
{
	KEY_PRESS = 0,        
	KEY_RELEASE = 1,      
}enum_key_state;

uint32 sdk_Get_key_up_state( void );                     
uint32 sdk_Get_key_down_state( void );                    
uint32 sdk_Get_key_replay_state( void );                 

void sdk_keypad_scan_handle(void);
void sdk_reg_key_short_cb(QYY_GPIO_KEY_CB vu, QYY_GPIO_KEY_CB vd, QYY_GPIO_KEY_CB rep);
void sdk_reg_key_long_cb(QYY_GPIO_KEY_CB lvu, QYY_GPIO_KEY_CB lvd, QYY_GPIO_KEY_CB lrep, QYY_GPIO_KEY_CB lvu_lrep);
void sdk_reg_key_double_cb(QYY_GPIO_KEY_CB vu_db,	QYY_GPIO_KEY_CB vd_db, QYY_GPIO_KEY_CB rep_db);

#endif
