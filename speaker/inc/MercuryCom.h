/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author:
#########################################################################*/

#ifndef __MERCURY_UART_H__
#define __MERCURY_UART_H__
#include "MercuryDef.h"

typedef enum
{
    COM_ID0 = 0,    //hardware uart1
    COM_ID1,		//hardware uart2		
	COM_ID2,		//hardware uart3
    COM_ID_MAX,
    MERCURY_ENUM_COMID_MAX = 0x7fffffff
}COM_ID_E;

#define    DISABLE_PARITY      0
#define    EVEN_PARITY            1
#define    ODD_PARITY              2

#define     DEFAULT_BITS      0   // default is 8bit
#define     FIVE_BITS             1
#define     SIX_BITS               2
#define     SEVEN_BITS           3
#define     EIGHT_BITS           4

#define     DEFAULT_STOP_BIT   0   // 1BIT

#define     NO_FLOW_CONTROL      0
#define     HW_FLOW_CONTROL     1
#define     SW_FLOW_CONTROL     2

typedef struct
{
    ULONG  baud_rate;
    BYTE   parity;
    BYTE   stop_bits;      //stop bits : 1bit,  can not modify
    BYTE   byte_size;
    BYTE   flow_control;
} COM_CONFIG_T;

#define BAUD_1200               0x2A50
#define BAUD_2400               0x1528
#define BAUD_4800               0x0A94
#define BAUD_9600               0x054A
#define BAUD_19200              0x02A5
#define BAUD_38400              0x0152
#define BAUD_57600              0x00E2
#define BAUD_74800               0x00AE
#define BAUD_115200             0x0071
#define BAUD_230400             0x0038
#define BAUD_256000             0x0032
#define BAUD_460800             0x001C
#define BAUD_500000             0x001A
#define BAUD_921600             0x000E
#define BAUD_1000000           0x000D
#define BAUD_1625000           0x8
#define BAUD_3250000           0x4

#define FASTBOOT_RUNNING    0
#define FASTBOOT_STOP           1

#define MERCURY_MSG_ID_COM_NEW_DATA   0xf000

typedef enum{
	MERCURY_COM_TYPE_UART = 0,
	MERCURY_COM_TYPE_USB,
	MERCURY_COM_TYPE_MAX,
}MERCURY_COM_TYPE_E;

typedef struct{
	uint8 comType;
	uint8 comId;
}MERCURY_COM_MSG_DATA_T;

typedef struct{
	uint32_t rxd;
	uint32_t txd;
	uint32_t cts;
	uint32_t rts;
}MC_COM_IOMUX_CFG_T;

/**
 * 串口初始化
 *
 * @param [ULONG] id: software uart id (0: hardware uart1; 1: hardware uart2; 2: hardware uart3)
 * @param [COM_CONFIG_T *] cfg: 串口配置
 * @return 0: success; others: fail
 */
extern INT COM_Init(ULONG id, COM_CONFIG_T* cfg);
extern INT COM_Deinit(ULONG id);
extern INT COM_Config(ULONG id, COM_CONFIG_T* cfg);
extern INT COM_Read(ULONG id,BYTE* pTargetBuffer,ULONG BufferLength,ULONG* pBytesRead,ULONG timeout);
extern INT COM_Write(ULONG id,BYTE* pSourceBytes,ULONG NumberOfBytes );

extern INT MercuryFastbootStateSet(uint32 state);
extern uint32 MercuryFastbootStateGet(void);
extern uint32 MercuryUsbRead(const uint8 *buffer,uint32 length);
extern uint32 MercuryUsbWrite(const uint8 *buffer,uint32 length);

/**
 * usb虚拟串口读数据
 *
 * @param [uint32] port_id: usb虚拟串口port号
 * @param [uint8 *] pTargetBuffer: 数据读取存放的buffer
 * @param [uint32] toReadBytes: 要读取的数据长度
 * @param [uint32*] pBytesRead: 实际读取的数据长度
 * @param [uint32] timeout: 未读取到指定长度数据的超时时间，单位s
 * @return 0: success; others: fail
 */
extern int VirComRead(uint32 port_id, uint8* pTargetBuffer,uint32 BufferLength,uint32* pBytesRead,uint32 timeout);
/**
 * usb虚拟串口写数据
 *
 * @param [uint32] port_id: usb虚拟串口port号
 * @param [uint8 *] pSourceBytes: 要发送的数据buffer
 * @param [uint32] NumberOfBytes: 要发送的数据长度
 * @param [uint32*] pBytesWrite: 实际发送的数据长度
 * @return 0: success; others: fail
 */
extern int VirComWrite(uint32 port_id, uint8* pSourceBytes,uint32 NumberOfBytes, uint32* pBytesWrite);

/**
 * uart回调设置
 *
 * @param [HANDLE] threadHandle: 数据接收线程，在线程中读取数据
 * @param [uint32] id: software uart id (0: hardware uart1; 1: hardware uart2; 2: hardware uart3)
 * @param [BOOLEAN] enable: 开启/关闭callback 模式
 * @return 0: success; others: fail
 * Note: 开启后，有新数据接收到，线程会接收到MERCURY_MSG_ID_COM_NEW_DATA
 * 请务必在线程中MercuryGetMessage等待消息
 */
extern int Com_SetCb(HANDLE threadHandle, uint32 id, BOOLEAN enable);
/**
 * usb virtual com回调设置
 *
 * @param [HANDLE] threadHandle: 数据接收线程，在线程中读取数据
 * @param [uint32] id: usb port id (0: usb port0; 1: usb port1)
 * @param [BOOLEAN] enable: 开启/关闭callback 模式
 * @return 0: success; others: fail
 * Note: 开启后，有新数据接收到，线程会接收到MERCURY_MSG_ID_COM_NEW_DATA
 * 请务必在线程中MercuryGetMessage等待消息
 */
extern int VirCom_SetCb(HANDLE threadHandle, uint32 id, BOOLEAN enable);

/**
 *  获得串口接收缓存数据的长度
 *
 * @param [uint32] id: com id (0: uart1, 1: uart2, 2: uart3)
 * @return 
 *		>=0: 数据长度；
 *		<0	: fail;
 */
extern int COM_ReadAvail(uint32 id);

/**
 *  获得串口可发送缓存的大小
 *
 * @param [uint32] id: com id (0: uart1, 1: uart2, 2: uart3)
 * @return 
 *		>=0: 可发送的长度；
 *		<0	: fail;
 */	
extern int COM_WriteAvail(uint32 id);

/**
 *  清空接收缓冲区数据
 *
 * @param [uint32] id: com id (0: uart1, 1: uart2, 2: uart3)
 * @return 0: success; others: fail
 */	
int COM_ReadFlush(uint32 id);

/**
 *  清空发送缓冲区数据
 *
 * @param [uint32] id: com id (0: uart1, 1: uart2, 2: uart3)
 * @return 0: success; others: fail
 */	
int COM_WriteFlush(uint32 id);

/**
 *  查询数据发送完成状态
 *
 * @param [uint32] id: com id (0: uart1, 1: uart2, 2: uart3)
 * @return 0: 发送中; 1: 发送完毕; others: fail
 */	
int COM_GetWriteFinish(uint32 id);

/**
 *  uart 功能pin脚复用配置
 *
 * Note: 该函数务必在所有uart初始化前调用才能生效
 * @param [MC_COM_IOMUX_CFG_T **] tab: uart iomux二维数组表
 * @return 0: success; others: fail
 */	
int COM_IomuxSet(MC_COM_IOMUX_CFG_T *tab);

/**
 *  获得usb虚拟串口接收缓存数据的长度
 *
 * @param [uint32] port_id: usb虚拟串口port号
 * @return 
 *		>=0: 数据长度；
 *		<0	: fail;
 */
int VirComReadAvail(uint32 port_id);

/**
 *  获得usb虚拟串口可发送缓存的大小
 *
 * @param [uint32] port_id: usb虚拟串口port号
 * @return 
 *		>=0: 可发送的长度；
 *		<0	: fail;
 */	
int VirComWriteAvail(uint32 port_id);

/**
 *  清空usb虚拟串口接收缓冲区数据
 *
 * @param [uint32] port_id: usb虚拟串口port号
 * @return 0: success; others: fail
 */	
int VirComReadFlush(uint32 port_id);

/**
 *  清空usb虚拟串口发送缓冲区数据
 *
 * @param [uint32] port_id: usb虚拟串口port号
 * @return 0: success; others: fail
 */	
int VirComWriteFlush(uint32 port_id);

/**
 *  查询usb虚拟串口数据发送完成状态
 *
 * @param [uint32] port_id: usb虚拟串口port号
 * @return 0: 发送中; 1: 发送完毕; others: fail
 */	
int VirComGetWriteFinish(uint32 port_id);

#endif  //__MERCURY_UART_H__
