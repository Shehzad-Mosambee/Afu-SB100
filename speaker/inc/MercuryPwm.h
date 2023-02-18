#ifndef _PWM_HEADER_
#define _PWM_HEADER_

/**
 * pwm��ʼ��
 *
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Init(void);

/**
 * pwm��������
 *
 * @param[in]  freq        ����Ƶ��[200-100K]
 * @param[in]  duty_cycle  ����ռ�ձ�[0-100]
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Config(uint32  freq,  uint16  duty_cycle);

/**
 * pwm��ʼ���
 *
 * @param[in]  times ����ʱ��, ��λms, ���Ϊ0 �������MercuryPWM_Stopֹͣ
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Start(uint32 times);

/**
 * pwmֹͣ���
 *
 * @param[in]  id          �豸id
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Stop(void);

/**
 * pwmȥ��ʼ��
 *
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_Deinit(void);


/**
 * pwm��������
 * @param[in]  freq        ����Ƶ��[PWT/PWL 50-11M HZ]
 * @param[in]  duty_cycle  ����ռ�ձ�[PWT/PWL 0-100]
 * @param[in]  clk_div     [0-255], ʱ��ԴAPB200M ����26M, pwt_clk= APB/(clk_div+1)
    pwt_peroid = (pwt_clk/freq - 1)/8 ���ֵ�ķ�Χ��[1-2047]
    pwt_duty = pwt_peroid*duty_cycle/100 ���ֵ�ķ�Χ��[1-2047],�ұ�pwt_peroidС
    ��Ҫ��pwm Ƶ�ʱȽϵ�clk_div ��ֵȡ��һЩ,Ƶ��Ҫ����clk_div ֵҪСһЩ
 * @return 0 SUCCESS  -1 FAIL
 */
int MercuryPWM_ConfigExt(uint32 freq, uint16 duty_cycle, uint8 clk_div);
#endif
