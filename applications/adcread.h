/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-09     小恐龙       the first version
 */
#ifndef __ADCREAD_H__
#define __ADCREAD_H__
#include <rtdbg.h>
#include <board.h>
#include <rtdevice.h>


#define ADC_DEV_NAME        "adc1"      /* ADC 设备名称 */
#define ADC_DEV_CHANNEL     5           /* ADC 通道 */
#define REFER_VOLTAGE       330         /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */
rt_uint32_t value1,value2;
float I1,I3,vol1,vol2;



void Adc_entry(void* paremeter);
int adc_vol_sample();





#endif /* __BOARD_H__ */
