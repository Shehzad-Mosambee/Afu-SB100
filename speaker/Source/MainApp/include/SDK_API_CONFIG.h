/*****
 * SDK_API_CONFIG.h
 * Init 2020.11.13 wangjianyu@qiyinyi.com
 */
#ifndef _QYY_API_CONFIG_H_
#define _QYY_API_CONFIG_H_

#include "sdk_version.h"

/******************************
 * APP & LIB
 ******************************/

/* Debug func */
#ifndef zpi_log
#define zpi_log(f,...)  do{MercuryTrace("\e[1;33m[%s,%d] ", __func__,__LINE__);MercuryTrace(f, ##__VA_ARGS__);MercuryTrace("\e[0m");}while(0)
#endif
#ifndef zpi_lgg
#define zpi_lgg(f,...)  do{MercuryTrace("\e[1;34m[%s,%d] ", __func__,__LINE__);MercuryTrace(f, ##__VA_ARGS__);MercuryTrace("\e[0m");}while(0)
#endif
#ifndef zpi_loo
#define zpi_loo(f,...)  do{MercuryTrace("\e[1;35m[%s,%d] ", __func__,__LINE__);MercuryTrace(f, ##__VA_ARGS__);MercuryTrace("\e[0m");}while(0)
#endif
//#include "cs_types.h"
#define LOG_DLTEMP		0x8
#define LOG_DLDEBUG		0x4
#define LOG_DLRELEASE		0x2
#include "cs_types.h"
extern u32 qyy_log_level;
#define zpi_logdl(type, format, ...)	do{ \
	if(type<=qyy_log_level){ \
		zpi_loo(format, ##__VA_ARGS__); \
	}; \
}while(0)
//#define zpi_logdl(t, f, ...)	do{MercuryTrace("\e[1;35m[%s,%d] ", __func__,__LINE__);MercuryTrace(f, ##__VA_ARGS__);MercuryTrace("\e[0m");}while(0)

#define DL_ABS(x,y)	(((x)>(y))?((x)-(y)):((y)-(x)))
#define DL_SIZE_MIN(x,y)        ((x)>(y)?(y):(x))


/* Print timestamp */
#if 0	// 1:enable
#define PRINT_TIME_COUNT()	do{ \
	ULONG tick; \
	GetTickCount(&tick); \
	MercuryTrace("%16s,%4d, %ld\n", __func__, __LINE__, tick); \
}while(0)
#else
#define PRINT_TIME_COUNT()	{}
#endif


/* SOCK_RECV_RET_TEMP_VER
 * In-func used macro
 * 1 : return 0 if socket connect closed; 
 * 0 : return RECV_SOCKET_CLOSE if socket connect closed;
 */
#define QYY_SOCK_RECV_RET_TEMP_VER 1


#define QYY_HTTP_RW_DEBUG_SW	0

/* HTTP PORT NUM */
//#define QYY_HTTP_MANUAL_HTTP_PORT_ENABLE	0
//#define QYY_HTTP_MANUAL_HTTP_PORT_NUM		80
/* HTTPS PORT NUM */
//#define QYY_HTTP_MANUAL_HTTPS_PORT_ENABLE	0
//#define QYY_HTTP_MANUAL_HTTPS_PORT_NUM		443


/* Debug print */
#define QYY_DEBUG_WIFI_INIT_AT	0


#if 0
/* Debug print switch of function - "gprs_nonblock_recv" */
extern int debug_sw_socket_recv_gprs = 0;
debug_sw_socket_recv_gprs = 1;
#endif


#if 0
/* Show QYY_SDK compilation info 
 * wangjianyu@qiyinyi.com
 */
extern const char *QYY_SDK_DATE;
extern const char *QYY_SDK_TIME;
zpi_log("SDK Compile %s %s\n", QYY_SDK_DATE, QYY_SDK_TIME);
zpi_log("APP Compile %s %s\n", __DATE__, __TIME__);
#endif


/*
 * WIFI UART BAUD RATE CONTROL
 * 1 : 115200
 * 0 : 19200
 */
#define QYY_WIFI_UART_SPEED_HIGH	1



/******************************
 * LIB & APP(TODO)
 ******************************/

/*
 * OTA mode
 *
 * Init
 * 	FOTA_Init
 * 	parser_field_value
 * 	FotaUpgrade_Init
 *
 * Write
 * 	FOTA_FlashWrite
 * 	FotaUpgrade_Write
 *
 * Finish
 * 	FOTA_ImgInfoSet
 * 	FotaUpgrade_Start
 */
#define QYY_FOTA_TYPE_FULL      0x8
#define QYY_FOTA_TYPE_DIFF      0x4
#define QYY_FOTA_TYPE_DEFAULT	QYY_FOTA_TYPE_FULL	// default mode: full
// [qyy_fota_download_http,107] OTA init!
// [recv_cb,63] OTA write!
// [qyy_fota_update,303] OTA set img!

/****
 * OTA DOWNLOAD RETRY
 * 2021.01.15, wangjianyu@qiyinyi.com
 * 	Init;
 * 	Disable by default !!!
 */
#define QYY_OTA_DOWNLOAD_RETRY_ENABLE   0		// Switch, 1: enable retry;
#define QYY_OTA_DOWNLOAD_RETRY_TIMES    4		// MAX retry times, 0:Disable;
#define QYY_OTA_DOWNLOAD_RETRY_INTERVAL_MS	(5000)	// Retry time interval in ms;


/*
 * _set_led_gpio_high
 * _set_led_gpio_low
 */
#define QYY_SET_GPIO_LEVEL_CONFIG_AGAIN 0

/*
 * System auto sleep
 */
#define QYY_SDK_WAKELOCK_ENABLE_SE	0
#define QYY_SDK_WAKELOCK_ENABLE_APP	0
#define QYY_SDK_WAKELOCK_ENABLE_WIFI	1	// default:1, need test if changed
#define QYY_SDK_WAKELOCK_ENABLE_SCREEN	0	
/* avoid bug - v1.9.6
* 2020/01/07 
* AL36V200_V1.9.7, set to 0;
 */




/* 
 * QYY POWER SAVE - GPIO KEY (Interrupt) 
 * wangjianyu@qiyinyi.com
 * For V6-QYY
 */
#define QYY_GPIO_KEY_EVENT_TIMEOUT_NEVER	0xffffffff
#define QYY_GPIO_KEY_EVENT_TIMEOUT_TIME_DEFAULT	QYY_GPIO_KEY_EVENT_TIMEOUT_NEVER
//#define QYY_GPIO_KEY_EVENT_TIMEOUT_TIME_DEFAULT	30000


/*
 * PA - AMOI
 */
#define QYY_SDK_AMOI_PA_SET_ENABLE	1	// 0:disable, Else:enable;
#define QYY_SDK_AMOI_PA_TYPE    AUD_SPKPA_TYPE_CLASSD	// Amoi default
//#define QYY_SDK_AMOI_PA_TYPE	AUD_SPKPA_TYPE_CLASSAB	// 0.5w


/*
 * HTTP RANGE
 * define / not
 * Depend on MERCURY_PLATFORM
 *
 * Control : 
 * 	extern int qyy_http_use_range;	// 1:use range; 0:no range;
 *
 */
#define DL_HTTP_SUPPORT_RANGE	1		// define: [enable] http range; WARNING: ref var qyy_http_use_range!
#define DL_HTTP_RANGE_DEFAULT_VALUE	(1)	// default: enable
#define DL_HTTP_RANGE_BLOCK_DEFAULE_SIZE	(10240)
/* Check dependency */
//#ifdef DL_HTTP_SUPPORT_RANGE
//#ifndef MERCURY_PLATFORM
//#error depend!
//#endif
//#endif



#endif
