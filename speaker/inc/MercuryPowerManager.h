/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017
                 Author: 
#########################################################################*/

#ifndef _MERCURY_POWER_MANAGER_H_
#define _MERCURY_POWER_MANAGER_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum _wakelockmodes {
    LOCK_SCREEN=0,
    LOCK_SLEEP,
    LOCK_NONE,
    MERCURY_ENUM_WAKE_MAX = 0x7fffffff
} WAKE_LOCK_MODES;

typedef enum
{
    ADC_ID_0 = 0,
    ADC_ID_1 = 1,
    ADC_ID_2,
    ADC_ID_3,
    ADC_ID_MAX,
    MERCURY_ENUM_ADCID_MAX = 0x7fffffff
} ADC_ID_E;

typedef enum
{
    ADC_SCALE_12BIT_3V = 0, // 实际量程是3.233V
    ADC_SCALE_12BIT_1V2,   // 实际是1.250V
    ADC_SCALE_12BIT_2V4,   // 实际是2.440V
    ADC_SCALE_12BIT_5V,    // 实际是5.000V     
    ADC_SCALE_MAX,
    MERCURY_ENUM_ADCSCALE_MAX = 0x7fffffff
}ADC_SCALE_E;

typedef enum
{
    DEFAULT_AUTO = 0,
    MANUAL_SWITCH_ON,
    MANUAL_SWITCH_OFF,
    MANUAL_SWITCH_MAX
}SWITCH_STATE_E;


typedef enum
{
    CHGMNG_ADP_UNKNOW,
    CHGMNG_ADP_STANDARD,
    CHGMNG_ADP_NONSTANDARD,
    CHGMNG_ADP_USB,
}ADAPTER_TYPE_E;

typedef enum{
    CLK_CAM,
    CLK_PWM_24M,
    CLK_PWM_32K,
    CLK_PWM_26M, 
    CLK_CAM_8M,
    CLK_CAM_12M,
    CLK_CAM_16M,
    CLK_CAM_20M,
    CLK_MAX
}CLK_TYPE;


#define CHARGER_CURRENT_300MA 300
#define CHARGER_CURRENT_350MA 350
#define CHARGER_CURRENT_400MA 400
#define CHARGER_CURRENT_450MA 450
#define CHARGER_CURRENT_500MA 500
#define CHARGER_CURRENT_550MA 550
#define CHARGER_CURRENT_600MA 600
#define CHARGER_CURRENT_650MA 650
#define CHARGER_CURRENT_700MA 700
#define CHARGER_CURRENT_750MA 750
#define CHARGER_CURRENT_800MA 800
//#define CHARGER_CURRENT_850MA   850
#define CHARGER_CURRENT_900MA 900
//#define CHARGER_CURRENT_950MA   950
#define CHARGER_CURRENT_1000MA 1000
//#define CHARGER_CURRENT_1050MA  1050
#define CHARGER_CURRENT_1100MA 1100
#define CHARGER_CURRENT_1200MA 1200
#define CHARGER_CURRENT_MAX 1300


/*This enum defines the lowest switchover point between constant-current and
constant-volatage.*/
typedef enum CCtoCVSwitchoverPoint
{
    CHG_SWITPOINT_LOWEST = 0x0F,
    CHG_SWITPOINT_1 = 0x0E,
    CHG_SWITPOINT_2 = 0x0D,
    CHG_SWITPOINT_3 = 0x0C,
    CHG_SWITPOINT_4 = 0x0B,
    CHG_SWITPOINT_5 = 0x0A,
    CHG_SWITPOINT_6 = 0x09,
    CHG_SWITPOINT_7 = 0x08,
    CHG_SWITPOINT_8 = 0x07,
    CHG_SWITPOINT_9 = 0x06,
    CHG_SWITPOINT_10 = 0x05,
    CHG_SWITPOINT_11 = 0x04,
    CHG_SWITPOINT_12 = 0x03,
    CHG_SWITPOINT_13 = 0x02,
    CHG_SWITPOINT_14 = 0x01,
    CHG_SWITPOINT_15 = 0x00,
    CHG_SWITPOINT_16 = 0x10,
    CHG_SWITPOINT_17 = 0x11,
    CHG_SWITPOINT_18 = 0x12,
    CHG_SWITPOINT_19 = 0x13,
    CHG_SWITPOINT_20 = 0x14,
    CHG_SWITPOINT_21 = 0x15,
    CHG_SWITPOINT_22 = 0x16,
    CHG_SWITPOINT_23 = 0x17,
    CHG_SWITPOINT_24 = 0x18,
    CHG_SWITPOINT_25 = 0x19,
    CHG_SWITPOINT_26 = 0x1A,
    CHG_SWITPOINT_27 = 0x1B,
    CHG_SWITPOINT_28 = 0x1C,
    CHG_SWITPOINT_29 = 0x1D,
    CHG_SWITPOINT_30 = 0x1E,
    CHG_SWITPOINT_HIGHEST = 0x1F
} chgSwitPoiint_e;

#define BAT_TABLE_SIZE (13)
#define PROD_HW_CONFIG (20)
#define CHR_PARAM_RESERVE_SIZE (48 - (PROD_HW_CONFIG + 2 * BAT_TABLE_SIZE))

typedef struct
{
    uint16 warning_vol;
    uint16 shutdown_vol;
    uint16 deadline_vol;
    uint16 warning_count;
} mc_chgDischarge_t;

typedef struct
{
    uint16 ovp_type;
    uint16 ovp_over_vol;
    uint16 ovp_resume_vol;
} mc_chgOvpParam_t;

typedef struct
{
    uint16 rechg_vol;
    uint16 chg_end_vol;
    uint16 bat_safety_vol;
    uint16 standard_chg_current;
    uint16 usb_chg_current;
    uint16 nonstandard_chg_current;
    uint16 chg_timeout;
} mc_chgCharge_t;


// All the voltage unit is mV
typedef struct
{
    struct dischg_param_t
    {
        uint16 voltage_warning;  // when low than the voltage, send the warning msg to client.
        uint16 voltage_shutdown; // when low than the voltage, send the shutdown msg to client.
        uint16 voltage_deadline; //adjust_backlight;    //
        uint16 warning_count;    // when reach the warning count, will send the voltage warning to client.
    } dischg_param;
    struct chg_param_t
    {
        uint16 recharge_voltage;
        uint16 charge_end_voltage;
        uint16 bat_safety_vol;
        uint16 standard_current_type;    //standard adapter charge current type
        uint16 usb_current_type;         //pc charge current type
        uint16 nonstandard_current_type; //nonstandard adapter charge current select,0:usb 1:standard adapter
        uint16 chg_timeout;
    } chg_param;
    struct ovp_param_t
    {
        uint16 ovp_type; //ovp type
        uint16 ovp_over_vol;
        uint16 ovp_resume_vol;
    } ovp_param;
    struct
    {
        uint16 otp_type; ///0:don't support,1:environment temp,2:vbat temp
        uint16 over_low;
        uint16 over_high;
        uint16 resume_low;
        uint16 resume_high;
    } otp_param;
    uint16 hw_switch_point; //set the lowest hardware switchover point between cc-cv
    // The table define as following:
    //  If the voltage >= the low WORD of table[n], the capacity is the high WORD of table[n].
    // The last one item of the table must be 0xFFFFFFFF
    uint16 dischg_bat_capacity_table[BAT_TABLE_SIZE];
    uint16 chg_bat_capacity_table[BAT_TABLE_SIZE];
    uint16 reseved[CHR_PARAM_RESERVE_SIZE];
} chgParamProd_t; // Size is 112 bytes

 /*************************************
申请wakelock, 只要有wakelock,系统就不能进去休眠
 *************************************/
int  WakeLock(WAKE_LOCK_MODES LockMode, const BYTE *LockName);
 
/*************************************
释放wakelock, 系统进入休眠之前,申请的wakelock,必须全部释放 
 *************************************/
int  WakeUnlock(WAKE_LOCK_MODES LockMode, const BYTE *LockName);

/*************************************
系统关机
*************************************/
int  PowerOff(void);

/*************************************
系统重启
*************************************/
int  PowerReboot(void);

/*************************************
获取电池剩余电量百分比
*************************************/
uint32 Power_GetVoltagetoPercent(void);

/*************************************
设置电池温度检查的高温值
*************************************/
void SetChgOverHighTemp(uint16 highTemp);

/*************************************
设置电池温度检查的低温检查值
*************************************/
void SetChgOverLowTemp(uint16 lowTemp);

/*************************************
设置再次充电的电压
*************************************/
void SetRechgVol(uint16 rechgVol);

/*************************************
设置结束充电电压
*************************************/
void SetChgEndVol(uint16 chg_end_vol);

/*************************************
开关充电
*************************************/
int SetChgSwtich(SWITCH_STATE_E chgSwitch);

/*************************************
不支持
*************************************/
int SetAutoIdentAdp(BOOLEAN autoIdentity);

/*************************************
获取充电类型, 只返回标准充电器类型
*************************************/
int GetAdpType();

/*获取ADC 的值
[in]ADC_ID_E : ADC_ID_0 adc1, ADC_ID_1 adc2, ADC_ID_2 adc3, ADC_ID_3 adc4
[in]ADC_SCALE_E:设置量程范围, 量程分4挡3.3v, 1.25V, 2.44V, 5V
*/
int ADC_GetResult(ADC_ID_E adcID, ADC_SCALE_E adcScale);

/*设置系统自动进入睡眠状态
[in]enable 1 表示使能自动休眠功能, 0 表示退出休眠
*/
int SystemAutoSleep(BOOLEAN enable);

/*设置是否启用底层的温度检查,温度达到预期会停止充电
[in]enable 1 启用 0 不启用
*/
int SetBatTempDetect(BOOLEAN enable);

/*获取底层温度检查的是否启用的标志
*/
BOOLEAN GetBatTempDetect(void);

/*设置跟充电相关的参数, 参考demo代码
特别说明hw_switch_point 这个不能设置, 这个参数是用来校准的,并不是设置CC-CV的电压,
这个电压固定是4.2V,底层已经校准过
*/
int SetChargerBoardParam(chgParamProd_t *chr_param);


/*获取ADCchnnel 的电压值
[in]ADC_ID_E : ADC_ID_0 adc1, ADC_ID_1 adc2, ADC_ID_2 adc3, ADC_ID_3 adc4
[in]ADC_SCALE_E:设置量程范围, 量程分4挡3.3v, 1.25V, 2.44V, 5V
*/
int AdcChannelGetVol(uint8 adc_channel, uint8 adcScale);


/*获取低功耗使能状态
 *返回：0:未打开     1:已打开
*/
char GetLowPowerState(void);

/*设置低功耗使能
 *[in]enable: 0:关闭     1:打开
*/
int SetLowPowerEnable(char enable);

void PmuSet7sReset(bool enable);

BOOLEAN PmuSetPowerLevel(uint32 id, uint32 mv);

/*
设置7s reset门限值，取值范围（2-16)
*/
int PmuSet7sResetThreshold(uint8_t rstThreshold);

int SetPoweronLongPressDuration(int time);
int GetPoweronLongPressDuration(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_MERCURY_POWER_MANAGER_H_

