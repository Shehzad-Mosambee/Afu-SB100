#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "MercuryNet.h"

// #define DEVELOPMENT_BUILD
#define PRODUCTION_BUILD

#define NULL	((void*)0)

#define SUCCESS     0
#define ERROR       1

#define DUAL_AUDIO // ENG+OTHER

// #define AMR_AUDIO
#define MP3_AUDIO

#ifdef AMR_AUDIO
#define AUDIO_TYPE  ".amr"
#else
#define AUDIO_TYPE  ".mp3"
#endif


#define ASYNC_AUDIO     1


#define DEVICE_CATEGORY     "2"
#define DEVICE_REG_MODE     "0"
// #define PARTNER_CODE_2		"MQ00002"
// #define PARTNER_CODE_1      "QF100"
#define PARTNER_CODE      "QF100"
#define KEY                 "357538782F4125442A472D4B6150645367566B59703373367639792442264528"

// #define PARTNER_CODE_1        "SOUNDBOX001"
// #define KEY                 "357538782F4125442A472D4B6150645367566B59703373367639792442264528"



#define TRANS_RECORD_NUM 10
#define TRANS_LEN_MAX 225

typedef struct _ResponseDetails_
{
	char result[10+1];
	// char message[128+1];

}RESPONSE_DETAILS;

typedef struct __MQTT_TXN_DETAILS__
{
    unsigned char notifType;
    char amt[13+1];
    char otp[13+1];
    char ln[2+1];

}MQTT_TXN_DETAILS;

typedef struct __tag_register_device__
{
	// char cryptoKey[64+1];
	// char cryptoNonce[32+1];

    // unsigned char retainFlag;
	int QOS;
    int cleanSession;
    int keepAlive;
    char userName[64+1];
    char clientID[64+1];
    char subscriptionTopic[128+1];

} REGISTER_DEVICE;

typedef struct __tag_PLMN__
{
    int mcc;
    int mnc;
} PLMN;

typedef struct __tag_PLMN_APN_LIST__
{
    PLMN plmn_details;
    char apn[48];
} APN_LIST;


typedef struct __tag_TRANS_RECORD__
{
	int total;
	int windex; // д��¼
	int rindex; // ����¼
	char data[TRANS_RECORD_NUM][TRANS_LEN_MAX];
}TRANS_RECORD;


extern int play_trans_mode;
extern int network_select_mode;

extern volatile int ota_thread_finished;

extern RESPONSE_DETAILS	ResponseDetails;

extern MQTT_TXN_DETAILS mqtt_txn_details;
extern REGISTER_DEVICE	RegisterDevice;

extern MERCURY_NETWORK_STATUS_T g_pStatus;

extern APN_LIST apn_list[];

extern TRANS_RECORD g_transRecord;

#endif // __GLOBAL_H__