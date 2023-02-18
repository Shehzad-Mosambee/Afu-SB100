/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author:
#########################################################################*/

#ifndef __MERCURY_PLAT_H__
#define __MERCURY_PLAT_H__
#include "MercuryDef.h"
/**************************************************************************
消息通知模式
app注册 NotifyCallback 函数，平台将通过NotifyCallback通知app各种消息
消息结构
    消息类型 + 消息ID
*************************************************************************/
typedef struct
{
    WORD NotifyClass;
    WORD NotifyID;
}NOTIFY_ID,*PNOTIFY_ID;

typedef enum
{
    NOTIFY_CLASS_PDP = 0,
    NOTIFY_CLASS_SMS,
    NOTIFY_CLASS_TEL,
    NOTIFY_CLASS_SOCKET,
    NOTIFY_CLASS_DNS,
    NOTIFY_CLASS_CHARGE,
    NOTIFY_CLASS_SCREEN,
    NOTIFY_CLASS_POWER,
    NOTIFY_CLASS_BARSCAN,
    NOTIFY_CLASS_TTS,
    NOTIFY_CLASS_SYSTEM,
    NOTIFY_CLASS_AUDIO,
    NOTIFY_CLASS_STK,
    NOTIFY_CLASS_PBK,
    NOTIFY_CLASS_HTTP,
	NOTIFY_CLASS_SIM,	
	NOTIFY_CLASS_NW,
    NOTIFY_CLASS_NTP,
    NOTIFY_CLASS_PWM,
    NOTIFY_CLASS_WIFISCAN,
    NOTIFY_CLASS_MAX,
    MERCURY_ENUM_NOTIFYCLASS_MAX = 0x7fffffff
}NOTIFY_CLASS_E;


typedef enum
{
    NOTIFY_ID_ACT_SUCESS = 0,
    NOTIFY_ID_ACT_FAILE,
    NOTIFY_ID_DEACT_SUCESS,
    NOTIFY_ID_DEACT_FAILE,
    NOTIFY_ID_DEACT_BY_NET,
    NOTIFY_ID_ATTACH_SUCCESS,
    NOTIFY_ID_ATTACH_FAILE,
    NOTIFY_ID_DEATTACH_SUCCESS,
    NOTIFY_ID_DEATTACH_FAILE,
    NOTIFY_ID_DEATTACH_BY_NET,
    NOTIFY_ID_ACT_DELAY_RETRY,
    NOTIFY_ID_QOS_CNF_SUCCESS,
    NOTIFY_ID_QOS_CNF_FAIL,
    NOTIFY_ID_MIN_QOS_CNF_SUCCESS,
    NOTIFY_ID_MIN_QOS_CNF_FAIL,
    MERCURY_ENUM_PADPNOTIFY_MAX = 0x7fffffff
}PDP_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_SEND_SUCESS= 0,
    NOTIFY_ID_SEND_FAILE,
    NOTIFY_ID_READ_TEXT,
    NOTIFY_ID_READ_PDU,
    NOTIFY_ID_RCV_SMS,
    NOTIFY_ID_SMS,
    MERCURY_ENUM_SMSNOTIFY_MAX = 0x7fffffff
}SMS_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_CALL_START=0,
    NOTIFY_ID_INCOMING_RING,
    NOTIFY_ID_REMOTE_HANG,
    NOTIFY_ID_REMOTE_BUSY,
    NOTIFY_ID_REMOTE_NO_ANSWER,
    NOTIFY_ID_CALL_CONNECT,
    NOTIFY_ID_CALL_HANGUP,
    NOTIFY_ID_TEL,
    MERCURY_ENUM_TELNOTIFY_MAX = 0x7fffffff
}TEL_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_SOCKET_CONNECT = 0,
    NOTIFY_ID_SCOKET_READ,
    NOTIFY_ID_SCOKET_WRITE,
    NOTIFY_ID_SCOKET_CLOSE,
    NOTIFY_ID_SOCKET_FULLCLOSE,
    NOTIFY_ID_TLSSOCKET_CONNECT=5,
    NOTIFY_ID_TLSSCOKET_READ,
    NOTIFY_ID_TLSSCOKET_WRITE,
    NOTIFY_ID_TLSSCOKET_CLOSE,
    NOTIFY_ID_TLSSOCKET_FULLCLOSE,
    NOTIFY_ID_SCOKET,
    MERCURY_ENUM_SOCKETNOTIFY_MAX = 0x7fffffff
}SOCKET_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_DNS_SUCESS= 0,
    NOTIFY_ID_DNS_FAILE,
    NOTIFY_ID_DNS,
    MERCURY_ENUM_DNSNOTIFY_MAX = 0x7fffffff
}DNS_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_CHR_CAP_IND = 0x1,      // Notify the battery's capacity
    NOTIFY_ID_CHR_CHARGE_START_IND,   // start the charge process.
    NOTIFY_ID_CHR_CHARGE_END_IND,     // the charge ended.
    NOTIFY_ID_CHR_WARNING_IND,        // the capacity is low, should charge.
    NOTIFY_ID_CHR_SHUTDOWN_IND,       // the capacity is very low and must shutdown.
    NOTIFY_ID_CHR_CHARGE_FINISH,      // the charge has been completed.
    NOTIFY_ID_CHR_CHARGE_DISCONNECT,  // the charge be disconnect
    NOTIFY_ID_CHR_CHARGE_FAULT,       // the charge fault, maybe the voltage of charge is too low.
    NOTIFY_ID_CHR_CHARGE_PLUG_IN_MSG, // plug in msg
    NOTIFY_ID_CHR_CHARGE_PLUG_OUT_MSG, // plug out msg
    NOTIFY_ID_CHR_MSG_MAX_NUM,
    MERCURY_ENUM_CHRNOTIFY_MAX = 0x7fffffff
} MC_CHR_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_SCREEN_ON= 0,
    NOTIFY_ID_SCREEN_OFF,
    NOTIFY_ID_SCREEN,
    MERCURY_ENUM_SCREENNOTIFY_MAX = 0x7fffffff
}MC_SCREEN_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_POWERKEY_LONGPRESS= 0,
    NOTIFY_ID_POWERKEY_SHORTPRESS,
    NOTIFY_ID_POWER_MSG_MAX_NUM,
    MERCURY_ENUM_POWERNOTIFY_MAX = 0x7fffffff
}MC_POWER_NOTIFY_ID_E;


typedef enum
{
    NOTIFY_ID_BARSCAN_INITED = 0,
    NOTIFY_ID_BARSCAN_PRESCAN_DONE,
    NOTIFY_ID_BARSCAN_SCANING,
    NOTIFY_ID_BARSCAN_SCANED_SUCCESS,
    NOTIFY_ID_BARSCAN_SCANED_FAILED,
    NOTIFY_ID_BARSCAN_ID_MAX,
    MERCURY_ENUM_SCANNOTIFY_MAX = 0x7fffffff
}MC_BARSCAN_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_TTS_PLAY_COMPLETE = 0,
    NOTIFY_ID_TTS_PLAY_START = 1,
    NOTIFY_ID_TTS_MSG_MAX,
    MERCURY_ENUM_TTSNOTIFY_MAX = 0x7fffffff
}MC_TTS_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_SIM_READY= 0,
    NOTIFY_ID_SIM_NOT_READY,
    NOTIFY_ID_PS_POWER_ON,
    NOTIFY_ID_PS_POWER_OFF,
    NOTIFY_ID_OS_PANIC,
    NOTIFY_ID_SYSTEM_MSG_MAX,
    MERCURY_ENUM_SYSNOTIFY_MAX = 0x7fffffff
}SYSTEM_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_DTMF_PLAY_COMPLETE= 0,
    NOTIFY_ID_STONE_PLAY_COMPLETE,
    NOTIFY_ID_PCM_PLAY_COMPLETE,
    NOTIFY_ID_AMR_PLAY_COMPLETE,
    NOTIFY_ID_MP3_PLAY_COMPLETE,
    NOTIFY_ID_AUDIO_MSG_MAX,
    MERCURY_ENUM_AUDIONOTIFY_MAX = 0x7fffffff
}MC_AUDIO_NOTIFY_ID_E;

typedef enum
{
    NOTIFY_ID_STK_REFRESH_IND= 0,
    NOTIFY_ID_STK_EID_GET_SUCCESS,
    NOTIFY_ID_STK_EID_GET_FAILE,
    NOTIFY_ID_STK_MSG_MAX = 0x7fffffff
}MC_STK_NOTIFY_ID_E;


typedef enum
{
    NOTIFY_ID_PBK_READ_OK = 0,
    NOTIFY_ID_PBK_READ_FAIL,
    NOTIFY_ID_PBK_READ_END,
    NOTIFY_ID_PBK_WRITE_OK,
    NOTIFY_ID_PBK_WRITE_FAIL,
    NOTIFY_ID_PBK_DETELE_OK,
    NOTIFY_ID_PBK_DETELE_FAIL,
    NOTIFY_ID_PBK_MAX
}PBK_NOTIFY_ID_E;


typedef enum
{
    NOTIFY_ID_HTTP_GET_COMPLETE = 1,
    NOTIFY_ID_HTTP_PUT_COMPLETE,
    NOTIFY_ID_HTTP_HEAD_COMPLETE,
    NOTIFY_ID_HTTP_AUTHOR_COMPLETE,
    NOTIFY_ID_HTTP_OPTIONS_COMPLETE,
    NOTIFY_ID_HTTP_TRACE_COMPLETE,
    NOTIFY_ID_HTTP_DELETE_COMPLETE,
    NOTIFY_ID_HTTP_POST_COMPLETE,
    NOTIFY_ID_HTTP_DOWNLOAD_COMPLETE,
    NOTIFY_ID_HTTP_GETN_COMPLETE = 10,
    NOTIFY_ID_HTTP_POSTN_COMPLETE,
    NOTIFY_ID_HTTP_DELETEN_COMPLETE,
    NOTIFY_ID_HTTP_CONFIG_COMPLETE,
    NOTIFY_ID_HTTP_HEADN_COMPLETE,
    NOTIFY_ID_HTTP_ONENET_DELETE_COMPLETE,
    NOTIFY_ID_HTTP_ONENET_PUT_COMPLETE,
    NOTIFY_ID_HTTP_INPUTDATA_COMPLETE,
    NOTIFY_ID_HTTP_READ_COMPLETE,
    NOTIFY_ID_HTTP_DATA, // 读取数据的上报，以及一些事件完成后 需要上报数据
    NOTIFY_ID_HTTP_STATUS,
    NOTIFY_ID_HTTP_MAX
} HTTP_NOTIFY_ID_E;

typedef enum
{
	NOTIFY_ID_SIM_PLUGIN,    
	NOTIFY_ID_SIM_PLUGOUT,	 
	NOTIFY_ID_SIM_CRSM_RESP,
    NOTIFY_ID_SIM_MSG_MAX
}MC_SIM_NOTIFY_ID_E;

typedef enum
{
	NOTIFY_ID_NW_SET_MODE_OK = 0,
    NOTIFY_ID_NW_SET_MODE_FAIL,
	NOTIFY_ID_NW_SCAN_FREQ_OK,		/*scan cell frequence with cells information success*/
	NOTIFY_ID_NW_SCAN_FREQ_FAIL,	/*scan cell frequence fail*/
	NOTIFY_ID_NW_SCAN_FREQ_TIMEOUT, /*scan cell frequence timeout*/    
	NOTIFY_ID_NW_MAX
}MC_NW_NOTIFY_ID_E;

typedef enum
{
	NOTIFY_ID_NTP_UPDATE_OK = 0,
    NOTIFY_ID_NTP_UPDATE_FAIL,
	NOTIFY_ID_NTP_MAX
}MC_NTP_NOTIFY_ID_E;


typedef enum
{
	NOTIFY_ID_PWM_TIMEOUT_STOP = 0,
	NOTIFY_ID_PWM_MAX
}MC_PWM_NOTIFY_ID_E;

typedef void (*NotifyCallback)(WORD,WORD ,void* ,DWORD);


#define MN_MAX_IMSI_ARR_LEN             8
#define MNSIM_ICCID_ID_NUM_LEN      10

typedef enum
{
    SIM_STATE_E = 0,
    SIM_IMSI_E,
    SIM_CCID_E,
    SIM_ALL,
    SIM_MAX,
    MERCURY_ENUM_SIMINFO_MAX = 0x7fffffff
}SIM_INFO_E;

typedef enum
{
	NOTIFY_ID_WIFISCAN_SUCCESS = 0,
    NOTIFY_ID_WIFISCAN_DATA,
    NOTIFY_ID_WIFISCAN_FAIL,
    NOTIFY_ID_WIFISCAN_TIMEOUT,
    NOTIFY_ID_WIFISCAN_NO_MEMORY,
    NOTIFY_ID_WIFISCAN_END,
    NOTIFY_ID_WIFISCAN_MAX = 0x7fffffff
}MC_WIFISCAN_NOTIFY_ID_E;


typedef struct
{
    BYTE imsi_len;
    BYTE imsi_val[20];
} SIM_IMSI_T;

typedef struct
{
    BYTE id_num[2*MNSIM_ICCID_ID_NUM_LEN + 1];
} SIM_ICCID_T;

#ifdef MERCURY_DEBUG_VARIANT
#define MercuryTrace _MercuryTrace
#else
#define MercuryTrace if(0)
#endif

typedef int (*MECURY_LOGO_UPDATA_CALLBACK_FPTR)(
    uint8					*pbuf,          
    uint32                  rlen);  


typedef struct
{
    uint32 magicNum;
    uint16 left;
    uint16 top;
    uint16 width;
    uint16 height;
    uint32 imageLen;
    uint32 lcdTypeIndex;
}MERCURY_BOOT_IMAGE_S;

typedef struct
{
	uint32 keyExist;
  	uint32 hashExist;
  	uint32 otpState;
  	uint32 secureState;
  	
}MERCURY_SECURE_STATE;

typedef enum{
	MC_TRACE_TYPE_AP = 0,
	MC_TRACE_TYPE_CP,
	MC_TRACE_TYPE_MAX
}MC_TRACE_TYPE_E;

typedef enum{
	MC_TRACE_STATE_OFF = 0,
	MC_TRACE_STATE_UART,
	MC_TRACE_STATE_USB,
	MC_TRACE_STATE_MAX
}MC_TRACE_STATE_E;

typedef enum
{
    ML_ISO8859_1,
    ML_UTF8,
    ML_UTF16BE,
    ML_UTF16LE,
    ML_GSM,
    ML_CP936   // gbk
}CHAR_SET;

void RegNotifyCallback(NotifyCallback fun);
int GetSimInfo(SIM_INFO_E flag,SIM_IMSI_T* imsi_str,SIM_ICCID_T* ccid);
BYTE* GetImei(void);
BYTE* GetMercuryVersion(void);
BYTE* GetMercuryCPVersion(void);
void MercuryDebug(uint8 *buffer,uint32 length);
int MercuryFactoryTestResult(uint32 result);
void _MercuryTrace(const char *fmt, ...);
DWORD GetLastError(void);
BOOLEAN MercuryReadID(uint8* MercuryID);
int MercuryEIDGet(void);
int MercuryLogoUpdata(MERCURY_BOOT_IMAGE_S *bootImageInfo, MECURY_LOGO_UPDATA_CALLBACK_FPTR callback_fptr, uint32 len);
char* GetSdkVersion(void);
uint8* MercuryGetPsRwMem(int type,int *pBufSize);

/*****************************************************************************/
//	Description:  set ap & cp trace output: off, uart, usb
//	Param
//		 type: ap or cp
//		 state : 	MC_TRACE_STATE_OFF: close trace output, uart, usb  
//		 		MC_TRACE_STATE_UART: trace output from uart
//		 		MC_TRACE_STATE_USB: trace output from usb
//	Return:
//		 0: Success
//		 -1: FAIL
//	Note: the config will be effective for cp trace type at once. but for ap trace type, you need reboot it
/*****************************************************************************/
int MercurySetTrace(MC_TRACE_TYPE_E type, MC_TRACE_STATE_E state);
MC_TRACE_STATE_E MercuryGetApTraceState(void);
int EnableWatchDog(bool enable);
int GetWatchDogPriority();
int SetWatchDogPriority(int priority);
// CHAR_SET 
// 返回值需要free
void *CharsetConvert(const void *from, int from_size, unsigned from_chset, unsigned to_chset, int *to_size);

int MercuryWriteImei(char* ImeiStr);

/*****************************************************************************/
//	Description:  write IMEI
//	Param
//		 ImeiStr1 : IMEI1
//		 ImeiStr2 : IMEI1.If it is not a dual card version, set to null.		 		
//	Return:
//		 0: Success
//		 -1: FAIL
/*****************************************************************************/
int MercuryWriteDualImei(char* ImeiStr1, char* ImeiStr2);

#endif //__MERCURY_PLAT_H__
