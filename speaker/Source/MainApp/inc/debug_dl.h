#ifndef _DEBUG_H_
#define _DEBUG_H_

/****
 * MESSAGE ID
 * 	1:Enable Fake;
 */
#define QYY_MSG_CONFIG_FAKE_ID_ENABLE	0
#define QYY_MSG_CONFIG_FAKE_ID_VALUE	(0)


/****
 * Agree Key & MQTT & Get Config
 * Use data from qyy(1).txt(FEITIAN).
 */
#define USE_FAKE_MQTT_INFO_FEITIAN_AGREE_KEY	0	// 1:use. [ConfigServer]Agree Key Parse : key, random, sign;
#if 0
#define QYY_FAKE_AGREE_KEY_KEY		"dYjhH7L1n4m97teLloUpTYV79oDEUMk5/YECglZQp8k="
#define QYY_FAKE_AGREE_KEY_RANDOM	"9HSDC4MBBF1LV8LM"
#define QYY_FAKE_AGREE_KEY_SIGN		"/1fYya/3NUPAQmjC9aR8hzDeKCk="
#endif
#if 1
#define QYY_FAKE_AGREE_KEY_KEY		"dm9tb01mYk01YWQ1SVlBSWdnVjhNTzhYWW5QTFVOT2I="
#define QYY_FAKE_AGREE_KEY_RANDOM	"9ZS1STTNV7KWXLTI"
#define QYY_FAKE_AGREE_KEY_SIGN		"Mcy0ADBYIeL8gaGAwaznYbizZ0E="
#endif
#define QYY_USE_TEMP_DEVICE_KEY		0	// 0:use act key
//#define QYY_TEMP_DEVICE_KEY	
//{0x75, 0x88, 0xe1, 0x1f, 0xb2, 0xf5, 0x9f, 0x89, 0xbd, 0xee, 0xd7, 0x8b, 0x96, 0x85, 0x29, 0x4d, 0x85, 0x7b, 0xf6, 0x80, 0xc4, 0x50, 0xc9, 0x39, 0xfd, 0x81, 0x02, 0x82, 0x56, 0x50, 0xa7, 0xc9}
#define QYY_TEMP_DEVICE_KEY	\
{0x76, 0x6f, 0x6d, 0x6f, 0x4d, 0x66, 0x62, 0x4d, 0x35, 0x61, 0x64, 0x35, 0x49, 0x59, 0x41, 0x49, 0x67, 0x67, 0x56, 0x38, 0x4d, 0x4f, 0x38, 0x58, 0x59, 0x6e, 0x50, 0x4c, 0x55, 0x4e, 0x4f, 0x62}

#define USE_FAKE_MQTT_INFO_FEITIAN_GET_CONFIG	1	// 1:use. [ConfigServer]Request message of Get Config;
#define USE_FAKE_MQTT_INFO_FEITIAN_MQTT_UNAME	0	// 1:use. [PushServer]	MQTT username_strcat_plain & username_base64;
#define USE_FAKE_RESPONSE_DATA_GET_CONFIG_ENABLE	0	// 1:use fake. [ConfigServer]Get Config Parse - response.message.data;


/****
 * LTE Base Stations` Info
 * Controled by QYY_DEV_API_USE_FAKE
 */
#define USE_FAKE_LTE_BASE_STATION_DRAMALIFE	0x8
#define USE_FAKE_LTE_BASE_STATION_FT_QYY1TXT	0x4
#define USE_FAKE_LTE_BASE_STATION_TYPE	USE_FAKE_LTE_BASE_STATION_FT_QYY1TXT
/****
 * LTE Module Info
 * Controled by QYY_DEV_API_USE_FAKE
 */
#if 0
// dramalife
#define FAKE_LTE_MODULE_INFO_imsi	"460046800400091"
#define FAKE_LTE_MODULE_INFO_ccid	"89860448161890070091"
#define FAKE_LTE_MODULE_INFO_imei	"861112058948747"
#endif
#if 0
// ft 1
#define FAKE_LTE_MODULE_INFO_imsi	"460046575005838"
#define FAKE_LTE_MODULE_INFO_ccid	"89860445101941223281"
#define FAKE_LTE_MODULE_INFO_imei	"861692040242572"
#endif
#if 1
// ft 2
#define FAKE_LTE_MODULE_INFO_imsi	"460045931208352"
#define FAKE_LTE_MODULE_INFO_ccid	"89860439101870028352"
#define FAKE_LTE_MODULE_INFO_imei	"869313044706668"



#endif




/****
 * BASIC INFO
 */
#if 0
/* SN */
#define USE_FAKE_SN_ENABLE	1
//#define USE_FAKE_SN_STRING	"1074071300001"
#define USE_FAKE_SN_STRING	"QYY_TEST_0001"


/* RANDOM */
#define USE_FAKE_RANDON_ENABLE	1
//#define USE_FAKE_RANDON_STRING	"00DC0465AA1FAD1D"
#define USE_FAKE_RANDON_STRING	"saGzGnUVWP52iFUg"


/* PRODUCT KEY */
#define USE_FAKE_PRODUCTKEY_ENABLE	1
//#define USE_FAKE_PRODUCTKEY_STRING      "84D48EECEF0568517743EB0A79565817101E9D3CFB1FCB7A72BBA1AD6E69A081"	// SN : 1074071300001
#define USE_FAKE_PRODUCTKEY_STRING	"F2E0FBD30A3A28290B49A2BCAABA77AACD1AF640F8B7D4AF2918EEC3A50F8EE7"	// SN : QYY_TEST_0001

/* OS VERSION */
#define USE_FAKE_OSVER_ENABLE	1
#define USE_FAKE_OSVER_VALUE	(1000)

/* AES - IV, KEY */
#define AES_PRODUCT_KEY_FIRST	1
#endif


#if 0
/* SN */
#define USE_FAKE_SN_ENABLE	1
#define USE_FAKE_SN_STRING	"1074071300001"
/* RANDOM */
#define USE_FAKE_RANDON_ENABLE	1
#define USE_FAKE_RANDON_STRING	"00DC0465AA1FAD1D"
/* PRODUCT KEY */
#define USE_FAKE_PRODUCTKEY_ENABLE	1
#define USE_FAKE_PRODUCTKEY_STRING      "84D48EECEF0568517743EB0A79565817101E9D3CFB1FCB7A72BBA1AD6E69A081"	// SN : 1074071300001
/* OS VERSION */
#define USE_FAKE_OSVER_ENABLE	1
#define USE_FAKE_OSVER_VALUE	(1000)
/* AES - IV, KEY */
#define AES_PRODUCT_KEY_FIRST	1
#endif


#if 1
/* SN */
#define USE_FAKE_SN_ENABLE	0		// NV!!!!
#define USE_FAKE_SN_STRING	"QYY_TEST_0003"
/* RANDOM */
#define USE_FAKE_RANDON_ENABLE	0
#define USE_FAKE_RANDON_STRING	"saGzGnUVWP52iFUg"
/* PRODUCT KEY */
#define USE_FAKE_PRODUCTKEY_ENABLE	0	// NV!!!!
#define USE_FAKE_PRODUCTKEY_STRING	"F2E0FBD30A3A28290B49A2BCAABA77AACD1AF640F8B7D4AF2918EEC3A50F8EE7"	// SN : QYY_TEST_0001
/* OS VERSION */
#define USE_FAKE_OSVER_ENABLE	1
#define USE_FAKE_OSVER_VALUE	(1000)
/* AES - IV, KEY */
#define AES_PRODUCT_KEY_FIRST	1
#endif

#endif
