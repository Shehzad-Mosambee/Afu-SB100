#ifndef _MERCURY_PBK_H_
#define _MERCURY_PBK_H_

#include "MercuryDef.h"

#define SIM_PBK_NAME_SIZE 50 
#define SIM_PBK_NUMBER_SIZE 10

// PBK Configuration
#define MC_PBK_SIM 0x01
#define MC_PBK_ME 0x02

#define MC_PBK_ME_MISSED 0x1
#define MC_PBK_ME_RECEIVED 0x2
#define MC_PBK_SIM_FIX_DIALLING 0x4
#define MC_PBK_SIM_LAST_DIALLING 0x8
#define MC_PBK_ME_LAST_DIALLING 0x10
#define MC_PBK_SIM_BARRED_DIALLING 0x11
#define MC_PBK_SIM_SERVICE_DIALLING 0x12
#define MC_PBK_ON 0x20
#define MC_PBK_EN 0x40
#define MC_PBK_SDN 0x80

#define MC_PBK_AUTO 0x00

#define MC_PHONE_NUM_STRING_LEN  (SIM_PBK_NUMBER_SIZE * 2 + 4)
#define MC_PBK_NAME_SIZE         SIM_PBK_NAME_SIZE

typedef struct _MC_SIM_PBK_ENTRY_INFO
{
    uint8 pNumber[MC_PHONE_NUM_STRING_LEN];
    uint8 pFullName[MC_PBK_NAME_SIZE];
    uint8 iFullNameSize;
    uint8 nNumberSize;
    uint8 nType;
    uint16 phoneIndex;
    uint8 *pRecordData;
} MC_SIM_PBK_ENTRY_INFO;

/**
* @brief 读取电话本中指定位置的记录
*
* @param[in] storage 电话本存储介质
* @param[in] startindex 要读取记录的开始索引
* @param[in] endindex 要读取记录的结束索引,如果只读一条记录,则等于startindex
* @return 0 SUCCESS; -1 FAIL
   只要参数正确该函数返回TRUE， 是否读取成功 MC_RegNotifyCallback注册的callback 函数中的 
   NOTIFY_CLASS_PBK 的事件处理
*/
int PhoneBookRead(uint8 storage, uint8 starindex, uint8 endindex);

/**
* @brief 删除指定位置的电话本记录
*
* @param[in] storage 电话本存储介质
* @param[in] index 要删除记录的索引
* @return 0 SUCCESS; -1 FAIL
   只要参数正确该函数返回TRUE， 得到的电话记录信息在 MC_RegNotifyCallback注册的callback 函数中的
   NOTIFY_CLASS_PBK的事件处理， ID NOTIFY_ID_PBK_DELETE_OK表示成功
*/

int PhonebookDelete(uint8 storage,uint32 index);

/**
* @brief 把电话记录写到指定位置
*
* @param[in] storage 电话本存储介质
* @param[in] entry_ptr 电话记录内容
* @param[in] index 要写入记录的索引
* @return TRUE SUCCESS; FALSE FAIL
 只要参数正确该函数返回TRUE， 是否添加成功 MC_RegNotifyCallback注册的callback 函数中的
 NOTIFY_CLASS_PBK  的事件处理，NOTIFY_ID_PBK_WRITE_OK 则写入成功
*/
BOOL PhonebookWrite(uint8 storage, MC_SIM_PBK_ENTRY_INFO* entry_ptr,uint32 index);


#endif

