#ifndef _NV_H_
#define _NV_H_

#define AUDIO_TEMPLATE_MAX 20

typedef enum
{
	NV_RESET_REASON = 0, // Record the cause of device restart
	
	NV_NETWORK_MODE =1,  //4g wifi
	NV_WIFI_INFO = 2,   //记录wifi信息
	NV_VOLUME_LEVEL =3, //音量
	NV_AUDIO_TYPE = 4,  //amr mp3 wav pcm tts 

	NV_APP_INFO_ITEM = 9,	// app info

	NV_SYS_LIFECYCLE = 10, //生命周期， 0为厂商阶段， 1为应用阶段
	NV_VENDOR_TEST_RESULTS = 11, // 工厂模式测试结果

	NV_SYS_INFO = 12,    //应用系统信息
	NV_TEST_ITEM = 13,   //用于记录测试项
	NV_WIFI_ENABLE = 14, //用于记录WIFI是否使能
	NV_GPRS_ENABLE = 15, //用于记录GPRS是否使能
	NV_SN,
	NV_PRODUCTKEY,
	NV_SERVERHOST,
	NV_SERVERPORT,
	NV_AUDIO_VERSION,
	NV_PLAY_SPEED,
	NV_HARDWARE_CONFIG,		//记录设备配置信息
#define NV_WIFI_CONFIG_MODE_AK		(1)
#define NV_WIFI_CONFIG_MODE_AP		(2)
	NV_WIFI_CONFIG_MODE = 30,	// int, 0:No Action, 1:AK, 2:AP
	NV_SYS_REBOOT_REASION = 31,	// 1:net cfg, 2:net switch
	NV_VENDOR_FLAG = 32,

	NV_MQTT_CFG = 40,        //记录MQTT配置信息
	NV_AUDIO_CFG = 41,       //记录AUDIO配置信息
	NV_POWERON_CFG = 42,     //记录开机配置
	NV_POWEROFF_CFG = 43,    //记录关机配置
	NV_AUDIO_CUSTOM_CFG = 44, //记录商家自定义配置
	NV_AD_CFG = 45,          //记录广告配置
	NV_POWERON_DATA = 46,    //记录开机语音数据
	NV_POWEROFF_DATA = 47,   //记录关机语音数据
	NV_AUDIO_CUSTOM_DATA = 48,   //记录客户语音数据
	NV_QRCODE_CFG = 49,   //记录动态二维码刷新数据

	NV_AUDIO_TEMPLATE_START = 50, //交易播报语音模板起始index
	NV_DISCOUNT_TEMPLATE_START = NV_AUDIO_TEMPLATE_START + AUDIO_TEMPLATE_MAX, //折扣语音模板起始index
	NV_VERIFYCODE_TEMPLATE_START = NV_DISCOUNT_TEMPLATE_START+ AUDIO_TEMPLATE_MAX, //验证码语音模板起始index

	NV_AUDIO_LEN_ARRAY = 120,
    NV_AUDIO_START_INDEX = 121,
	
	NV_DEVICE_DEBUG_INFO = 485,
	NV_TRANS_RECORD = 486, // trans record 

    QYY_LICENSE_NV = 499,

}NVIndex;

#define AUDIO_TTS_LEN_MAX 100
#define QYY_LICENSE_LEN_MAX 1024

typedef struct 
{
	int len;
	char type;
	char data[100];
}NvAudioTemplateStruct;


typedef struct 
{
	//int len;   //语音数据长度
	int mode;  //0：不播放； 1：播放默认语音；2：播放自定义语音，语音数据及格式由本模板确定
	int format; //语音数据格式，整型数值，TTS（1）、AMR（2）、PCM（3）、MP3（4）
	//char data[1]; //语音数据
}NvPowerCfgStruct;

//广告配置
typedef struct 
{
	int len; //音频数据的长度
	int mode; //播放模式   0：不播放广告；1：每次开机时播放广告；2：定时播放广告；3：循环播放广告，每收款到账n笔后播放。
	int period_start; //广告播放模式为2和3时，广告播放的起止时间段。开始时间；
	int period_end; //结束时间 start和end为0到24的整数，代表时间整点数，并要求 end > start
	int interval; //广告播放模式为2时有效：	定时播放间隔时间，单位为秒。
	int pay_count; //广告播放模式为3时有效：	收款到账次数
	int format;   //语音数据格式，整型数值，TTS（1）、AMR（2）、PCM（3）、MP3（4）。
	char data[1];
}NvAdCfgStruct;

//动态二维码配置信息
typedef struct 
{
	char url[256];
	int  gap;
}NvQrcodeCfgStruct;
#endif
