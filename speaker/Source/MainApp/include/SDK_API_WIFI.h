#ifndef _QYY_API_WIFI_H
#define _QYY_API_WIFI_H


struct qyy_st_wifi_addr
{
	char sta_ip_addr[32];
	char sta_mac_addr[32];
	// ...
};


/************************************************
 * function  : start config network of ESP8266
 * parameter : timeout - airkiss timeout, unit(s)
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
typedef void(*GET_WIFI_CONNECT)( unsigned char *ssid, unsigned char *pwd, int state );
int sdk_start_airkiss( GET_WIFI_CONNECT connectCallback, int timeout );

int sdk_abort_airkiss( void );

/************************************************
 * function  : ESP8266 connect to AP
 * parameter : timeout - connect timeout, unit(s) 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_start_connect( unsigned char *ssid, unsigned char *pwd, GET_WIFI_CONNECT connectCallback, int timeout );

/************************************************
 * function  : get rssi of AP
 * parameter : 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_get_wifi_singnal( unsigned char *ssid, unsigned char *pwd, int *strength );

/************************************************
 * function  : get rssi of connected AP
 * parameter : 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_get_connected_wifi_singnal( BOOL *ap_connected, int *strength );

/************************************************
 * function  : get wifi list
 * parameter : 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
typedef enum
{
	ECN_TYPE_OPEN            = 0,
	ECN_TYPE_WEP             = 1,
	ECN_TYPE_WPA_PSK         = 2,
	ECN_TYPE_WPA2_PSK        = 3,
	ECN_TYPE_WPA_WPA2_PSK    = 4,
	ECN_TYPE_WPA2_Enterprise = 5,
}ECN_TYPE_E;
typedef struct
{
	ECN_TYPE_E ecn_type;
	unsigned char ssid[64];
	int rssi;
	unsigned char mac[18];
}AP_INFO_S;
int sdk_get_wifi_list( AP_INFO_S **ap_info, int *items );

/************************************************
 * function  : start softAP
 * parameter : ap_ssid - wifi name, timeout - unit(s)
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_start_AP( GET_WIFI_CONNECT connectCallback, unsigned char *ap_ssid, int timeout, const char *index_html );

int sdk_abort_AP( void );

/************************************************
 * function  : enable or disable wifi module, please call this function before System_Init
 * parameter : 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
void sdk_WIFI_ENABLE( BOOL enable );

/************************************************
 * function  : get esp8266 station mode MAC address
 * parameter : sta_mac - buffer
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_get_wifi_MAC( char *sta_mac );


/************************************************
 * function  : get esp8266 firmware version
 * parameter : wifi_firmware_version - buffer
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
typedef struct
{
	char *AT_version[32];
	char *SDK_version[32];
	char *compile_time[32];
	char *Bin_version[32];
}WIFI_FIRMWARE_VERSION_S;
int sdk_get_wifi_firmware_version( WIFI_FIRMWARE_VERSION_S *wifi_firmware_version );

/************************************************
 * function  : ping hostname by wifi
 * parameter : hostname - hostname or ip, time - time when ping success
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_wifi_ping( const char *hostname, int *time );

/************************************************
 * function  : ESP8266 enter deep sleep mode
 * parameter : 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_wifi_deep_sleep_mode( void );

/************************************************
 * function  : ESP8266 power mode set function
 * parameter : 
 * 		power_mode = 0;
 * 		power_mode = 1; 
 * 		power_mode = 2; 
 * return    : 0 indicate success, error code please refer to A20G_API_ERROR.h
 * **********************************************/
int sdk_wifi_setpowersavemode( int power_mode );

/************************************************
 * function  : ESP8266 wakeup
 * parameter : 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_wifi_wakeup( void );

/************************************************
 * function  : set default DNS server
 * parameter : 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_wifi_set_dns_server( BOOL def, char enable, const char *dns_server );

/************************************************
 * function  : wifi firmware update
 * parameter : 
 * return    : 0 indicate success, error code please refer to SDK_API_ERROR.h
 * **********************************************/
int sdk_wifi_firmware_update( void );
#endif
