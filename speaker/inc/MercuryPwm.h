#ifndef _PWM_HEADER_
#define _PWM_HEADER_

/**
 * pwm初始化
 *
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Init(void);

/**
 * pwm参数配置
 *
 * @param[in]  freq        配置频率[200-100K]
 * @param[in]  duty_cycle  配置占空比[0-100]
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Config(uint32  freq,  uint16  duty_cycle);

/**
 * pwm开始输出
 *
 * @param[in]  times 持续时间, 单位ms, 如果为0 则必须由MercuryPWM_Stop停止
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Start(uint32 times);

/**
 * pwm停止输出
 *
 * @param[in]  id          设备id
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Stop(void);

/**
 * pwm去初始化
 *
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Deinit(void);


/**
 * pwm参数配置
 * @param[in]  freq        配置频率[PWT/PWL 50-11M HZ]
 * @param[in]  duty_cycle  配置占空比[PWT/PWL 0-100]
 * @param[in]  clk_div     [0-255], 时钟源APB200M 休眠26M, pwt_clk= APB/(clk_div+1)
    pwt_peroid = (pwt_clk/freq - 1)/8 这个值的范围是[1-2047]
    pwt_duty = pwt_peroid*duty_cycle/100 这个值的范围是[1-2047],且比pwt_peroid小
    需要的pwm 频率比较低clk_div 的值取大一些,频率要高则clk_div 值要小一些
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_ConfigExt(uint32 freq, uint16 duty_cycle, uint8 clk_div);
#endif
