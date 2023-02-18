#ifndef _VENDOR_H
#define _VENDOR_H

//�˱����ڼ�¼������ƫ��
typedef enum
{
	TEST_ITEM_POWER_KEY_OFFSET    = 0,
	TEST_ITEM_REPLAY_KEY_OFFSET   = 1,
	TEST_ITEM_UP_KEY_OFFSET       = 2,
	TEST_ITEM_DOWN_KEY_OFFSET     = 3,
	TEST_ITEM_LED_OFFSET          = 4,
	TEST_ITEM_VOLTAGE_OFFSET      = 5,
	TEST_ITEM_SPEAKER_OFFSET      = 6,
	TEST_ITEM_SIM_OFFSET          = 7,
	TEST_ITEM_IMEI_OFFSET         = 8,
	TEST_ITEM_WIFI_VERSION_OFFSET = 9,
	TEST_ITEM_RSSI_OFFSET         = 10,
	TEST_ITEM_NETSTATUS_OFFSET    = 11,
	TEST_ITEM_PDPSTATUS_OFFSET    = 12,
}TEST_ITEM_OFFSET_E;

//�˱����ڼ�¼�������Խ���и�������ƫ��
typedef enum
{
	TEST_KEYPAD_POWER_KEY_OFFSET  = 0,
	TEST_KEYPAD_UP_KEY_OFFSET     = 1,
	TEST_KEYPAD_DOWN_KEY_OFFSET   = 2,
	TEST_KEYPAD_REPLAY_KEY_OFFSET = 3,
}TEST_KEYPAD_OFFSET_E;

typedef enum
{
	DEVICE_WIFI_ENABLE_TRUE  = 0x11,
	DEVICE_WIFI_ENABLE_FALSE = 0x22,
}DEVICE_WIFI_ENABLE_E;

typedef enum
{
	DEVICE_GPRS_ENABLE_TRUE  = 0x33,
	DEVICE_GPRS_ENABLE_FALSE = 0x44,
}DEVICE_GPRS_ENABLE_E;

// ������������
#define LIFECYCLE_APP     0xAA
#define LIFECYCLE_VENDOR  0xBB

// ���������
#define PORT_ID			6

// USBָ���
#define USB_CMD_FASTBOOT             "fastboot"            //��fastboot
#define USB_CMD_START_KEYPAD_TEST    "start_keypad_test"   //��ʼ��������
#define USB_CMD_GET_KEYPAD_TEST      "get_keypad_test"     //��ȡ�������Խ��
#define USB_CMD_STOP_KEYPAD_TEST     "stop_keypad_test"    //������������
#define USB_CMD_START_LED_TEST       "start_led_test"      //��ʼLED����
#define USB_CMD_STOP_LED_TEST        "stop_led_test"       //����LED����
#define USB_CMD_START_SPEAKER_TEST   "start_speaker_test"  //��ʼ���Ȳ���
#define USB_CMD_STOP_SPEAKER_TEST    "stop_speaker_test"   //�������Ȳ���
#define USB_CMD_IMEI                 "IMEI"                //��ȡIMEI
#define USB_CMD_SIM_INFO             "sim_info"            //��ȡSIM����Ϣ
#define USB_CMD_VOLTS                "volts"               //��ȡ��ص�ѹ
#define USB_CMD_RSSI                 "rssi"                //��ȡ�ź�ǿ��
#define USB_CMD_NETSTATUS            "netstatus"           //����״̬
#define USB_CMD_PDPSTATUS            "pdpstatus"           //PDP״̬
#define USB_CMD_WIFI_INIT            "wifi_init"           //WIFI��ʼ��ָ��
#define USB_CMD_WIFI_INIT_STATUS     "wifi_init_status"    //��ѯWIFI��ʼ��״̬
#define USB_CMD_GPRS_ENABLE          "gprs_enable"         //ʹ��GPRS
#define USB_CMD_GPRS_DISABLE         "gprs_disable"        //ȥʹ��GPRS
#define USB_CMD_WIFI_ENABLE          "wifi_enable"         //ʹ��WIFI
#define USB_CMD_WIFI_DISABLE         "wifi_disable"        //ȥʹ��WIFI
#define USB_CMD_WIFI_VERSION         "wifi_version"        //��ȡWIFI�̼��汾
#define USB_CMD_REGISTER_URL         "register_url:"       //д�����register_url
#define USB_CMD_MQTT_SERVER          "mqtt_server:"        //д�����mqtt_server
#define USB_CMD_AUTHKEY_N            "authkey_n:"          //д�����authkey_n
#define USB_CMD_DEVICEID             "deviceid:"           //д�����deviceid
#define USB_CMD_CLIENTID             "clientid:" 
#define USB_CMD_PRODUCTKEY           "product_key:"
#define USB_CMD_DEVSECRET            "device_secret:"
#define USB_CMD_REGIONID             "region_id:"
#define USB_CMD_KEYSECRET            "key_secret:"

#define USB_CMD_MANU_TO_APP                	"MANU_TO_APP"               //��Ӧ��״̬
#define USB_CMD_MANU_TO_VENDOR             	"MANU_TO_VENDOR"            //�س���״̬
#define USB_CMD_MANU_SN                   	"MANU_SN="
#define USB_CMD_MANU_SERV_HOST            	"MANU_SERV_HOST="
#define USB_CMD_MANU_SERV_PORT            	"MANU_SERV_PORT="
#define USB_CMD_MANU_PRODUCT_MODEL          "MANU_PRODUCT_MODEL="
#define USB_CMD_MANU_PROJ_ID              	"MANU_PROJ_ID="
#define USB_CMD_MANU_GET_SN               	"MANU_GET_SN"
#define USB_CMD_MANU_GET_SIMINFO          	"MANU_GET_SIMINFO"
#define USB_CMD_MANU_GET_IMEI          		"MANU_GET_IMEI"


#define USB_CMD_APP_GET_SN               	"APP_GET_SN"
#define USB_CMD_APP_GET_SIMINFO          	"APP_GET_SIMINFO"
#define USB_CMD_APP_GET_IMEI          		"APP_GET_IMEI"
#define USB_CMD_APP_GET_PLATFORM			"APP_GET_PLATFORM"
#define USB_CMD_APP_GET_APP_VERSION		 	"APP_GET_APP_VERSION"
#define USB_CMD_APP_GET_PRODUCT_MODEL	 	"APP_GET_PRODUCT_MODEL"
#define USB_CMD_APP_GET_PROJ_ID		 		"APP_GET_PROJ_ID"
#define USB_CMD_APP_GET_BATTERY_LEVEL  		"APP_GET_BATTERY_LEVEL"


// ������������
int set_Lifecycle(int life);
// ��ȡ��������
int get_lifecycle(void);

#endif
