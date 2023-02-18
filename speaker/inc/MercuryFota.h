/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author:
#########################################################################*/

#ifndef __MERCURY_FOTA_H__
#define __MERCURY_FOTA_H__

#include "MercuryDef.h"

typedef enum
{
    STORE_INSIDE_FLASH = 0,
    STORE_FILE_SYSTEM,
    STORE_INSIDE_FLASH_EXT,    //仅在release 且不支持audio版本可使用
    IMG_STORE_MAX,
    MERCURY_ENUM_STORE_MAX = 0x7fffffff
}IMG_STORE_E;

typedef enum
{
    IMG_UPDATA_NOW = 0,
    IMG_UPDATA_NEXT_BOOT,
    IMG_UPDATA_NEVER,
    IMG_UPDATA_MAX,
    MERCURY_ENUM_UPDATE_MAX = 0x7fffffff
}IMG_UPDATE_E;

extern void FOTA_Init(void);
extern int FOTA_ImgInfoSet(IMG_STORE_E storeType, const uint8* name, IMG_UPDATE_E updataType);
extern uint32 FOTA_WroteLenGet(void);
extern int FOTA_FlashWrite(uint32 addr, const uint8 * buf,uint32 write_len);
extern int FOTA_FlashRead(uint32 addr, uint8* buf, uint32 read_len);

/*************************************************************************************
主应用刷次应用时需要flash操作
addr 范围0~1024kbyte
addr+readlen 不能大于1024kbyte

执行erase时，addr必须是32kbyte的整数倍，按32kbyte擦除
*************************************************************************************/
extern BOOLEAN APP_FlashWrite(uint32 addr, const uint8 * buf,uint32 read_len);
extern BOOLEAN APP_FlashErase(uint32 addr);
extern BOOLEAN APP_FlashRead(uint32 addr, uint8* buf, uint32 read_len);


extern int FOTA_RawDataInfoSet(uint32 cs_id, uint32 imgAddr, uint32 imgLen, uint32 enable);
extern int FOTA_RawDataClear(void);

extern BOOLEAN LOGO_FlashWrite(uint32 addr, const uint8 * buf,uint32 read_len);
extern BOOLEAN LOGO_FlashErase(uint32 addr);
extern BOOLEAN LOGO_FlashRead(uint32 addr, uint8* buf, uint32 read_len);

/*************************************************************************************
以下仅在release版本且不支持音频的版本中可用

fota img存储类型新增 IMG_STORE_E -- STORE_INSIDE_FLASH_EXT
使用方式，应用使用下面三个api将fota img写入，然后配置FOTA_ImgInfoSet(STORE_INSIDE_FLASH_EXT,NULL,IMG_UPDATA_NEXT_BOOT)即可

该区域大小32*1024*31kbyte = 992kbyte
客户次应用等亦可使用此区间，执行规划
*************************************************************************************/
extern BOOLEAN APP_FlashWriteExt(uint32 addr, const uint8 * buf,uint32 read_len);
extern BOOLEAN APP_FlashEraseExt(uint32 addr);
extern BOOLEAN APP_FlashReadExt(uint32 addr, uint8* buf, uint32 read_len);


/*************************************************************************************
 * 函数名：MC_FotaUpgrade_Init
 * 函数功能：差分包升级初始化
 * 返回  TURE/falae  如果返回失败  就不要执行升级的剩余操作 
 *************************************************************************************/
extern BOOLEAN FotaUpgrade_Init(void);

/*************************************************************************************
 * 函数名：MC_FotaUpgrade_Deinit
 * 函数功能：差分包升级反初始化,该函数再升级成功后调用
 *************************************************************************************/
extern void FotaUpgrade_Deinit(void);

/*************************************************************************************
 * 函数名：MC_FotaUpgrade_Write
 * 函数功能：差分包升级写数据
 * [in ]：  buf
 *          uint32      data_len;

 * 返回    写入数据长度
 *************************************************************************************/
extern int FotaUpgrade_Write(uint32 offset, uint8* buf, uint32 data_len);

/*************************************************************************************
 * 函数名：MC_FotaUpgrade_Read
 * 函数功能：差分包升级读数据
 * [in ]：  buf
 *          uint32      data_len;

 * 返回    实际读取到的数据长度
 *************************************************************************************/
extern int FotaUpgrade_Read(uint32 offset, uint8* buf, uint32 data_len);

/*************************************************************************************
 * 函数名：MC_FotaUpgradeLen
 * 函数功能：获取当前写入总的数据长度

 * 返回    当前写入总的数据长度
 *************************************************************************************/
extern int FotaUpgradeLen(void);

/*************************************************************************************
 * 函数名：MC_FotaUpgrade_Start
 * 函数功能：开始升级、该函数会去校验数据，数据检验成功才能进行升级，需要可以手动重启或自动重启才会进行升级操作
 * 返回  true 检验成功，可以进行升级，  false 检验失败 
 *************************************************************************************/
extern BOOLEAN FotaUpgrade_Start(void);

/*************************************************************************************
 * 函数名：MC_FotaUpgrade_GetSpace
 * 函数功能：获取当前空间是否足够
 * 返回   空间大小
 *************************************************************************************/
extern uint32 FotaUpgrade_GetSpace(void);


BOOLEAN QspiFlashRead(uint32 addr, uint8* buf, uint32 read_len);
BOOLEAN QspiFlashWrite(uint32 addr, const uint8 * buf, uint32 read_len);
BOOLEAN QspiFlashErase(uint32 addr, uint32 size);

/*************************************************************************************
 * 函数名：QspiFlashInit
 * 函数功能：初始化flash
 * flash ： 1 lcd这路复用flash功能   0：gpio 0 1 2 3 
 * vol： 电压值  当 flash 为0时默认1.8V 可以不设置， flash为1时，电压值根据实际flash设置，3.3v的flash vol=3000 1.8V的flash vol=1800
 *************************************************************************************/
void QspiFlashInit(int flash, int vol);

#endif //__MERCURY_FOTA_H__
