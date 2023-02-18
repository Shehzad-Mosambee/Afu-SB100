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
    LEVEL,		/*电平变化时触发:当电平为高时，内部设置为低电平触发，反之亦然*/
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGE,	
    NO_INT,    	 /*中断关闭*/
	HIGH_LEVEL,  /*高电平触发:当电平维持为触发电平，会一直产生中断，需要关中断*/  
	LOW_LEVEL,	 /*低电平触发:当电平维持为触发电平，会一直产生中断，需要关中断*/	  
    INVALID_INT,
    MERCURY_ENUM_INTERTUPT_MAX = 0x7fffffff
} MERCURY_INTERTUPT_MODE_E;

/*
扩展两个成员，用途:
1.int8  int_mode;   
	单独设置指定gpio中断处理模式
	0:在底包中断线程执行
	1:直接在中断服务程序中执行, 即fast isr模式
	Note: 如果GIO_SetFastIsrMode(1)，int_mode设置与否，都为fast isr模式
		  如果GIO_SetFastIsrMode(0)，指定gpio的中断处理模式取决于int_mode值?
2.InterruptCallbackWithId  fun_v2;		  
	函数指针定义带一个表示gpio id的指针参数
	Note: 如果结构体成员fun有赋值，中断产生时执行fun，而不执行fun_v2
		只有fun=NULL且fun_v2有赋函数指针的情况下被执行。
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

//GPIO电压域ID定义
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
 *  设置pin功能复用
 *
 * @param [uint32] gpioID: gpio id(Note: unused)
 * @param [uint32] fun_pad: 
 *		填写func & pad 组合值, 见MercuryIomuxPin.h中定义的HAL_IOMUX_FUNC_xxx_PAD_xxx。
 *		即对芯片的PAD脚设置相应的FUNC复用功能。
 *		前提需要知道模块封装pin脚定义对应芯片的pad id。
 * @return : void
 */
extern void MercuryPinSetGpio(uint32 gpioID,uint32 fun_pad);

/**
 *  设置pin脚上下拉
 *
 * @param [uint32] fun_pad: 
 * 		(1)对于gpio口功能的pin，可以直接传id(0-31), 0: gpio0
 *		(2)推荐填写pad id, 见MercuryIomuxPin.h中定义的HAL_IOMUX_PAD_xxx。
 *		前提需要知道模块封装pin脚定义对应芯片的pad id。
 * @param [MERCURY_IOMUX_PULL_TYPE_E] pull: 详见枚举类型定义。
 * @return :[BOOL]: TRUE: success; FALSE : fail
 */
extern BOOL PAD_SetPull(uint32 fun_pad, MERCURY_IOMUX_PULL_TYPE_E pull);

/**
 *  设置pin脚上拉驱动能力
 *
 * @param [uint32] fun_pad: 
 * 		(1)对于gpio口功能的pin，可以直接传id(0-31), 0: gpio0
 *		(2)推荐填写pad id, 见MercuryIomuxPin.h中定义的HAL_IOMUX_PAD_xxx。
 *		前提需要知道模块封装pin脚定义对应芯片的pad id。
 * @param [uint32] driving: 默认为2, 取值范围
 * 			pad type: WPUS:  0-3 
 *				 0:2ma 1:4ma 2:6ma 3:8ma
 * 			pad type: SPU:  0-15  (复用功能为LCD/SIM1/SDMMC pad为SPU类型)
 *				0:2.4ma 		1:5.5ma 		2:7.5ma 		3:10.4ma
 *				4:12.4ma 	5:15ma 		6:17.4ma 	7:20ma
 *				8:24.6ma 	9:27.8ma 	10:29.6ma 	11:32ma
 *				12:34.2ma 	13:37ma 	14:39ma 	15:42ma
 * @return :[BOOL]: TRUE: success; FALSE : fail
 */
extern BOOL PAD_SetDriving(uint32 fun_pad, uint32 driving);

/**
 *  设置pin脚输入输出模式
 *  Note: 底包一般默认为0,即default模式,通常情况无需修改，
 *  		当gpio输入输出电平不受控时，考虑配置为MC_IOMUX_INOUT_DEFAULT
 * @param [uint32] fun_pad: 
 *		填写pad id, 见MercuryIomuxPin.h中定义的HAL_IOMUX_PAD_xxx。
 *		前提需要知道模块封装pin脚定义对应芯片的pad id。
 * @param [MERCURY_IOMUX_INOUT_TYPE_E] pull: 详见枚举类型定义。
 * @return :[BOOL]: TRUE: success; FALSE : fail
 */
 extern BOOL PAD_SetInout(uint32 fun_pad, MERCURY_IOMUX_INOUT_TYPE_E pull);

/**
 *  设置gpio快速中断模式
 *
 * @param [BOOL] enable: 快速中断模式开启/关闭
 * @return :void
 * @Note:1.开启快速模式后，中断回调直接在ISR中执行
 *		  2.需要在GIO_SetMode前执行才有效
 */
extern void GIO_SetFastIsrMode (BOOL enable);

/**
 *  设置gpio电压域电压
 *
 * @param [MC_GIO_POW_DOMAIN_ID_E] id: 所属电压域id
 * @param [uint32] mv: 电压值，单位:mV，range: 1612mv ~ 3200mv
 * @return :[BOOL]: TRUE: success; FALSE : fail
 * @Note:规格书引脚描述备注一栏，列出所属的电压域，其中
 * V_PAD_1V8_RTC和V_IO18电压域是固定值，无法设置 
 */ 
extern BOOL GIO_SetPowerDomainLevel(MC_GIO_POW_DOMAIN_ID_E id, uint32 mv);

/**
 *  设置gpio中断模式
 *
 * @param [DWORD] id: gpio id
 * @param [MERCURY_INTERTUPT_MODE_E] int_sense: 中断触发模式
 * @return :[int]: 0: success; others : fail
 * @Note: 必须调用过GIO_SetMode才能配置成功，用于过程中
 * 	1.开关中断。 2.更改中断触发模式
 */ 
extern int GIO_SetIntrMode(DWORD id, MERCURY_INTERTUPT_MODE_E  int_sense);

/**
 *  设置低包gpio中断线程的优先级
 *
 * @param [int] priority: 取值范围0-32，默认为24
 * @return :[int]: 0: success; others : fail
 * @Note: GPIO中断回调有两种模式: 快速中断模式和中断线程模式，此接口用于设置线程模式的线程优先级
 */ 
extern int GIO_SetIntThreadPriority(int priority);


#endif //__MERCURY_GPIO_H__
