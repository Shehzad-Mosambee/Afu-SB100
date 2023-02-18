#ifndef __SDK_API_BCS_DISPLAY_H__
#define __SDK_API_BCS_DISPLAY_H__

#include "MercuryDef.h"

typedef enum {
    GPRS_ICON = 0,
    WIFI_ICON
}BCS_ICON;

/**
 * bcs init
*/
void sdk_bcs_init(void);

/**
 * set bcs number
 * 
 * number:  0 ~ 999999
 * bit:     0	无小数点
 *          1 	1位小数
 *          2	2位小数
 * return:  0 success
*/
int sdk_bcs_number(int number, int bit);
int sdk_bcs_character(char *str);

/**
 * set bcs rssi
 * 
 * channel:     network status
 *              using BCS_ICON
 * bconnect:    connection status
 *              true false
 * return:  0 success
*/
int sdk_bcs_set_rssi(BCS_ICON channel, BOOL bConnect);

/**
 * set speaker status
 * 
 * bPlaying:    speaker status
 *              true false
 * return:  0 success
*/
int sdk_bcs_set_speaker(BOOL bPlaying);

/**
 * set bluetooth status
 * 
 * bDisIocn:    bluetooth icon display
 *              true false
 * 
 * bPlaying:    bluetooth status
 *              true false
 * return:  0 success
*/
int sdk_bcs_set_bluetooth(BOOL bDisIocn, BOOL bPlaying);

#endif