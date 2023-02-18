#ifndef _SDK_API_ERROR_H
#define _SDK_API_ERROR_H

//系统初始化错误,以下是 sdk_System_Init() 函数可能返回的错误值（网络相关错误请先检查ESIM和天线是否正常）
#define SYS_INIT_TYPE          0x10000000
#define SECOMM_INIT_ERROR      SYS_INIT_TYPE+0x00000001       //SE通信串口初始化失败
#define TTS_INIT_ERROR         SYS_INIT_TYPE+0x00000002       //TTS初始化失败
#define LABEL_INIT_ERROR       SYS_INIT_TYPE+0x00000003       //label初始化失败
#define NET_STATE_ERROR		   SYS_INIT_TYPE+0x00000004       //获取网络状态失败
#define GPRS_SEARCH_ERROR      SYS_INIT_TYPE+0x00000005       //GSM&GPRS网络获取失败
#define SIGNAL_QUALITY_ERROR   SYS_INIT_TYPE+0x00000006       //信号质量过弱
#define PDP_OPEN_ERROR         SYS_INIT_TYPE+0x00000007       //PDP打开失败 
#define DNS_EVENT_CREAT_ERROR  SYS_INIT_TYPE+0x00000008       //DNS消息事件创建失败
#define CON_EVENT_CREAT_ERROR  SYS_INIT_TYPE+0x00000009       //TCP连接事件创建失败
#define KP_INIT_ERROR          SYS_INIT_TYPE+0x0000000a       //KP初始化失败

//发送APDU错误,以下是 sdk_Transmit_APDU() 函数可能返回的错误值
#define TRANS_APDU_TYPE        0x20000000
#define APDU_RECV_TIMEOUT	   TRANS_APDU_TYPE+0x00000001     //接收超时
#define APDU_HEAD_ERROR        TRANS_APDU_TYPE+0x00000002     //RAPDU头部错误
#define APDU_SN_ERROR          TRANS_APDU_TYPE+0x00000003     //RAPDU帧序列号错误
#define APDU_STATUS_ERROR      TRANS_APDU_TYPE+0x00000004     //RAPDU帧STATUS字段错误
#define APDU_RLENGTH_BEYOND    TRANS_APDU_TYPE+0x00000005     //RAPDU数据长度超出缓冲区大小
#define APDU_CHECKSUM_ERROR    TRANS_APDU_TYPE+0x00000006     //RAPDU校验和错误

//音频播放错误,以下是 // sdk_voiceplay() 函数可能返回的错误值
#define VOICE_PLAY_TYPE        0x30000000
#define TTS_PLAY_ERROR         VOICE_PLAY_TYPE+0x00000001     //TTS播放错误

//TCP连接错误,以下是 sdk_tcp_connect() 函数可能返回的错误值
#define TCP_CON_TYPE           0x40000000
#define CON_EVENT_WAIT_ERROR   TCP_CON_TYPE+0x00000001        //TCP连接事件等待失败
#define CONNECT_FAILED         TCP_CON_TYPE+0x00000002        //TCP连接失败(检查地址/端口是否正确,服务器是否打开)
#define SET_KEEPALIVE_ERROR    TCP_CON_TYPE+0x00000003        //设置keepalive失败
#define RECV_EVENT_CREAT_ERROR TCP_CON_TYPE+0x00000004

//TCP接收错误,以下是 sdk_tcp_recv() 函数可能返回的错误值
#define TCP_RECV_TYPE          0x50000000
#define RECV_EVENT_OPEN_ERROR  TCP_RECV_TYPE+0x00000001       //TCP接收事件打开失败(检查系统初始化,TCP连接是否成功)
#define RECV_TIMEOUT           TCP_RECV_TYPE+0x00000002       //接收超时(在指定时间内未接收到足够数据)
#define RECV_SOCKET_DISCONNECT TCP_RECV_TYPE+0x00000003       //SOCKET断开连接(请检查连接状态)
#define RECV_SOCKET_CLOSE      TCP_RECV_TYPE+0x00000004       
#define SOCKETRECV_CALL_ERROR  TCP_RECV_TYPE+0x00000005       //SocketRecv函数调用失败
#define RECV_SUCCESS		(0)

//TCP发送错误,以下是 sdk_tcp_send() 函数可能返回的错误值
#define TCP_SEND_TYPE          0x60000000
#define SEND_SOCKET_DISCONNECT TCP_SEND_TYPE+0x00000001       //SOCKET断开连接(检查系统初始化,TCP连接是否成功)
#define SOCKETSEND_CALL_ERROR  TCP_SEND_TYPE+0x00000002       //SocketSend函数调用失败
#define SEND_TIMEOUT           TCP_SEND_TYPE+0x00000003       //发送超时
//TCP关闭错误,以下是 sdk_tcp_close() 函数可能返回的错误值
#define TCP_CLOSE_TYPE         0x70000000
#define TCP_UNLINK             TCP_CLOSE_TYPE+0x00000001

//DNS解析错误,以下是 sdk_hosttoip() 函数可能返回的错误值
#define DNS_PARSE_TYPE         0x80000000
#define HOSTBYNAME_CALL_ERROR  DNS_PARSE_TYPE+0x00000001
#define DNS_TIMEOUT_ERROR      DNS_PARSE_TYPE+0x00000002
#define DNS_PARSE_FAILED       DNS_PARSE_TYPE+0x00000003      //DNS解析失败(检查域名是否正确)
#define WIFI_DNS_READ_FAILED   DNS_PARSE_TYPE+0x00000004
#define WIFI_DNS_IP_FAILED     DNS_PARSE_TYPE+0x00000005
#define WIFI_DNS_PARSE_FAILED  DNS_PARSE_TYPE+0x00000006

//MQTT连接错误,以下是 sdk_MQTT_connect() 函数可能返回的错误值
#define MQTT_CON_TYPE          0x90000000
#define MQTT_TCP_CON_ERR       MQTT_CON_TYPE+0x00000001   //连接MQTT服务器时TCP连接失败
#define MQTT_HANDSHAKE_ERR     MQTT_CON_TYPE+0x00000002   //连接MQTT服务器时与MQTT服务器握手失败
#define INPUT_PARAMETER_ERROR  MQTT_CON_TYPE+0x00000003   //调用函数时传参错误
#define TLS_INIT_ERROR         MQTT_CON_TYPE+0x00000004   //TLS初始化失败
#define TLS_CON_NUM_OVER       MQTT_CON_TYPE+0x00000005   //TLS连接数目超出最大限制
#define MQTT_SOCKET_ERROR      MQTT_CON_TYPE+0x00000006   //SOCKET创建失败
#define MQTT_RETURN_CODE_01    MQTT_CON_TYPE+0x00000007   //return code 01
#define MQTT_RETURN_CODE_02    MQTT_CON_TYPE+0x00000008   //return code 02
#define MQTT_RETURN_CODE_03    MQTT_CON_TYPE+0x00000009   //return code 03
#define MQTT_RETURN_CODE_04    MQTT_CON_TYPE+0x0000000a   //return code 04
#define MQTT_RETURN_CODE_05    MQTT_CON_TYPE+0x0000000b   //return code 05

//MQTT订阅错误,以下是 sdk_MQTT_subscribe() 函数可能返回的错误值
#define MQTT_SUB_TYPE          0x11000000
#define MQTT_SUB_ERR           MQTT_SUB_TYPE+0x00000001   //订阅失败

//MQTT退订错误,以下是 sdk_MQTT_unsubcribe() 函数可能返回的错误值
#define MQTT_UNSUB_TYPE        0x12000000
#define MQTT_UNSUB_ERR         MQTT_UNSUB_TYPE+0x00000001  //取消订阅失败

//MQTT关闭错误,以下是 sdk_MQTT_close() 函数可能返回的错误值
#define MQTT_CLOSE_TYPE            0x13000000
#define MQTT_CON_NO_ERR            MQTT_CLOSE_TYPE+0x00000001  //关闭MQTT时，连接并未创建或在程序中已经主动断开
#define MQTT_CLOSE_FUNC_ERR        MQTT_CLOSE_TYPE+0x00000002  //执行MQTT连接关闭失败
#define MQTT_SSL_CLOSE_NOTIFY_ERR  MQTT_CLOSE_TYPE+0x00000003  //执行发送MQTT_TLS提示关闭连接时失败
#define MQTT_TLS_CLOSE_PARA_ERR    MQTT_CLOSE_TYPE+0x00000004  //关闭MQTT_TLS参数失败
//sdk_MQTT_yield() 函数可能返回的错误值
#define MQTT_YIELD_TYPE        0x14000000
#define MQTT_YIELD_NO_CON      MQTT_YIELD_TYPE+0x00000001  //执行MQTT空闲任务时连接不存在(连接并未创建或在程序中已主动断开)
#define MQTT_YIELD_ERR         MQTT_YIELD_TYPE+0x00000002  //执行MQTT空闲任务失败

//FOTA下载错误,以下是 sdk_fota_download_http() 函数可能返回的错误值
#define FOTA_DOWNLOAD_TYPE     0x15000000
#define DOWNLOAD_ALL_FAILED    FOTA_DOWNLOAD_TYPE+0x00000001  //第一次下载失败
#define HTTP_STATUS_CODE_ERROR FOTA_DOWNLOAD_TYPE+0x00000002  //http返回状态码错误
#define DOWNLOAD_ALL_BEYOND    FOTA_DOWNLOAD_TYPE+0x00000003  //第一次下载接收到数据越界
#define RESUME_FAILED          FOTA_DOWNLOAD_TYPE+0x00000004  //断点续传下载失败
#define RESUME_BEYOND          FOTA_DOWNLOAD_TYPE+0x00000005  //断点续传接收到数据越界
#define RESUME_MAX             FOTA_DOWNLOAD_TYPE+0x00000006  //断点续传次数达最大

//FOTA更新错误,以下是 sdk_fota_update() 函数可能返回的错误值
#define FOTA_UPDATE_TYPE       0x16000000
#define IMGINFOSET_FAILED      FOTA_UPDATE_TYPE+0x00000001    //FOTA_ImgInfoSet()函数调用失败
#define IMG_READ_ERROR         FOTA_UPDATE_TYPE+0x00000002    //读FLASH错误
#define IMG_LENGTH_ERROR       FOTA_UPDATE_TYPE+0x00000003    //IMG长度错误
#define ALGORITHMS_TYPE_ERROR  FOTA_UPDATE_TYPE+0x00000004    //算法错误
#define IMG_CHECK_ERROR        FOTA_UPDATE_TYPE+0x00000005    //校验错误

//以下是 Label_read_int() 函数可能返回的错误值
#define LABEL_READ_INT_TYPE    0x17000000
#define READ_INT_NO_FIND_VALID	LABEL_READ_INT_TYPE+0x00000001     //未发现有效页
#define READ_INT_UNKNOWN_TYPE	LABEL_READ_INT_TYPE+0x00000002     //未知LABEL类型
#define READ_INT_ADDR_ERROR		LABEL_READ_INT_TYPE+0x00000003
#define READ_INT_UNKNOWN		LABEL_READ_INT_TYPE+0x00000004     //未找到对应LABEL
#define READ_INT_FLASH          LABEL_READ_INT_TYPE+0x00000005

//以下是 Label_write_int() 函数可能返回的错误值
#define LABEL_WRITE_INT_TYPE   0x18000000
#define WRITE_INT_NO_FIND_VALID	LABEL_WRITE_INT_TYPE+0x00000001
#define WRITE_INT_UNKNOWN_TYPE	LABEL_WRITE_INT_TYPE+0x00000002
#define WRITE_INT_FIND_ADDR_ERR	LABEL_WRITE_INT_TYPE+0x00000003
#define WRITE_INT_UNKNOWN  	    LABEL_WRITE_INT_TYPE+0x00000004 
#define WRITE_INT_FLASH_ERROR	LABEL_WRITE_INT_TYPE+0x00000005
#define WRITE_INT_CHANGE_ERROR	LABEL_WRITE_INT_TYPE+0x00000006

//以下是 Label_read_data() 函数可能返回的错误值
#define LABEL_READ_DATA_TYPE   0x19000000
#define READ_DATA_NO_FIND_VALID	LABEL_READ_DATA_TYPE+0x00000001
#define READ_DATA_UNKNOWN_TYPE	LABEL_READ_DATA_TYPE+0x00000002
#define READ_DATA_UNKNOWN_LABEL	LABEL_READ_DATA_TYPE+0x00000003    //未找到对应LABEL
#define READ_DATA_ADDR_ERROR	LABEL_READ_DATA_TYPE+0x00000004
#define READ_DATA_FLASH         LABEL_READ_DATA_TYPE+0x00000005
#define READ_DATA_LEN_ERROR		LABEL_READ_DATA_TYPE+0x00000006

//以下是 Label_write_data() 函数可能返回的错误值
#define LABEL_WRITE_DATA_TYPE  0x21000000
#define WRITE_DATA_NO_FIND_VALID	LABEL_WRITE_DATA_TYPE+0x00000001
#define WRITE_DATA_UNKNOWN_TYPE		LABEL_WRITE_DATA_TYPE+0x00000002
#define WRITE_DATA_UNKNOWN_LABEL	LABEL_WRITE_DATA_TYPE+0x00000003
#define WRITE_DATA_NO_MACH_LABEL	LABEL_WRITE_DATA_TYPE+0x00000004   //未找到对应LABEL
#define WRITE_DATA_FLASH            LABEL_WRITE_DATA_TYPE+0x00000005
#define WRITE_DATA_CHANGE_ERROR		LABEL_WRITE_DATA_TYPE+0x00000006

//以下是 Label_delete() 函数可能返回的错误值
#define LABEL_DEL_TYPE         0x22000000
#define LABEL_NO_VALID		LABEL_DEL_TYPE+0x00000001
#define LABEL_TYPE_ERROR	LABEL_DEL_TYPE+0x00000002
#define LABEL_FLASH_ERROR	LABEL_DEL_TYPE+0x00000005

//以下是 使用TLS可能返回的错误值
#define TLS_ERR_TYPE           0x23000000
#define TLS_HANDSHAKE_FAILED   TLS_ERR_TYPE+0x00000001
#define TCP_TLS_CON_ERR        TLS_ERR_TYPE+0x00000002
#define TLS_CERT_VERIFY_ERR        TLS_ERR_TYPE+0x00000003
#define TLS_CERT_NOT_FOUND        TLS_ERR_TYPE+0x00000004

//以下是change_mqtt_alive_interval函数可能返回的错误值
#define CHANGE_MQTT_ALIVE           0x24000000
#define INPUT_HANDLE_INVALID        CHANGE_MQTT_ALIVE+0x00000001
#define INPUT_ALIVEINTERVAL_INVALID CHANGE_MQTT_ALIVE+0x00000002

#endif
