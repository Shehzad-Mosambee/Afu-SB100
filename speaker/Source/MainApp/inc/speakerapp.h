#ifndef SPEAKER_APP_H
#define SPEAKER_APP_H

#include "common.h"
#include "http.h"
#include "SDK_API_LED.h"

#define APP_VERSION            		"2.1.7" 
#define APP_VERSION_STR            	"Z100" 
#define FW_BUILD_VERSION            1

#define MOS_AUDIO_VERSION			"1.2"

// #define APP_VERSION                "1.1.1"
// #define FW_BUILD_VERSION            1

#define DEFAULT_KEEP_ALIVE  100

#define MQTT_RECONNECT -1
#define CUSTOMER_NAME "szaf"

#define SERVER_HOST                 "152.136.213.55"
#define SERVER_PORT                 10501

#define SZAF                 "/szanfu-admin"
#define SERVER_GET_CFG       SZAF"/speaker/dev-config"
#define SERVER_REPORT_LBS    SZAF"/speaker/lbs-info"
#define SERVER_CHECK_UPDATE  SZAF"/speaker/check-update"
#define REPORT_CFG_UPDATE_RESULT    SZAF"/speaker/config-update-result"							
#define REPORT_FOTA_RESULTS         SZAF"/speaker/fw-update-result"  


#define VOICE_TEMPLATE_PAY        1
#define VOICE_TEMPLATE_DISCOUNT   2
#define VOICE_TEMPLATE_VERIFYCODE 3

#define FACTORY_NO                        "01"
#define DEVICE_TYPE                       "01" //������
#define APP_NAME                          "Speaker"

#define RET_SUCCESS           0
#define ERR_DEVICE_DISMATCH   1
#define ERR_VOICE_NOREQID     2
#define ERR_VOICE_TYPEERR     3

#define ERR_MEMORY_NOT_ENOUGH 1007 
#define ERR_DATA_PARSE_FAIL   1008
#define ERR_UPDATE_FAIL       1009

#define HTTPS_REQUEST_FAIL      11
#define HTTPS_STATUS_CODE_ERROR 12


#define OTA_NOT_UPDATE     0 //����Ҫ��OTA����
#define OTA_NEED_UPDATE    1 //��Ҫ��OTA����

#define MSG_DATA_LEN 512

#define KEY_LEN  128

#define REG_FLAG 0x5A5B

#define BAT_POWER_OFF 10
#define BAT_AUDIO_WARNING15	15
#define BAT_AUDIO_WARNING20	20
#define BAT_AUDIO_WARNING 25

#define WIFI_STATUS_IDLE    0
#define WIFI_STATUS_CONFIG  1
#define WIFI_STATUS_CONNECTING  2
#define WIFI_STATUS_CONNECT 3
#define WIFI_STATUS_CONNECTFAIL 4
#define WIFI_STATUS_APP     5



#define	APP_STATUS_IDLE          0      //idle
#define	APP_STATUS_WIFICFG       1   //����wifi����
#define	APP_STATUS_WIFICFGFAIL   2    //��������ʧ��
#define	APP_STATUS_CONNECTING    3   //������������
#define	APP_STATUS_CONNECTSUCC   4   //�������ӳɹ�
#define	APP_STATUS_CONNECTFAIL   5   //��������ʧ��
#define	APP_STATUS_SERV_CONNTING 6   //�������ӷ�����
#define	APP_STATUS_SERV_CONNSUC  7   //���������ӳɹ�
#define	APP_STATUS_SERV_CONNFAIL 8   //����������ʧ��

typedef enum{
	DOWNLOAD_FAILED,
	UPDATE_FAILED,
	UPDATE_SUCCESS,
	MAX_STATUS = 0x7fffffff
}update_status;


extern char app_status;

typedef struct
{
	char SN[32];				// SN
	char model[16];             //��Ʒ�ͺ�
	char item_no[16];           //��Ŀ���
	char cfg_serv_host[64];				// Server Host
	int  cfg_serv_port;				// Server Port
	int  app_config_ver;
	int  bind;                //0:δ�󶨣�1����
	int  cfg_done;            //��¼�Ƿ��Ѿ���ɶ�ȡ������Ϣ
	// int  hw_capability;				//��¼Ӳ�����ò���
}AppInfoStruct;

typedef struct 
{
	char ip[64];      //MQTT broker��������IP��ַ�����磺"151.123.107.55"
	int  port;        // MQTT broker�������ķ���˿ڣ�������ֵ��1~65535
	char clientid[128]; //MQTT�������ĵ�¼id
	char username[128]; //MQTT�������ĵ�¼�û���
	char password[128]; //MQTT�������ĵ�¼����
	char productkey[64]; //MQTT ��Ʒ��Կ���ڰ����Ƶ�mqtt�豸ͨ���Ƿ���productkey��ota��������
	char subtopic[128]; //�豸������Ϣ�����⣬������ͨ�����������豸������Ϣ��
	char pubtopic[128]; //�豸������Ϣ�����⣬�豸ͨ��������������������豸����״̬����Ϣ
	int  keep_alive;   //MQTT���������ʱ�䣬��λΪ��
	int  cert_update;  // RFU���Ƿ���Ҫ����SSL֤�飬0�������£�1�������
}AppMqttCfgStruct;

typedef struct 
{
	int type;   //�������ͣ�������ֵ��������1����Ů����2��������������3~255��
	int volume; //����������������������ֵ���ٷֱȵ�����ֵ��0~100����0��ʾ������1��ʾ��С������100��ʾ���������
	int format; //�������ݸ�ʽ��������ֵ��TTS��1����AMR��2����PCM��3����MP3��4��
	int dialect_id; //RFU���������࣬������ֵ
}AppAudioCfgStruct;


typedef struct 
{
	char  ota_url[256];
	char  New_Version[16];
	char  check_value[65];
	int   build_no;
	int   file_len;
	int   fota_type; //�Ƿ�ʹ�ò��
	int   sign_method; //ǩ������ 1: MD5     2: SHA256
}OtaInfoStruct;

#if 1
struct st_ft_appinfo
{
	//char sn[QYY_SIZE_NV_GENERAL];
	//char productkey[QYY_SIZE_NV_GENERAL];
	//char serv_host[QYY_SIZE_NV_GENERAL];
	//char serv_port[QYY_SIZE_NV_GENERAL];
	char sn[16];
	char productkey[64 + 1];
	char serv_host[64];
	char serv_port[16];
};
#endif


#define VOICE_POWERON  1
#define VOICE_POWEROFF 2
#define VOICE_CUSTOM   3
void PlayPowerAudio(int type);


extern char deviceid_buf[20];  //�����豸�ţ�����ƴ������ĸ+���ֱ�ţ�һ��16λ���ο� yx00000000000001
extern char devicekey[33]; //�豸��Կ
extern char wifi_status;

//����ģʽ�ӿ�
int vendor_Keypad_init(void);
int vendor_LED_init( void );
int vendor_speaker_init( void );
int qyy_vendor_check_passall(int flag);

BOOL device_gprs_enable( void );
BOOL device_wifi_enable( void );


//������Ӧ�ýӿ�
int speaker_app_init(void);
void speaker_run(void);
int wifi_mode_connect(void);
int speaker_close_mqtt(void);
int speaker_connect_server(void);

//OTA�����ӿ�
int speaker_fota_init(void);
int speaker_fota_query(void);
int speaker_fota_notify_result(void);
int fota_start_down(void);
int fota_start_update(void);

//���������ӿ�
int netcfg_AP(void);
int netcfg_airkiss(void);

int speaker_https( const char *url, const char* post_data, int post_data_len, data_recv_cb_t cb);
//void hex2str(uint8_t *input, uint16_t input_len, char *output);
int str2int(char *s);

void hmac_sha1(const uint8_t *msg, uint32_t msg_len, const uint8_t *key, uint32_t key_len, uint8_t output[20]);

int Keypad_init(void);
int amrfile_init(void);
int usbthread_init(void);

int get_lifecycle(void);
int gettimestamp(char *buf);


void ledblink(int   led );
void led_set(LED_NUM_E led, BOOL on);

int amount_play(double amount);

int app_voiceplay( int tts, const char *message );
void app_play_error_code(int code);
unsigned long app_mqtt_get_handle(void);


void netcfg_voice_play_loop( int addr );
void qyyft_switch_to_4g( int type );
void qyyft_wait_key_queue_finish(int wait_max_time, int atry_after_empty);
void qyyft_reboot4apply(int type);
void qyyft_set_restart_reason(int reset_reason);
int qyyft_tts_queue_init(void *arg);
void qyyft_start_power_monitor(void *arg);
int qyyft_check_usb_online(void);

//���ݵ�����߻�ȡ��Ӧ�ĵ����ٷֱ�
uint32 app_get_battery_percent(void);

void *vendor_cb_usb_in(void *arg);
void *vendor_cb_usb_out(void *arg);

int get_dev_config_info(void);

extern void Dbg_dump(const unsigned char *buff, uint32_t len);
extern uint32 CharString2UnicodeString(char * destination, const char * string);
extern void app_hex2str( unsigned char *source, unsigned int source_len, unsigned char *dst );

#endif

