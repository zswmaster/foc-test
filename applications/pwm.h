/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-11     7510       the first version
 */
#ifndef APPLICATIONS_PWM_H_
#define APPLICATIONS_PWM_H_
#include <rtthread.h>
#include <rtdevice.h>
//设置log文件
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#define PWM_DEV_NAME "pwm2" /* PWM 设 备 名 称 */
#define PWM_DEV_CHANNEL 1
#define PWM_DEV_CHANNEL2 2
#define PWM_DEV_CHANNEL3 3
void pwm_init();
int pwm_test(void);
void foc_pwm();

#endif /* APPLICATIONS_PWM_H_ */
