#ifndef _QYY_MQTT_PUSH_SERVER_H_
#define _QYY_MQTT_PUSH_SERVER_H_

#include "MercuryDef.h"
#include "cjson.h"

#define QYY_FT_MQTT_PASSWORD_LEN	(20)

//#define PUSH_MSG_TLV_VERSION //ʹ��tlv��ʽ������Ϣ

typedef enum
{
	/* MIN */
	QYY_MSG_TYPE_RESERVE_MIN	= 0,

	QYY_MSG_TYPE_VOICE_TRANS1	= 1,   			// ���ײ�����������
	QYY_MSG_TYPE_OTA	= 2,           			// �̼������ø������ѣ��豸���������÷���������ȡ�����¹̼����豸����
	QYY_MSG_TYPE_VOICE_CUSTOMIZE = 3, 			// �Զ��岥������
	QYY_MSG_TYPE_DISCOUNT	= 4,       			// �ۿۻ��Ż���Ϣ
	QYY_MSG_TYPE_VERF_CODE = 5,       			// ��̬��֤��
	QYY_MSG_TYPE_SET_POWER = 6,       			// �豸������ػ�
	QYY_MSG_TYPE_CFG_DYN   = 7,       			// ��̬���ã�������������������/Ů����������TTS/AMR/PCM/MP3���������ࡢ���÷�����URL���������������⣩��
	QYY_MSG_TYPE_CFG_SERV  = 8,       			// �������÷�������IP�Ͷ˿ڣ�
	//QYY_MSG_TYPE_CFG_TOPIC = 9,       		// ���÷������������⣨topic��
	// QYY_MSG_TYPE_SET_MQTT_SERV  = 9,    		// ����MQTT��������������
	QYY_MSG_TYPE_QR_CODE  = 10,    				// ���Ͷ�̬��ά�루�տ��Ϣ��������Ϣ����ά����Ϣ��
	QYY_MSG_TYPE_AUDIO_UPDATE  = 11,    		// ������Ƶ�ļ�
	QYY_MSG_TYPE_VOICE_TRANS2 = 12,             // ���ײ����������ݵڶ��ָ�ʽ
	QYY_MSG_TYPE_VOICE_TRANS3 = 13,             // ���ײ����������ݵ����ָ�ʽ

	QYY_TEST_GET_SYS_INFO = 240,				// �����豸�����ȡ�豸ϵͳ������Ϣ

	/* MAX */
	QYY_MSG_TYPE_MAX,
}qyy_msg_category_enum;

struct msg_json_tbl
{
	uint8_t type;

	int (*func)(cJSON *data);
};


#ifdef PUSH_MSG_TLV_VERSION
#define QYY_MSGP_TAG_VOICE_TEMPLATE	 0xDF01   //����ģ��ID������ȡֵ��Χ��0x01��0xFF��
#define QYY_MSGP_TAG_PAY_AMOUNT      0xDF02   //֧��������ֵΪ4�ֽ�������С�ˣ�����λΪ��
#define QYY_MSGP_TAG_VOICE_CUSTOMIZE 0xDF03   //�Զ���������������
#define QYY_MSGP_TAG_DCOUNT_TYPE	 0xDF04   //�Ż�����
#define QYY_MSGP_TAG_DCOUNT_AMOUNT	 0xDF05   //�Żݽ�����ֵΪ4�ֽ�������С�ˣ�����λΪ��
#define QYY_MSGP_TAG_DCOUNT_VOICE	 0xDF06   //�Զ����Ż�����������������
#define QYY_MSGP_TAG_AUTH_CODE	     0XDF07   //��̬��֤�룬ASCII����
#define QYY_MSGP_TAG_FIRM_VER	     0xDF08   //�̼������°汾�š�3���ֽڵİ汾�ţ����汾��+�ΰ汾��+С�汾�š�����ѡ��4�ֽڵ�build�ţ�С�ˣ�
#define QYY_MSGP_TAG_APPCFG_VER      0xDF09   //Ӧ�����õ����°汾�š�3���ֽڵİ汾�ţ����汾��+�ΰ汾��+С�汾�š�����ѡ��4�ֽڵ�build�ţ�С�ˣ���
#define QYY_MSGP_TAG_DEV_CONTROL     0xDF0A   //�豸����ָ��豸������0x1�����豸�ػ���0x2��
#define QYY_MSGP_TAG_VOICE_TYPE	     0xDF0B   //�������ͣ�������0x1����Ů����0x2��������������0x3~0xFF��
#define QYY_MSGP_TAG_VOICE_VOL  	 0xDF0C   //�����������������ٷֱȵ�����ֵ��0~100����0��ʾ������1��ʾ��С������100��ʾ�������
#define QYY_MSGP_TAG_VOICE_FMT  	 0xDF0D   //�������ݸ�ʽ�����ֽڣ�TTS��0x1����AMR��0x2����PCM��0x3����MP3��0x4��
#define QYY_MSGP_TAG_DIALECT_ID	     0XDF0E   //�������࣬4�ֽ�С��
#define QYY_MSGP_TAG_SERV_TOPIC 	 0xDF0F   //�����������豸��Ϣ������
#define QYY_MSGP_TAG_CFGSERV_IP  	 0xDF10   //���÷�������IP��ַ
#define QYY_MSGP_TAG_CFGSERV_PORT    0xDF11   //���÷������ķ���˿�

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
