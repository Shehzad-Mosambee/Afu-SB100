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
    STORE_INSIDE_FLASH_EXT,    //����release �Ҳ�֧��audio�汾��ʹ��
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
��Ӧ��ˢ��Ӧ��ʱ��Ҫflash����
addr ��Χ0~1024kbyte
addr+readlen ���ܴ���1024kbyte

ִ��eraseʱ��addr������32kbyte������������32kbyte����
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
���½���release�汾�Ҳ�֧����Ƶ�İ汾�п���

fota img�洢�������� IMG_STORE_E -- STORE_INSIDE_FLASH_EXT
ʹ�÷�ʽ��Ӧ��ʹ����������api��fota imgд�룬Ȼ������FOTA_ImgInfoSet(STORE_INSIDE_FLASH_EXT,NULL,IMG_UPDATA_NEXT_BOOT)����

�������С32*1024*31kbyte = 992kbyte
�ͻ���Ӧ�õ����ʹ�ô����䣬ִ�й滮
*************************************************************************************/
extern BOOLEAN APP_FlashWriteExt(uint32 addr, const uint8 * buf,uint32 read_len);
extern BOOLEAN APP_FlashEraseExt(uint32 addr);
extern BOOLEAN APP_FlashReadExt(uint32 addr, uint8* buf, uint32 read_len);


/*************************************************************************************
 * ��������MC_FotaUpgrade_Init
 * �������ܣ���ְ�������ʼ��
 * ����  TURE/falae  �������ʧ��  �Ͳ�Ҫִ��������ʣ����� 
 *************************************************************************************/
extern BOOLEAN FotaUpgrade_Init(void);

/*************************************************************************************
 * ��������MC_FotaUpgrade_Deinit
 * �������ܣ���ְ���������ʼ��,�ú����������ɹ������
 *************************************************************************************/
extern void FotaUpgrade_Deinit(void);

/*************************************************************************************
 * ��������MC_FotaUpgrade_Write
 * �������ܣ���ְ�����д����
 * [in ]��  buf
 *          uint32      data_len;

 * ����    д�����ݳ���
 *************************************************************************************/
extern int FotaUpgrade_Write(uint32 offset, uint8* buf, uint32 data_len);

/*************************************************************************************
 * ��������MC_FotaUpgrade_Read
 * �������ܣ���ְ�����������
 * [in ]��  buf
 *          uint32      data_len;

 * ����    ʵ�ʶ�ȡ�������ݳ���
 *************************************************************************************/
extern int FotaUpgrade_Read(uint32 offset, uint8* buf, uint32 data_len);

/*************************************************************************************
 * ��������MC_FotaUpgradeLen
 * �������ܣ���ȡ��ǰд���ܵ����ݳ���

 * ����    ��ǰд���ܵ����ݳ���
 *************************************************************************************/
extern int FotaUpgradeLen(void);

/*************************************************************************************
 * ��������MC_FotaUpgrade_Start
 * �������ܣ���ʼ�������ú�����ȥУ�����ݣ����ݼ���ɹ����ܽ�����������Ҫ�����ֶ��������Զ������Ż������������
 * ����  true ����ɹ������Խ���������  false ����ʧ�� 
 *************************************************************************************/
extern BOOLEAN FotaUpgrade_Start(void);

/*************************************************************************************
 * ��������MC_FotaUpgrade_GetSpace
 * �������ܣ���ȡ��ǰ�ռ��Ƿ��㹻
 * ����   �ռ��С
 *************************************************************************************/
extern uint32 FotaUpgrade_GetSpace(void);


BOOLEAN QspiFlashRead(uint32 addr, uint8* buf, uint32 read_len);
BOOLEAN QspiFlashWrite(uint32 addr, const uint8 * buf, uint32 read_len);
BOOLEAN QspiFlashErase(uint32 addr, uint32 size);

/*************************************************************************************
 * ��������QspiFlashInit
 * �������ܣ���ʼ��flash
 * flash �� 1 lcd��·����flash����   0��gpio 0 1 2 3 
 * vol�� ��ѹֵ  �� flash Ϊ0ʱĬ��1.8V ���Բ����ã� flashΪ1ʱ����ѹֵ����ʵ��flash���ã�3.3v��flash vol=3000 1.8V��flash vol=1800
 *************************************************************************************/
void QspiFlashInit(int flash, int vol);

#endif //__MERCURY_FOTA_H__
