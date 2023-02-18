#ifndef _QYY_MQTT_PUSH_SERVER_H_
#define _QYY_MQTT_PUSH_SERVER_H_

#include "MercuryDef.h"
#include "cjson.h"

#define QYY_FT_MQTT_PASSWORD_LEN	(20)

//#define PUSH_MSG_TLV_VERSION //使用tlv格式推送消息

typedef enum
{
	/* MIN */
	QYY_MSG_TYPE_RESERVE_MIN	= 0,

	QYY_MSG_TYPE_VOICE_TRANS1	= 1,   			// 交易播报语音数据
	QYY_MSG_TYPE_OTA	= 2,           			// 固件和配置更新提醒，设备可连接配置服务器，获取并更新固件和设备配置
	QYY_MSG_TYPE_VOICE_CUSTOMIZE = 3, 			// 自定义播报数据
	QYY_MSG_TYPE_DISCOUNT	= 4,       			// 折扣或优惠信息
	QYY_MSG_TYPE_VERF_CODE = 5,       			// 动态验证码
	QYY_MSG_TYPE_SET_POWER = 6,       			// 设备重启或关机
	QYY_MSG_TYPE_CFG_DYN   = 7,       			// 动态配置（开机播放语音、男声/女声、音量、TTS/AMR/PCM/MP3、方言种类、配置服务器URL、服务器监听主题）。
	QYY_MSG_TYPE_CFG_SERV  = 8,       			// 设置配置服务器（IP和端口）
	//QYY_MSG_TYPE_CFG_TOPIC = 9,       		// 设置服务器监听主题（topic）
	// QYY_MSG_TYPE_SET_MQTT_SERV  = 9,    		// 设置MQTT服务器——待定
	QYY_MSG_TYPE_QR_CODE  = 10,    				// 发送动态二维码（收款方信息、交易信息、二维码信息）
	QYY_MSG_TYPE_AUDIO_UPDATE  = 11,    		// 升级音频文件
	QYY_MSG_TYPE_VOICE_TRANS2 = 12,             // 交易播报语音数据第二种格式
	QYY_MSG_TYPE_VOICE_TRANS3 = 13,             // 交易播报语音数据第三种格式

	QYY_TEST_GET_SYS_INFO = 240,				// 用于设备查错，获取设备系统环境信息

	/* MAX */
	QYY_MSG_TYPE_MAX,
}qyy_msg_category_enum;

struct msg_json_tbl
{
	uint8_t type;

	int (*func)(cJSON *data);
};


#ifdef PUSH_MSG_TLV_VERSION
#define QYY_MSGP_TAG_VOICE_TEMPLATE	 0xDF01   //语音模板ID，数据取值范围从0x01到0xFF。
#define QYY_MSGP_TAG_PAY_AMOUNT      0xDF02   //支付金额，数据值为4字节整数（小端），单位为分
#define QYY_MSGP_TAG_VOICE_CUSTOMIZE 0xDF03   //自定义语音播放数据
#define QYY_MSGP_TAG_DCOUNT_TYPE	 0xDF04   //优惠类型
#define QYY_MSGP_TAG_DCOUNT_AMOUNT	 0xDF05   //优惠金额，数据值为4字节整数（小端），单位为分
#define QYY_MSGP_TAG_DCOUNT_VOICE	 0xDF06   //自定义优惠类型语音播放数据
#define QYY_MSGP_TAG_AUTH_CODE	     0XDF07   //动态验证码，ASCII编码
#define QYY_MSGP_TAG_FIRM_VER	     0xDF08   //固件的最新版本号。3个字节的版本号：主版本号+次版本号+小版本号。（可选）4字节的build号（小端）
#define QYY_MSGP_TAG_APPCFG_VER      0xDF09   //应用配置的最新版本号。3个字节的版本号：主版本号+次版本号+小版本号。（可选）4字节的build号（小端）。
#define QYY_MSGP_TAG_DEV_CONTROL     0xDF0A   //设备控制指令：设备重启（0x1）、设备关机（0x2）
#define QYY_MSGP_TAG_VOICE_TYPE	     0xDF0B   //语音类型：男声（0x1）、女声（0x2）、其它配音（0x3~0xFF）
#define QYY_MSGP_TAG_VOICE_VOL  	 0xDF0C   //播报语音的音量，百分比的整数值（0~100），0表示静音，1表示最小音量，100表示最大音量
#define QYY_MSGP_TAG_VOICE_FMT  	 0xDF0D   //语音数据格式，单字节，TTS（0x1）、AMR（0x2）、PCM（0x3）、MP3（0x4）
#define QYY_MSGP_TAG_DIALECT_ID	     0XDF0E   //方言种类，4字节小端
#define QYY_MSGP_TAG_SERV_TOPIC 	 0xDF0F   //服务器监听设备消息的主题
#define QYY_MSGP_TAG_CFGSERV_IP  	 0xDF10   //配置服务器的IP地址
#define QYY_MSGP_TAG_CFGSERV_PORT    0xDF11   //配置服务器的服务端口

#define TLV_VALUE_LEN_MAX 1024

typedef struct qyy_msg_push_tlv
{
	uint16_t m_tag;
	uint16_t m_len;

	unsigned char m_value[TLV_VALUE_LEN_MAX];
}msg_tlv_struct;

typedef struct qyy_msg_push
{
#define SIZE_QYY_MQTT_PUSHSERVER_HASH	32
	uint32_t pushid;
	uint8_t version;	// fixed to 1

	/* REF : DS0001 */
	uint8_t type;

	uint16_t length;	// length of data

	// data
	unsigned char data[];
}qyy_msg_push_struct;


struct msg_cmd_tbl
{
	uint8_t type;

	int (*func)(unsigned char *data, int len);
};
#endif


#endif
