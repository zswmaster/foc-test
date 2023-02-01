/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-09     小恐龙       the first version
 */
#include <adcread.h>
#include <foc.h>
rt_uint32_t value1,value2;
float I1,I3,vol1,vol2;
void Adc_entry(void* paremeter)
{
    rt_adc_device_t adc_dev;

    rt_err_t ret = RT_EOK;

    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
    }
    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);
    if(ret == RT_EOK)
    {
        rt_kprintf("adc sample run success!  find %s device!\n", ADC_DEV_NAME);
    }
    ret = rt_adc_enable(adc_dev,4 );
        if(ret == RT_EOK)
        {
            rt_kprintf("adc sample run success!  find %s device!\n", ADC_DEV_NAME);
        }
    while(1)
    {
        /* 读取采样值 */
        value1 = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
       // rt_kprintf("the value1 is :%d \n", value1);
        /* 转换为对应电压值 */
        vol1 = (value1 * REFER_VOLTAGE / CONVERT_BITS);
        I1 = (vol1-160)/5;
       // printf("the I1 is :%f\n", I1);
      //  rt_kprintf("the voltage1 is :%d.%02d \n", vol1 / 100, vol1 % 100);
       // rt_thread_delay(500);
        value2 = rt_adc_read(adc_dev, 4);
       // rt_kprintf("the value2 is :%d \n", value2);
        /* 转换为对应电压值 */
         vol2 = (value2 * REFER_VOLTAGE / CONVERT_BITS);
         I3 = (vol2-160)/5;
         //printf("%f,%f",PI_D.out_value,PI_Q.out_value);
        // if(moto1.id!=0&&moto1.iq!=0)
         //printf("%f,%f\n",moto1.id,moto1.iq);
        // printf("the I3 is :%f\n", I3);
        // rt_kprintf("the voltage2 is :%d.%02d \n", vol2 / 100, vol2 % 100);
        // printf("%f,%f,%f\n",I1*100,-(I1+I3)*100,I3*100);
         //printf("%d,%d\n",value1,value2);
         //printf("%d,%d\n",vol1,vol2);
         //rt_hw_us_delay(10000);
         printf("%f,%f\n",I1,I3);
        rt_thread_delay(500);
    }
}

int adc_vol_sample(int argc, char *argv[])
{
    rt_adc_device_t adc_dev;
    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }

    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);

    /* 读取采样值 */
    value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
    rt_kprintf("the value is :%d \n", value);

    /* 转换为对应电压值 */
    vol = value * REFER_VOLTAGE / CONVERT_BITS;
    rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);

    /* 关闭通道 */
    ret = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(adc_vol_sample, adc voltage convert sample);
