#ifndef _VENDOR_H
#define _VENDOR_H

//此表用于记录测试项偏移
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

//此表用于记录按键测试结果中各个按键偏移
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

// 工厂生命周期
#define LIFECYCLE_APP     0xAA
#define LIFECYCLE_VENDOR  0xBB

// 串口输出口
#define PORT_ID			6

// USB指令表
#define USB_CMD_FASTBOOT             "fastboot"            //打开fastboot
#define USB_CMD_START_KEYPAD_TEST    "start_keypad_test"   //开始按键测试
#define USB_CMD_GET_KEYPAD_TEST      "get_keypad_test"     //获取按键测试结果
#define USB_CMD_STOP_KEYPAD_TEST     "stop_keypad_test"    //结束按键测试
#define USB_CMD_START_LED_TEST       "start_led_test"      //开始LED测试
#define USB_CMD_STOP_LED_TEST        "stop_led_test"       //结束LED测试
#define USB_CMD_START_SPEAKER_TEST   "start_speaker_test"  //开始喇叭测试
#define USB_CMD_STOP_SPEAKER_TEST    "stop_speaker_test"   //结束喇叭测试
#define USB_CMD_IMEI                 "IMEI"                //获取IMEI
#define USB_CMD_SIM_INFO             "sim_info"            //获取SIM卡信息
#define USB_CMD_VOLTS                "volts"               //获取电池电压
#define USB_CMD_RSSI                 "rssi"                //获取信号强度
#define USB_CMD_NETSTATUS            "netstatus"           //网络状态
#define USB_CMD_PDPSTATUS            "pdpstatus"           //PDP状态
#define USB_CMD_WIFI_INIT            "wifi_init"           //WIFI初始化指令
#define USB_CMD_WIFI_INIT_STATUS     "wifi_init_status"    //查询WIFI初始化状态
#define USB_CMD_GPRS_ENABLE          "gprs_enable"         //使能GPRS
#define USB_CMD_GPRS_DISABLE         "gprs_disable"        //去使能GPRS
#define USB_CMD_WIFI_ENABLE          "wifi_enable"         //使能WIFI
#define USB_CMD_WIFI_DISABLE         "wifi_disable"        //去使能WIFI
#define USB_CMD_WIFI_VERSION         "wifi_version"        //获取WIFI固件版本
#define USB_CMD_REGISTER_URL         "register_url:"       //写入参数register_url
#define USB_CMD_MQTT_SERVER          "mqtt_server:"        //写入参数mqtt_server
#define USB_CMD_AUTHKEY_N            "authkey_n:"          //写入参数authkey_n
#define USB_CMD_DEVICEID             "deviceid:"           //写入参数deviceid
#define USB_CMD_CLIENTID             "clientid:" 
#define USB_CMD_PRODUCTKEY           "product_key:"
#define USB_CMD_DEVSECRET            "device_secret:"
#define USB_CMD_REGIONID             "region_id:"
#define USB_CMD_KEYSECRET            "key_secret:"

#define USB_CMD_MANU_TO_APP                	"MANU_TO_APP"               //回应用状态
#define USB_CMD_MANU_TO_VENDOR             	"MANU_TO_VENDOR"            //回厂商状态
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


// 设置生命周期
int set_Lifecycle(int life);
// 获取生命周期
int get_lifecycle(void);

#endif
