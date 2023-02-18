#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "speakerapp.h"
#include "SDK_API_AUDIO.h"

#include "global.h"

#define FS_DRIVE        "E://" 
#define AUDIO_AMR       ".amr"
#define AUDIO_PCM       ".pcm"
#define AUDIO_WAV       ".wav"
#define AUDIO_MP3       ".mp3"
#define AUDIO_TYPE_COUNT      4
#define DEF_AUDIO_TYPE      AUDIO_AMR

#define MAX_AUDIO_FILE_NAME_LEN     0x20



#define AUDIO_HOST_ADDR "https://test.mosambee.in"

// #define AUDIO_RESOURCE_COUNT_URL "/szanfu-admin/audio/count?productId=a1TrHYpY5LD&version=1.3"
// #define AUDIO_FILE_INFO_URL "/szanfu-admin/audio/info?productId=a1TrHYpY5LD&version=1.3&index="

#define AUDIO_RESOURCE_COUNT_URL_BASE	"/szanfu-admin/audio/count?productId=a1TrHYpY5LD&version="
// #define AUDIO_RESOURCE_COUNT_URL AUDIO_RESOURCE_COUNT_URL_BASE MOS_AUDIO_VERSION

#define AUDIO_FILE_INFO_URL_BASE "/szanfu-admin/audio/info?productId=a1TrHYpY5LD&version="
#define AUDIO_FILE_INFO_URL_IDX	"&index="
#define AUDIO_FILE_INFO_URL	AUDIO_FILE_INFO_URL_BASE MOS_AUDIO_VERSION AUDIO_FILE_INFO_URL_IDX

// #define AUDIO_RESOURCE_URL "/iot/resources/audio/list?productId=a1TrHYpY5LD&version=1.4"
// #define AUDIO_RESOURCE_LIST "/szanfu-admin/audio/download?productId=a1TrHYpY5LD&version=1.3&audioId="

#define AUDIO_RESOURCE_URL_BASE "/iot/resources/audio/list?productId=a1TrHYpY5LD&version="
#define AUDIO_RESOURCE_URL AUDIO_RESOURCE_URL_BASE MOS_AUDIO_VERSION

#define AUDIO_RESOURCE_LIST_BASE	"/szanfu-admin/audio/download?productId=a1TrHYpY5LD&version="
#define AUDIO_RESOURCE_LIST_IDX		"&audioId="
#define AUDIO_RESOURCE_LIST AUDIO_RESOURCE_LIST_BASE MOS_AUDIO_VERSION AUDIO_RESOURCE_LIST_IDX

#define AUDIO_INFO_URL "/speaker/audio-res/info"




#define CRORE           10000000
#define LAKH            100000
#define THOUSAND        1000
#define HUNDRED         100

#define	EN	0
#define	HI	1
#define MR	2




/*
#define AUDIO_DOWNLOAD_FAIL                           "11003"
#define AUDIO_CHECK_SIM                               "21003"

#define AUDIO_MQTT_SUBSCRIBE_SUCCESS                  "51028"
#define AUDIO_MQTT_SUBSCRIBE_FAIL                     "11027"
#define AUDIO_MQTT_CONNECT                            "51030"
#define AUDIO_MQTT_SERVER_CONNECT                     "51010"

#define AUDIO_ONE                                     "41001"
#define AUDIO_TWO                                     "41002"
#define AUDIO_THREE                                   "41003"
#define AUDIO_FOUR                                    "41004"
#define AUDIO_FIVE                                    "41005"
#define AUDIO_SIX                                     "41006"
#define AUDIO_SEVEN                                   "41007"
#define AUDIO_EIGHT                                   "41008"
#define AUDIO_NONE                                    "41009"
#define AUDIO_TEN                                     "41013"
#define AUDIO_ZERO                                    "41010"
#define AUDIO_HUNDRED                                 "41014"
#define AUDIO_THOUSAND                                "41015"
#define AUDIO_MILLION                                 "41037"
#define AUDIO_AND                                     "41012"
#define AUDIO_TWENTY                                  "41027"

#define AUDIO_PORINT                                  "41011"
#define AUDIO_DOLLAR                                  "41016"
#define AUDIO_DOLLARS                                 "41017"
#define AUDIO_CENT                                    "41035"
#define AUDIO_CENTS                                   "41036"



#define AUDIO_WIFI_NETWORK_INIT                       "51022"
#define AUDIO_WIFI_NETWORK_INIT_FAIL                  "11008"
#define AUDIO_NO_WIFI_CONFIG                          "51023"
#define AUDIO_WIFI_CONNECT_SUCCESS                    "51025"

#define AUDIO_NETWORK_CONFIG_FAIL                     "11011"
#define AUDIO_CONFIG_COMPLETE                         "51006"
#define AUDIO_WIFI_NETWORK_CONNETC                    "51009"
#define AUDIO_WIFI_NETWORK_CHECK_COMPLETED            "51043"

#define AUDIO_BATTERY                                 "51001"
#define AUDIO_PERCENT                                 "51002"
#define AUDIO_MOBILE_NETWORK                          "51003"
#define AUDIO_WIRELESS_NETWORK                        "51004"
#define AUDIO_NO_WIRELESS_NETWORK_CONFIGURED          "51023"

#define AUDIO_REACH_THE_MOBILE_NETWORK                "31011"
#define AUDIO_REACH_THE_WIRELESS_NETWORK              "31012"


#define AUDIO_DEVICE_STARTING_UP                      "71001"
#define AUDIO_DEVICE_SHUTDOWN                         "71002"
#define AUDIO_DEVICE_REBOOTING                        "51026"

#define AUDIO_MOBLILE_NETWORK_CONNECTING              "51008"
#define AUDIO_MOBLILE_NETWORK_CONNECTED               "51024"

#define AUDIO_VOLUME_UP                               "31002"
#define AUDIO_VOLUME_DOWN                             "31003"
#define AUDIO_MAXIMUM_VOLUME                          "31004"
#define AUDIO_MINIMUM_VOLUME                          "31005"
#define AUDIO_VOLUME_UP_BUTTON                        "51039"
#define AUDIO_VOLUME_DOWN_BUTTON                      "51040"
#define AUDIO_REPLAY_KEY_BUTTON                       "51041"
#define AUDIO_POWER_BUTTON                            "51042"
	
#define AUDIO_SIGNAL_STRENGTH                         "51047"

#define AUDIO_ONE                                     "41001"
#define AUDIO_TWO                                     "41002"
#define AUDIO_THREE                                   "41003"
#define AUDIO_FOUR                                    "41004"
#define AUDIO_FIVE                                    "41005"
#define AUDIO_SIX                                     "41006"
#define AUDIO_SEVEN                                   "41007"
#define AUDIO_EIGHT                                   "41008"
#define AUDIO_NONE                                    "41009"
#define AUDIO_TEN                                     "41013"
#define AUDIO_ZERO                                    "41010"
#define AUDIO_HUNDRED                                 "41014"
#define AUDIO_THOUSAND                                "41015"
#define AUDIO_MILLION                                 "41037"
#define AUDIO_AND                                     "41012"
#define AUDIO_TWENTY                                  "41027"

#define AUDIO_PORINT                                  "41011"
#define AUDIO_DOLLAR                                  "41016"
#define AUDIO_DOLLARS                                 "41017"
#define AUDIO_CENT                                    "41035"
#define AUDIO_CENTS                                   "41036"
*/


// #define AUDIO_MV_VER
// #ifndef AUDIO_MV_VER
// enum 
// {
// 	AUDIO_DOWNLOAD_FAIL = 0,
// 	AUDIO_CHECK_SIM,
// 	AUDIO_MQTT_SUBSCRIBE_SUCCESS,
// 	AUDIO_MQTT_SUBSCRIBE_FAIL,
// 	AUDIO_MQTT_CONNECT,
// 	AUDIO_MQTT_SERVER_CONNECT,
// 	AUDIO_WIFI_NETWORK_INIT,
// 	AUDIO_WIFI_NETWORK_INIT_FAIL,
// 	AUDIO_NO_WIFI_CONFIG,
// 	AUDIO_WIFI_CONNECT_SUCCESS,
// 	AUDIO_NETWORK_CONFIG_FAIL = 10,
// 	AUDIO_CONFIG_COMPLETE,
// 	AUDIO_WIFI_NETWORK_CONNETC,
// 	AUDIO_WIFI_NETWORK_CHECK_COMPLETED,
// 	AUDIO_BATTERY,
// 	AUDIO_PERCENT,
// 	AUDIO_MOBILE_NETWORK,
// 	AUDIO_WIRELESS_NETWORK,
// 	AUDIO_NO_WIRELESS_NETWORK_CONFIGURED,
// 	AUDIO_REACH_THE_MOBILE_NETWORK,
// 	AUDIO_REACH_THE_WIRELESS_NETWORK = 20,
// 	AUDIO_DEVICE_STARTING_UP,
// 	AUDIO_DEVICE_SHUTDOWN,
// 	AUDIO_DEVICE_REBOOTING,
// 	AUDIO_MOBLILE_NETWORK_CONNECTING,
// 	AUDIO_MOBLILE_NETWORK_CONNECTED,
// 	AUDIO_VOLUME_UP,
// 	AUDIO_VOLUME_DOWN,
// 	AUDIO_MAXIMUM_VOLUME,
// 	AUDIO_MINIMUM_VOLUME,
// 	AUDIO_VOLUME_UP_BUTTON = 30,
// 	AUDIO_VOLUME_DOWN_BUTTON,
// 	AUDIO_REPLAY_KEY_BUTTON,
// 	AUDIO_POWER_BUTTON,
// 	AUDIO_SIGNAL_STRENGTH,
//     AUDIO_PAY_WECHAT,
// 	AUDIO_ONE, // 36
// 	AUDIO_TWO,
// 	AUDIO_THREE,
// 	AUDIO_FOUR,
// 	AUDIO_FIVE = 40,
// 	AUDIO_SIX,
// 	AUDIO_SEVEN,
// 	AUDIO_EIGHT,
// 	AUDIO_NINE,
// 	AUDIO_TEN,
// 	AUDIO_ZERO,
// 	AUDIO_HUNDRED,
// 	AUDIO_THOUSAND,
// 	AUDIO_MILLION,
// 	AUDIO_AND = 50,
// 	AUDIO_DOT = 50,
// 	AUDIO_ELEVN,
// 	AUDIO_TWELVE,
// 	AUDIO_THIRTEEN,
// 	AUDIO_FOURTEEN,
// 	AUDIO_FIFTEEN,
// 	AUDIO_SIXTEEN,
// 	AUDIO_SEVENTEEN,
// 	AUDIO_EIGHTEEN,
// 	AUDIO_NINETEEN,
// 	AUDIO_TWENTY = 60,
// 	AUDIO_THIRTY,
// 	AUDIO_FOURTY,
// 	AUDIO_FIFTY,
// 	AUDIO_SIXTY,
// 	AUDIO_SEVENTY,
// 	AUDIO_EIGHTY,
// 	AUDIO_NINETY,
// 	AUDIO_POINT,
// 	AUDIO_DOLLAR,
// 	AUDIO_DOLLARS = 70,
// 	AUDIO_YUAN = 70,
// 	AUDIO_FEN,
// 	AUDIO_CENTS,

// 	AUDIO_OTA_UPDATE,
// 	AUDIO_UPGRADE_FILE_DL_SUC,
// 	AUDIO_UPGRADE_DL_FAIL,
// 	AUDIO_START_UPGRADE,
// 	AUDIO_WAIT_UPGRADING,
// 	AUDIO_UPDATE_FAIL,
// 	AUDIO_NO_UPGRADING,
// 	AUDIO_UPDATE_SUC = 80,

// 	AUDIO_ONLY_WIRELESS_NETWORKS,
// 	AUDIO_ONLY_MOBILE_NETWORKS,

// 	AUDIO_WIFI_APCFG,
// 	AUDIO_WIFI_AKCFG,

// 	AUDIO_GET_SERV_CONFIG,
// 	AUDIO_GET_SERVCFG_FAIL,
// 	AUDIO_GET_SERVCFG_SUCC,

// 	AUDIO_MQTT_SERVER_FAIL,

// 	AUDIO_BAT_POWEROFF,
// 	AUDIO_BAT_POWEROFF2,
// 	AUDIO_BAT_WARNING,
// 	AUDIO_CHARGER_IN,
// 	AUDIO_CHARGER_OUT,
// 	AUDIO_NO_SIGNAL,
// 	AUDIO_NO_LICENSE,
// 	AUDIO_OTA_DL_SUCC,
// 	AUDIO_OTA_DL_FAIL,
// 	AUDIO_OTA_UPDATE_FAIL,
// 	AUDIO_OTA_UPDATE_SUCC,
// 	AUDIO_OTA_DOWNLOADING,
// 	AUDIO_OTA_UPDATING,
	
// 	AUDIO_INDEX_MAX,
// };
// #else
// enum
// {
// 	AUDIO_ONE, // 10001
// 	AUDIO_TWO,
// 	AUDIO_THREE,
// 	AUDIO_FOUR,
// 	AUDIO_FIVE,
// 	AUDIO_SIX,
// 	AUDIO_SEVEN,
// 	AUDIO_EIGHT,
// 	AUDIO_NINE,
// 	AUDIO_TEN,
// 	AUDIO_HUNDRED, // 10011
// 	AUDIO_THOUSAND,
// 	AUDIO_WAN,
// 	AUDIO_MILLION,
// 	AUDIO_ELEVN,
// 	AUDIO_TWELVE,
// 	AUDIO_THIRTEEN,
// 	AUDIO_FOURTEEN,
// 	AUDIO_FIFTEEN,
// 	AUDIO_SIXTEEN,
// 	AUDIO_SEVENTEEN, // 10021
// 	AUDIO_EIGHTEEN,
// 	AUDIO_NINETEEN,
// 	AUDIO_TWENTY,
// 	AUDIO_THIRTY,
// 	AUDIO_FOURTY,
// 	AUDIO_FIFTY,
// 	AUDIO_SIXTY,
// 	AUDIO_SEVENTY,
// 	AUDIO_EIGHTY,
// 	AUDIO_NINETY, // 10031
// 	AUDIO_YUAN,
// 	AUDIO_FEN,
// 	AUDIO_PERCENT,
// 	AUDIO_ZERO,
// 	AUDIO_DOT,
// 	AUDIO_AND,
// 	AUDIO_DOLLAR,
// 	AUDIO_DOLLARS,
// 	AUDIO_CENT,
// 	AUDIO_CENTS,
// 	AUDIO_RUPEE,
// 	AUDIO_RUPEES,
// 	AUDIO_PAISE,
// 	AUDIO_DEVICE_STARTING_UP, // 20001
// 	AUDIO_FACTORY_MODE,
// 	AUDIO_POWER_BUTTON,
// 	AUDIO_VOLUME_UP,
// 	AUDIO_VOLUME_DOWN,
// 	AUDIO_REPLAY_KEY_BUTTON,
// 	AUDIO_NET_ABNORMAL,
// 	AUDIO_NO_LICENSE,
// 	AUDIO_USE_CORRECT_AUTH,
// 	AUDIO_POWER_OFF,
// 	AUDIO_AUTH_FAIL, // 20011
// 	AUDIO_NO_SN,
// 	AUDIO_AUTH_EXPIRED,
// 	AUDIO_NO_SIGNAL,
// 	AUDIO_SIGNAL_STRENGTH,
// 	AUDIO_VERSION_NO,
// 	AUDIO_IMEI,
// 	AUDIO_ICCID,
// 	AUDIO_SN,
// 	AUDIO_SOFTWARE_NO_UPDATE,
// 	AUDIO_SOFTWARE_UPDATE, // 20021
// 	AUDIO_DISCOUNT,
// 	AUDIO_VERIFY_CODE,
// 	AUDIO_CFG_CHANGE_RESET,
// 	AUDIO_DEVICE_SHUTDOWN,
// 	AUDIO_DEVICE_REBOOTING,
// 	AUDIO_MINIMUM_VOLUME,
// 	AUDIO_MAXIMUM_VOLUME,
// 	// 20029�ź�ǿ���ظ�
// 	AUDIO_BATTERY,
// 	AUDIO_PAY_WECHAT, // 20031
// 	AUDIO_WIFI_AKCFG,
// 	AUDIO_WIFI_APCFG,
// 	AUDIO_GET_SERV_CONFIG,
// 	AUDIO_GET_SERVCFG,
// 	AUDIO_MQTT_SERVER_FAIL,
// 	AUDIO_BAT_POWEROFF,
// 	AUDIO_BAT_POWEROFF2,
// 	AUDIO_BAT_WARNING,
// 	AUDIO_CHARGER_IN,
// 	AUDIO_CHARGER_OUT, // 20041
// 	AUDIO_OTA_DOWNLOADING,
// 	AUDIO_OTA_DOWNLOAD,
// 	AUDIO_OTA_UPDATING,
// 	AUDIO_OTA_UPDATE,
// 	AUDIO_OTA_SUCCESS,
// 	AUDIO_SPEAKER, // 30001
// 	AUDIO_ALIPAY,
// 	AUDIO_KEYPAD,
// 	AUDIO_MOBILE_NETWORK,
// 	AUDIO_WIRELESS_NETWORK,
// 	AUDIO_LED,
// 	AUDIO_USB,
// 	AUDIO_USB_OUT,
// 	AUDIO_BCS,
// 	AUDIO_TEST_RESET_KEY,
// 	AUDIO_PRESS_RESET, // 30011
// 	AUDIO_WRITE_CFG,
// 	AUDIO_LICENSE_NET_FAIL,
// 	AUDIO_LICENSE_ACTIVATE,
// 	AUDIO_LICENSE,
// 	// 30016��ص����ظ�
// 	AUDIO_CFG_DONE,
// 	AUDIO_SUB_SUCCESS,
// 	AUDIO_MQTT_SUBSCRIBE_FAIL,
// 	AUDIO_MQTT,
// 	AUDIO_SERVER, // 30021
// 	AUDIO_INITIAL,
// 	AUDIO_INITIAL_FAIL,
// 	AUDIO_CONNECTING,
// 	AUDIO_GET_CFG,
// 	AUDIO_CHECK_DONE,
// 	AUDIO_CURRENT,
// 	AUDIO_NO_CFG,
// 	AUDIO_SWITCH,
// 	// AUDIO_CONNECTING,
// 	AUDIO_DEV_SUPPORT, // 30031
// 	// AUDIO_INITIAL_SUCCESS,

// 	AUDIO_PASS, // 61001
// 	AUDIO_FAIL,
// 	AUDIO_TIMEOUT,
// 	AUDIO_SUCCESS,
// 	AUDIO_TEST,
// 	AUDIO_CONNECT,
// 	AUDIO_FACTORY_CHECK,

// 	AUDIO_SN_ERROR, // 40001
// 	AUDIO_LATEST_VER,
// 	AUDIO_SERVER_REFUSE,
// 	AUDIO_SYSTEM_BUSY,
// 	AUDIO_NO_SOFTWARE,
// 	AUDIO_HASH_ERROR,
// 	AUDIO_NO_SPACE,
// 	AUDIO_DATA_PARSE_FAIL,
// 	AUDIO_UPDATE_FAIL,
// 	AUDIO_UPDATE_INFO_ERROR,
// 	AUDIO_UPDATE_INVALID,
// 	AUDIO_SERVE_RECG_FAIL,
// 	AUDIO_INDEX_MAX,

// 	AUDIO_WIFI_NETWORK_INIT_FAIL = AUDIO_INDEX_MAX,
// 	AUDIO_MOBLILE_NETWORK_CONNECTING,
// 	AUDIO_MOBLILE_NETWORK_CONNECTED,
// 	AUDIO_WIFI_CONNECT_SUCCESS,
// 	AUDIO_NETWORK_CONFIG_FAIL,
// 	AUDIO_OTA_DL_SUCC,
// 	AUDIO_OTA_DL_FAIL,
// 	AUDIO_OTA_UPDATE_FAIL,
// 	AUDIO_OTA_UPDATE_SUCC,
// 	AUDIO_MQTT_SERVER_CONNECT,
// 	AUDIO_GET_SERVCFG_FAIL,
// 	AUDIO_ONLY_MOBILE_NETWORKS,
// 	AUDIO_NO_WIRELESS_NETWORK_CONFIGURED,
// 	AUDIO_REACH_THE_WIRELESS_NETWORK,
// 	AUDIO_REACH_THE_MOBILE_NETWORK,
// 	AUDIO_JOINT_MAX,
// };
// #endif

/*

// #ifdef DUAL_AUDIO
enum
{
	AUDIO_000,		// 121	// 10000
	
	AUDIO_001,				// 10001
	AUDIO_002,				// 10002
	AUDIO_003,		// 124	// 10003
	AUDIO_004,				// 10004
	AUDIO_005,		// 126	// 10005
	AUDIO_006,				// 10006
	AUDIO_007,				// 10007
	AUDIO_008,				// 10008
	AUDIO_009,				// 10009
	AUDIO_010,		// 131	// 10010
	
	AUDIO_011,		// 132	// 10011
	AUDIO_012,				// 10012
	AUDIO_013,				// 10013
	AUDIO_014,				// 10014
	AUDIO_015,				// 10015
	AUDIO_016,				// 10016
	AUDIO_017,				// 10017
	AUDIO_018,				// 10018
	AUDIO_019,				// 10019
	AUDIO_020,		// 141	// 10020
	AUDIO_030,		// 142	// 10021
	AUDIO_040,		// 143	// 10022
	AUDIO_050,		// 144	// 10023
	AUDIO_060,		// 145	// 10024
	AUDIO_070,		// 146	// 10025
	AUDIO_080,		// 147	// 10026
	AUDIO_090,		// 148	// 10027
	
	AUDIO_RUPEES,				// 149	// 10028
	AUDIO_PAISE,				// 150	// 10029
	AUDIO_AND,					// 151	// 10030
	AUDIO_RUPEE,				// 152  // 10031
	
	AUDIO_HUNDRED, 			// 153	// 10032
	AUDIO_THOUSAND,			// 154	// 10033
	AUDIO_LAKH,				// 155	// 10034
	AUDIO_CRORE,			// 156	// 10035
	
	AUDIO_DEVICE_STARTING_UP,	// 157	// 10036
	AUDIO_RECVD_PAYMENT_OF,		// 158	// 10037
	AUDIO_LAST_RECVD,			// 159	// 10038
	AUDIO_CNF_CODE_H,			// 160	// 10039
	
	AUDIO_SEPARATOR,			// 161	// 10040
	AUDIO_SUCCESS,				// 162	// 10041
	AUDIO_FAILURE,				// 163	// 10042
	AUDIO_THANK_YOU,			// 164	// 10043

	AUDIO_SHUTTING_DOWN,		// 165	// 10044
	AUDIO_SHUT_MESSAGE,			// 166	// 10045
	
	AUDIO_MIN_VOLUME,				// 167	// 10046
	AUDIO_MAX_VOLUME,				// 168	// 10047
	AUDIO_BAT_LOW_PC,				// 169	// 10048
	AUDIO_BAT_LOW_SD,				// 170	// 10049
	AUDIO_CHARGING,					// 171	// 10050
	AUDIO_BATTERY_PERCENT,			// 172	// 10051
	AUDIO_SIGNAL_STRENGTH,			// 173	// 10052 -- Not using anymore
	
	AUDIO_CONNECTING,				// 174	// 10053
	AUDIO_RECONNECTING,				// 175	// 10054 *
	AUDIO_SERVER_CONNECT_SUCCESS,	// 176	// 10055
	AUDIO_SERVER_CONNECT_FAILED,	// 177	// 10056	
	
	AUDIO_START_UPDATE,				// 178	// 10057
	AUDIO_UPDATE_FAILED,			// 179	// 10058
	AUDIO_UPDATE_SUCCESS,			// 180	// 10059
	AUDIO_DOWNLOAD_FAILED,			// 181	// 10060
	AUDIO_DOWNLOAD_SUCCESS,			// 182	// 10061
	AUDIO_SETUP_FAILED,				// 183	// 10062
	AUDIO_SETUP_SUCCESS,			// 184	// 10063
	
	AUDIO_CHECKING_WIFI,			// 185	// 10064
	AUDIO_CONNECTING_WIFI,			// 186	// 10065
	AUDIO_CONNECTING_SIM,			// 187	// 10066
	AUDIO_NETWORK_MODE_SIM,			// 188	// 10067
	AUDIO_NETWORK_MODE_WIFI,		// 189	// 10068
	AUDIO_ONLY_SIM_NETWORK,			// 190	// 10069
	AUDIO_ONLY_WIFI_NETWORK,		// 191	// 10070
	
	AUDIO_WIFI_AP_MODE,				// 192	// 10071
	AUDIO_WIFI_AK_MODE,				// 193	// 10072
	AUDIO_WIFI_CONFIG_FAILED,		// 194	// 10073
	AUDIO_WIFI_CONFIG_SUCCESS,		// 195	// 10074
	AUDIO_WIFI_CONNECT_FAILED,		// 196	// 10075
	AUDIO_WIFI_CONNECT_SUCCESS,		// 197	// 10076
	AUDIO_WIFI_NOT_CONFIGURED,		// 198	// 10077

	AUDIO_DEVICE_SERIAL_NUMBER,     // 199	// 10078
	AUDIO_DEVICE_REG_SUCCESS,       // 200	// 10079
	AUDIO_DEVICE_REG_FAILED,        // 201	// 10080
	AUDIO_EXTRA_001,			    // 202	// 10081
	AUDIO_EXTRA_002,     			// 203	// 10082
	AUDIO_EXTRA_003,     			// 204	// 10083
	AUDIO_EXTRA_004,			    // 205	// 10084
	AUDIO_EXTRA_005,			    // 206	// 10085
	
	OTH_AUDIO_000,					// 207	// 10086
	
	OTH_AUDIO_001,							// 10087
	OTH_AUDIO_002,
	OTH_AUDIO_003,							// 10089
	OTH_AUDIO_004,							// 10090
	OTH_AUDIO_005,
	OTH_AUDIO_006,
	OTH_AUDIO_007,
	OTH_AUDIO_008,
	OTH_AUDIO_009,
	OTH_AUDIO_010,
	
	OTH_AUDIO_011,
	OTH_AUDIO_012,
	OTH_AUDIO_013,							// 10099
	OTH_AUDIO_014,							// 10100
	OTH_AUDIO_015,
	OTH_AUDIO_016,
	OTH_AUDIO_017,
	OTH_AUDIO_018,
	OTH_AUDIO_019,
	OTH_AUDIO_020,
	
	OTH_AUDIO_021,
	OTH_AUDIO_022,
	OTH_AUDIO_023,							// 10109
	OTH_AUDIO_024,							// 10110
	OTH_AUDIO_025,
	OTH_AUDIO_026,
	OTH_AUDIO_027,
	OTH_AUDIO_028,
	OTH_AUDIO_029,
	OTH_AUDIO_030,
	
	OTH_AUDIO_031,
	OTH_AUDIO_032,
	OTH_AUDIO_033,							// 10119
	OTH_AUDIO_034,							// 10120
	OTH_AUDIO_035,
	OTH_AUDIO_036,
	OTH_AUDIO_037,
	OTH_AUDIO_038,
	OTH_AUDIO_039,	
	OTH_AUDIO_040,
	
	OTH_AUDIO_041,
	OTH_AUDIO_042,
	OTH_AUDIO_043,							// 10129
	OTH_AUDIO_044,							// 10130
	OTH_AUDIO_045,
	OTH_AUDIO_046,
	OTH_AUDIO_047,
	OTH_AUDIO_048,
	OTH_AUDIO_049,
	OTH_AUDIO_050,
	
	OTH_AUDIO_051,
	OTH_AUDIO_052,
	OTH_AUDIO_053,							// 10139
	OTH_AUDIO_054,							// 10140
	OTH_AUDIO_055,
	OTH_AUDIO_056,
	OTH_AUDIO_057,
	OTH_AUDIO_058,
	OTH_AUDIO_059,
	OTH_AUDIO_060,
	
	OTH_AUDIO_061,
	OTH_AUDIO_062,
	OTH_AUDIO_063,							// 10149
	OTH_AUDIO_064,							// 10150
	OTH_AUDIO_065,
	OTH_AUDIO_066,
	OTH_AUDIO_067,
	OTH_AUDIO_068,
	OTH_AUDIO_069,
	OTH_AUDIO_070,
	
	OTH_AUDIO_071,
	OTH_AUDIO_072,
	OTH_AUDIO_073,							// 10159
	OTH_AUDIO_074,							// 10160
	OTH_AUDIO_075,
	OTH_AUDIO_076,
	OTH_AUDIO_077,
	OTH_AUDIO_078,
	OTH_AUDIO_079,
	OTH_AUDIO_080,
	
	OTH_AUDIO_081,
	OTH_AUDIO_082,
	OTH_AUDIO_083,							// 10169
	OTH_AUDIO_084,							// 10170
	OTH_AUDIO_085,
	OTH_AUDIO_086,
	OTH_AUDIO_087,
	OTH_AUDIO_088,
	OTH_AUDIO_089,
	OTH_AUDIO_090,
	
	OTH_AUDIO_091,
	OTH_AUDIO_092,
	OTH_AUDIO_093,							// 10179
	OTH_AUDIO_094,							// 10180
	OTH_AUDIO_095,
	OTH_AUDIO_096,
	OTH_AUDIO_097,
	OTH_AUDIO_098,
	OTH_AUDIO_099,					// 306	// 10185

	OTH_AUDIO_RUPEES,				// 307	// 10186
	OTH_AUDIO_PAISE,				// 308	// 10187
	OTH_AUDIO_AND,					// 309  // 10188
	OTH_AUDIO_HUNDRED,				// 310	// 10189
	OTH_AUDIO_THOUSAND,				// 311	// 10190
	OTH_AUDIO_LAKH,					// 312  // 10191
	OTH_AUDIO_CRORE,				// 313	// 10192
	OTH_AUDIO_RECVD_PAYMENT_OF,		// 314	// 10193
	OTH_AUDIO_LAST_RECVD,			// 315	// 10194
	OTH_AUDIO_CNF_CODE_H,			// 316	// 10195
	OTH_AUDIO_THANK_YOU,			// 317	// 10196
	
	AUDIO_INDEX_MAX
};

*/

// #else

/*
enum
{
	AUDIO_000,		// 121

	AUDIO_001,
	AUDIO_002,
	AUDIO_003,		// 124
	AUDIO_004,
	AUDIO_005,		// 126
	AUDIO_006,
	AUDIO_007,
	AUDIO_008,
	AUDIO_009,
	AUDIO_010,		// 131
	
	AUDIO_011,		// 132
	AUDIO_012,
	AUDIO_013,
	AUDIO_014,
	AUDIO_015,
	AUDIO_016,
	AUDIO_017,
	AUDIO_018,
	AUDIO_019,
	AUDIO_020,		// 141
	
	AUDIO_021,		// 142
	AUDIO_022,
	AUDIO_023,
	AUDIO_024,
	AUDIO_025,
	AUDIO_026,
	AUDIO_027,
	AUDIO_028,
	AUDIO_029,
	AUDIO_030,		// 151
	
	AUDIO_031,		// 152
	AUDIO_032,
	AUDIO_033,
	AUDIO_034,
	AUDIO_035,
	AUDIO_036,
	AUDIO_037,
	AUDIO_038,
	AUDIO_039,	
	AUDIO_040,		// 161
	
	AUDIO_041,		// 162
	AUDIO_042,
	AUDIO_043,
	AUDIO_044,
	AUDIO_045,
	AUDIO_046,
	AUDIO_047,
	AUDIO_048,
	AUDIO_049,
	AUDIO_050,		// 171
	
	AUDIO_051,		// 172
	AUDIO_052,
	AUDIO_053,
	AUDIO_054,
	AUDIO_055,
	AUDIO_056,
	AUDIO_057,
	AUDIO_058,
	AUDIO_059,
	AUDIO_060,		// 181
	
	AUDIO_061,		// 182
	AUDIO_062,
	AUDIO_063,
	AUDIO_064,
	AUDIO_065,
	AUDIO_066,
	AUDIO_067,
	AUDIO_068,
	AUDIO_069,
	AUDIO_070,		// 191
	
	AUDIO_071,		// 192
	AUDIO_072,
	AUDIO_073,
	AUDIO_074,
	AUDIO_075,
	AUDIO_076,
	AUDIO_077,
	AUDIO_078,
	AUDIO_079,
	AUDIO_080,		// 201
	
	AUDIO_081,		// 202
	AUDIO_082,
	AUDIO_083,
	AUDIO_084,
	AUDIO_085,
	AUDIO_086,
	AUDIO_087,
	AUDIO_088,
	AUDIO_089,
	AUDIO_090,		// 211

	AUDIO_091,		// 212
	AUDIO_092,
	AUDIO_093,
	AUDIO_094,
	AUDIO_095,
	AUDIO_096,
	AUDIO_097,
	AUDIO_098,
	AUDIO_099,		// 220
	
	AUDIO_RUPEES,				// 221	// 20001
	AUDIO_PAISE,				// 222	// 20002
	AUDIO_AND,					// 223	// 20003
	AUDIO_SHUTTING_DOWN,		// 224	// 20004
	// AUDIO_EXTRA,				// 224	// 20004 *
	
	AUDIO_HUNDRED, 				// 225	// 40001
	AUDIO_THOUSAND,				// 226	// 40002
	AUDIO_LAKH,					// 227	// 40003
	AUDIO_CRORE,				// 228	// 40004

	AUDIO_DEVICE_STARTING_UP,	// 229	// 50001
	AUDIO_RECVD_PAYMENT_OF,		// 230	// 50002
	AUDIO_LAST_RECVD,			// 231	// 50003
	AUDIO_CNF_CODE_H,			// 232	// 50004
	
	AUDIO_SEPARATOR,			// 233	// 50005
	AUDIO_SUCCESS,				// 234	// 50006
	AUDIO_FAILURE,				// 235	// 50007
	AUDIO_THANK_YOU,			// 236	// 50008

	AUDIO_SHUT_MESSAGE,			// 237	// 50009
	
	AUDIO_MIN_VOLUME,				// 238	// 50010
	AUDIO_MAX_VOLUME,				// 239	// 50011
	
	AUDIO_BAT_LOW_PC,				// 240	// 50012
	AUDIO_BAT_LOW_SD,				// 241	// 50013
	AUDIO_CHARGING,					// 242	// 50014
	AUDIO_BATTERY_PERCENT,			// 243	// 50015
	AUDIO_SIGNAL_STRENGTH,			// 244	// 50016

	AUDIO_CONNECTING,				// 245	// 50017
	AUDIO_RECONNECTING,				// 246	// 50018 *
	AUDIO_SERVER_CONNECT_SUCCESS,	// 247	// 50019
	AUDIO_SERVER_CONNECT_FAILED,	// 248	// 50020	
	
	AUDIO_START_UPDATE,				// 249	// 50021
	AUDIO_UPDATE_FAILED,			// 250	// 50022
	AUDIO_UPDATE_SUCCESS,			// 251	// 50023
	AUDIO_DOWNLOAD_FAILED,			// 252	// 50024
	AUDIO_DOWNLOAD_SUCCESS,			// 253	// 50025
	AUDIO_SETUP_FAILED,				// 254	// 50026
	AUDIO_SETUP_SUCCESS,			// 255	// 50027
	
	AUDIO_CHECKING_WIFI,			// 256	// 50028
	AUDIO_CONNECTING_WIFI,			// 257	// 50029
	AUDIO_CONNECTING_SIM,			// 257	// 50030
	AUDIO_NETWORK_MODE_SIM,			// 258	// 50031
	AUDIO_NETWORK_MODE_WIFI,		// 259	// 50032
	AUDIO_ONLY_SIM_NETWORK,			// 260	// 50033
	AUDIO_ONLY_WIFI_NETWORK,		// 261	// 50034
	AUDIO_WIFI_AP_MODE,				// 262	// 50035
	AUDIO_WIFI_AK_MODE,				// 263	// 50036
	AUDIO_WIFI_CONFIG_FAILED,		// 264	// 50037
	AUDIO_WIFI_CONFIG_SUCCESS,		// 265	// 50038
	AUDIO_WIFI_CONNECT_FAILED,		// 266	// 50039
	AUDIO_WIFI_CONNECT_SUCCESS,		// 267	// 50040
	AUDIO_WIFI_NOT_CONFIGURED,		// 267	// 50041

	AUDIO_MOGO,						// 268	// 50042
	
	AUDIO_INDEX_MAX
};
*/


enum {

	AUDIO_INDEX_MAX
};

typedef struct 
{
	int id;
	int fileid;
	char *tts;
}audioMapStruct;

typedef struct 
{
	int id;
	int id1;
	int id2;
	int id3;
	char *tts;
}audioJointStruct;

extern const audioMapStruct g_audioMapTbl[];

extern void set_audio_type(char * sAudioType);
extern char * get_audio_type(void);
extern void get_audio_file_name(char * fileName, int fileId);
extern void format_audio_file_name(char * fileName, int fileId, char * sAudioType);

extern const audioMapStruct g_audioMapTbl[];
extern void get_audio_fileId(audioMapStruct *audioMapTbl);

extern int get_nv_audio_data_len(int nAudioId);

int app_audio_tts_callback(void);
int app_audio_amr_callback(void);
int app_audio_mp3_callback(void);

int app_get_audio_type(void);
int app_audio_play_file(int fileId);


int app_audio_play(int index);
int app_audio_play_single(int index, int id);
int app_audio_play_multi(int index1, int index2, int index3);


int app_audio_save_data_from_server(char *data, int len, char *name);
int app_print_debug_buff(char *buff, int len);
int app_audio_get_file_info(int index, char *buff, int buff_len, char *filename, char *audioid);
int app_audio_get_file_data(char *recv_buff, char *out_buff, int buff_len, char *audioId, int *olen );
int app_audio_download_from_server(int count);
int app_audio_get_list_from_server(int nformat);
int app_audio_check_update();





#endif
