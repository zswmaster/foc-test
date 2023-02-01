/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-11     7510       the first version
 */
#include <pwm.h>
#define DBG_TAG "pwm_test.c"
extern float Ta,Tb,Tc;
rt_uint32_t pluseA=10;
rt_uint32_t pluseB=10;
rt_uint32_t pluseC=10;
struct rt_device_pwm *pwm_dev; /* PWM 设 备 句 柄 */
int pwm_test(void)
{
    rt_uint32_t period , pulse;
    //设置周期及 脉冲长度
    period = 100000;
    pulse =  100000;
    pwm_dev = (struct rt_device_pwm*)rt_device_find(PWM_DEV_NAME); //查找PWM设备
    //配置PWM
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);    //占空比应该是50%
    if (rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL) == RT_EOK)
    {
        LOG_D("PWM Init is ok \t\n");
        return RT_EOK;
    }
    return RT_EOK;
}
void pwm_init()
{
    pwm_dev = (struct rt_device_pwm*)rt_device_find(PWM_DEV_NAME); //查找PWM设备
    rt_pwm_enable(pwm_dev, 1);
    rt_pwm_enable(pwm_dev, 2);
    rt_pwm_enable(pwm_dev, 3);

}
void foc_pwm()
{

    while(1)
    {
        rt_pwm_set(pwm_dev, 1, 20000, Ta*10000);
        rt_pwm_set(pwm_dev, 2, 20000, Tb*10000);
        rt_pwm_set(pwm_dev, 3, 20000, Tc*10000);


        rt_thread_mdelay(1);
    }
}
MSH_CMD_EXPORT(pwm_test, pwm device sample);
