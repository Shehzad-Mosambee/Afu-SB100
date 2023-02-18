#ifndef _MC_WIFISCAN_H_
#define _MC_WIFISCAN_H_

/*******************************************************************************
**	MACRO
********************************************************************************/
#define WIFI_SCAN_MAX_TIME (65000)                 /*Uint: ms*/
#define WIFI_SCAN_DEF_TIME (12000)
#define WIFI_SCAN_MIN_TIME (4000)
#define WIFI_SCAN_MAX_TIMEOUT (5000)               /*Uint: ms*/
#define WIFI_SCAN_DEF_TIMEOUT (600)
#define WIFI_SCAN_MIN_TIMEOUT (120)
#define WIFI_SCAN_MAX_AP_CNT (30)
#define WIFI_SCAN_DEF_AP_CNT (5)
#define WIFI_SCAN_MIN_AP_CNT (4)
#define WIFI_SCAN_MAX_ROUND (3)
#define WIFI_SCAN_DEF_ROUND (1)
#define WIFI_SCAN_MIN_ROUND (1)


/*******************************************************************************
**  Type definition
********************************************************************************/
typedef struct {
	uint32_t time;
    uint8_t  round;
    uint8_t  maxbssidnum;
    uint32_t scantimeout;
    uint8_t  priority;
} wScanParam_t;

typedef enum
{
    WIFI_SCAN_ERR_SUCCESS = 0,
    WIFI_SCAN_ERR_SCAN_END,     
    WIFI_SCAN_ERR_OPEN_FAIL,
    WIFI_SCAN_ERR_NO_MEMORY,
    WIFI_SCAN_ERR_TIMEOUT,
    WIFI_SCAN_ERR_MAX
} WIFI_SCAN_STATUSCODE;

/************************************************
start wifiscan
param[in]wScanParam
return : 0 success, the scan result is callbacked by MercuryNotifyCallback

WIFI SCAN DATA example:
0: (-,-,-96,"00:19:3B:0C:D5:3E",11) 
1: (-,-,-83,"2C:B2:1A:E7:22:F2",4)
2: (-,-,-81,"FA:8C:21:AF:AC:17",11)
3: (-,-,-81,"10:12:B4:21:8C:91",3)
num: (ssid,authmode,rssi, "mac",channel)
*************************************************/
int WifiScan_Start(wScanParam_t *wScanParam);


#endif


