/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author: 
#########################################################################*/

#ifndef _MERCURY_NLED_H_
#define _MERCURY_NLED_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum NLED_ID_E_TAG
{
    LED_1,
    LED_2,
    LED_INDICATION,           //�ɵ���LED
    LED_LCD_BL,               //LCD�����
    LED_FLASH_BL,             //FLASH_BL
    LED_MAX_NUM,
    MERCURY_ENUM_NLEDID_MAX = 0x7fffffff
}NLED_ID_E;


typedef enum NLED_MODE_E_TAG
{
    NLED_MODE_OFF,
    NLED_MODE_ON,
    NLED_MODE_BLINK,
    NLED_MODE_MAX,
    MERCURY_ENUM_NLEDMODE_MAX = 0x7fffffff
}NLED_MODE_E;


INT NLedInit(NLED_ID_E LedID);

INT NLedDeinit(NLED_ID_E LedID);

INT NLedSetMode(
NLED_ID_E LedID,
INT OffOnBlink, 
LONG TotalCycleTime, 
LONG OnTime, 
LONG OffTime,
INT MetaCycleOn, 
INT MetaCycleOff
);


/*************************************************************************************
 *��������:  NledConfigLedGpioId 
 *����˵��:  ���� NLED_ID_E �ж�Ӧ��GPIO  ֻ֧��LED_1,LED_2
 *����˵��:
        LedID  [in]  NLED_ID_E
        Gpio_Id [in]  GPIO
 *����ֵ��TURE/FALSE
*************************************************************************************/
BOOLEAN NledConfigLedGpioId(UINT LedID, UINT Gpio_Id);

/*************************************************************************************
 *��������:  MC_SetLedIndicationlevel 
 *����˵��:  ���õ���Դ�����ȣ����13
 *����˵��:
*************************************************************************************/
void SetLedIndicationlevel(uint32 level);

/*************************************************************************************
 *��������:  MC_SetLedIndication 
 *����˵��:  ���Ƶ���Դ������
 *����˵��:  
*************************************************************************************/
void SetLedIndication(BOOLEAN type);

/*************************************************************************************
 *��������:  NLedSetCurrentLedlevel
 *����˵��:  ���õ���Դ������, ��֧��LED_1,LED_2
 *����˵��:
        type  [in]  ����ԴID 
        level [in]  �����ȼ�: 
                            �� type ΪLED_LCD_BL ʱ 0-14��������Χ5.04-163.8mA
                            �� type ΪLED_INDICATION ʱ 0-14��������Χ1.68-54.6mA
                            �� type ΪLED_FLASH_BLʱ  0-15��������Χ��15-240mA��step��15mA
*************************************************************************************/
void NLedSetCurrentLedlevel(NLED_ID_E type, uint32 level);

/*************************************************************************************
 *��������:  NledSetCurrentLed
 *����˵��:  ���Ƶ���Դ������, ��֧��LED_1,LED_2
 *����˵��:
*************************************************************************************/
void NledSetCurrentLed(NLED_ID_E type, BOOLEAN enble);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_MERCURY_NLED_H_

