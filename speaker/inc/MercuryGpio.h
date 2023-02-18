/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author: 
#########################################################################*/

#ifndef __MERCURY_GPIO_H__
#define __MERCURY_GPIO_H__
#include "MercuryDef.h"

typedef void (*InterruptCallback)(void);
typedef void (*InterruptCallbackWithId)(DWORD *id);

typedef enum
{
    OUTPUT,
    INPUT,
    INVALID_DIR,
    MERCURY_ENUM_DIR_MAX = 0x7fffffff
} MERCURY_DIR_E;

typedef enum
{
    LEVEL,		/*��ƽ�仯ʱ����:����ƽΪ��ʱ���ڲ�����Ϊ�͵�ƽ��������֮��Ȼ*/
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGE,	
    NO_INT,    	 /*�жϹر�*/
	HIGH_LEVEL,  /*�ߵ�ƽ����:����ƽά��Ϊ������ƽ����һֱ�����жϣ���Ҫ���ж�*/  
	LOW_LEVEL,	 /*�͵�ƽ����:����ƽά��Ϊ������ƽ����һֱ�����жϣ���Ҫ���ж�*/	  
    INVALID_INT,
    MERCURY_ENUM_INTERTUPT_MAX = 0x7fffffff
} MERCURY_INTERTUPT_MODE_E;

/*
��չ������Ա����;:
1.int8  int_mode;   
	��������ָ��gpio�жϴ���ģʽ
	0:�ڵװ��ж��߳�ִ��
	1:ֱ�����жϷ��������ִ��, ��fast isrģʽ
	Note: ���GIO_SetFastIsrMode(1)��int_mode������񣬶�Ϊfast isrģʽ
		  ���GIO_SetFastIsrMode(0)��ָ��gpio���жϴ���ģʽȡ����int_modeֵ?
2.InterruptCallbackWithId  fun_v2;		  
	����ָ�붨���һ����ʾgpio id��ָ�����
	Note: ����ṹ���Աfun�и�ֵ���жϲ���ʱִ��fun������ִ��fun_v2
		ֻ��fun=NULL��fun_v2�и�����ָ�������±�ִ�С�
*/
typedef struct
{
    uint16   default_val;
    MERCURY_DIR_E dir;
    MERCURY_INTERTUPT_MODE_E  int_sense;
    InterruptCallback  fun;
    uint8  int_mode;     				/*0: interrup thread call;  1: interrup routine call directory, mean fast isr mode*/
    InterruptCallbackWithId  fun_v2;    /*call back with a param which is gpio id pointer*/
} MERCURY_GPIO_CFG_S;

typedef enum
{
    /** not force pull up/down, use function default */
    MC_IOMUX_PULL_DEFAULT = 0,
    /** force pull, with neither pull up nor pull down */
    MC_IOMUX_PULL_NONE,
    /** force pull down */
    MC_IOMUX_PULL_DOWN,
	/** force pull up with weakest strength (largest resistance: 20k) */
    MC_IOMUX_PULL_UP,
    /** force pull up with smaller resistance (4.7k) */
    MC_IOMUX_PULL_UP_2,
    /** force pull up with smaller resistance(1.8k. only the pad type is SPU support this type. if the type is WPUS, will be change to PULL_UP_2 automatically) */
    MC_IOMUX_PULL_UP_3,
} MERCURY_IOMUX_PULL_TYPE_E;

typedef enum
{
    /** not force output, use function default */
    MC_IOMUX_INOUT_DEFAULT = 0,
    /** force input */
    MC_IOMUX_FORCE_INPUT,
    /** force output */
    MC_IOMUX_FORCE_OUTPUT,
    /** force output high */
    MC_IOMUX_FORCE_OUTPUT_HI,
    /** force output low */
    MC_IOMUX_FORCE_OUTPUT_LO,
} MERCURY_IOMUX_INOUT_TYPE_E;

//GPIO��ѹ��ID����
typedef enum{
	MC_GIO_POW_DOMAIN_ID_VMMC = 0,
	MC_GIO_POW_DOMAIN_ID_VLCD,
	MC_GIO_POW_DOMAIN_ID_VSIM1,			/*this is id of software, the id of hardware is VDDSIM2*/
}MC_GIO_POW_DOMAIN_ID_E;

extern DWORD GIO_Init(void);
extern BOOL GIO_Deinit(void);
extern VOID GIO_SetBit(DWORD id);
extern VOID GIO_ClrBit(DWORD id);
extern DWORD GIO_GetBit(DWORD id);
extern VOID GIO_SetMode(DWORD id, MERCURY_GPIO_CFG_S* pCfg);

/**
 *  ����pin���ܸ���
 *
 * @param [uint32] gpioID: gpio id(Note: unused)
 * @param [uint32] fun_pad: 
 *		��дfunc & pad ���ֵ, ��MercuryIomuxPin.h�ж����HAL_IOMUX_FUNC_xxx_PAD_xxx��
 *		����оƬ��PAD��������Ӧ��FUNC���ù��ܡ�
 *		ǰ����Ҫ֪��ģ���װpin�Ŷ����ӦоƬ��pad id��
 * @return : void
 */
extern void MercuryPinSetGpio(uint32 gpioID,uint32 fun_pad);

/**
 *  ����pin��������
 *
 * @param [uint32] fun_pad: 
 * 		(1)����gpio�ڹ��ܵ�pin������ֱ�Ӵ�id(0-31), 0: gpio0
 *		(2)�Ƽ���дpad id, ��MercuryIomuxPin.h�ж����HAL_IOMUX_PAD_xxx��
 *		ǰ����Ҫ֪��ģ���װpin�Ŷ����ӦоƬ��pad id��
 * @param [MERCURY_IOMUX_PULL_TYPE_E] pull: ���ö�����Ͷ��塣
 * @return :[BOOL]: TRUE: success; FALSE : fail
 */
extern BOOL PAD_SetPull(uint32 fun_pad, MERCURY_IOMUX_PULL_TYPE_E pull);

/**
 *  ����pin��������������
 *
 * @param [uint32] fun_pad: 
 * 		(1)����gpio�ڹ��ܵ�pin������ֱ�Ӵ�id(0-31), 0: gpio0
 *		(2)�Ƽ���дpad id, ��MercuryIomuxPin.h�ж����HAL_IOMUX_PAD_xxx��
 *		ǰ����Ҫ֪��ģ���װpin�Ŷ����ӦоƬ��pad id��
 * @param [uint32] driving: Ĭ��Ϊ2, ȡֵ��Χ
 * 			pad type: WPUS:  0-3 
 *				 0:2ma 1:4ma 2:6ma 3:8ma
 * 			pad type: SPU:  0-15  (���ù���ΪLCD/SIM1/SDMMC padΪSPU����)
 *				0:2.4ma 		1:5.5ma 		2:7.5ma 		3:10.4ma
 *				4:12.4ma 	5:15ma 		6:17.4ma 	7:20ma
 *				8:24.6ma 	9:27.8ma 	10:29.6ma 	11:32ma
 *				12:34.2ma 	13:37ma 	14:39ma 	15:42ma
 * @return :[BOOL]: TRUE: success; FALSE : fail
 */
extern BOOL PAD_SetDriving(uint32 fun_pad, uint32 driving);

/**
 *  ����pin���������ģʽ
 *  Note: �װ�һ��Ĭ��Ϊ0,��defaultģʽ,ͨ����������޸ģ�
 *  		��gpio���������ƽ���ܿ�ʱ����������ΪMC_IOMUX_INOUT_DEFAULT
 * @param [uint32] fun_pad: 
 *		��дpad id, ��MercuryIomuxPin.h�ж����HAL_IOMUX_PAD_xxx��
 *		ǰ����Ҫ֪��ģ���װpin�Ŷ����ӦоƬ��pad id��
 * @param [MERCURY_IOMUX_INOUT_TYPE_E] pull: ���ö�����Ͷ��塣
 * @return :[BOOL]: TRUE: success; FALSE : fail
 */
 extern BOOL PAD_SetInout(uint32 fun_pad, MERCURY_IOMUX_INOUT_TYPE_E pull);

/**
 *  ����gpio�����ж�ģʽ
 *
 * @param [BOOL] enable: �����ж�ģʽ����/�ر�
 * @return :void
 * @Note:1.��������ģʽ���жϻص�ֱ����ISR��ִ��
 *		  2.��Ҫ��GIO_SetModeǰִ�в���Ч
 */
extern void GIO_SetFastIsrMode (BOOL enable);

/**
 *  ����gpio��ѹ���ѹ
 *
 * @param [MC_GIO_POW_DOMAIN_ID_E] id: ������ѹ��id
 * @param [uint32] mv: ��ѹֵ����λ:mV��range: 1612mv ~ 3200mv
 * @return :[BOOL]: TRUE: success; FALSE : fail
 * @Note:���������������עһ�����г������ĵ�ѹ������
 * V_PAD_1V8_RTC��V_IO18��ѹ���ǹ̶�ֵ���޷����� 
 */ 
extern BOOL GIO_SetPowerDomainLevel(MC_GIO_POW_DOMAIN_ID_E id, uint32 mv);

/**
 *  ����gpio�ж�ģʽ
 *
 * @param [DWORD] id: gpio id
 * @param [MERCURY_INTERTUPT_MODE_E] int_sense: �жϴ���ģʽ
 * @return :[int]: 0: success; others : fail
 * @Note: ������ù�GIO_SetMode�������óɹ������ڹ�����
 * 	1.�����жϡ� 2.�����жϴ���ģʽ
 */ 
extern int GIO_SetIntrMode(DWORD id, MERCURY_INTERTUPT_MODE_E  int_sense);

/**
 *  ���õͰ�gpio�ж��̵߳����ȼ�
 *
 * @param [int] priority: ȡֵ��Χ0-32��Ĭ��Ϊ24
 * @return :[int]: 0: success; others : fail
 * @Note: GPIO�жϻص�������ģʽ: �����ж�ģʽ���ж��߳�ģʽ���˽ӿ����������߳�ģʽ���߳����ȼ�
 */ 
extern int GIO_SetIntThreadPriority(int priority);


#endif //__MERCURY_GPIO_H__
