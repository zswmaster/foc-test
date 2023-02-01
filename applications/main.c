/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-09     小恐龙    first version
 */

#include <rtthread.h>
#include <adcread.h>
#include <as5600.h>
#include <pwm.h>
#include <foc.h>
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <board.h>
#include <rtdevice.h>
#include <vofa_plus.h>
rt_thread_t Adc_thread = RT_NULL;
rt_thread_t IIC_thread = RT_NULL;
rt_thread_t FOC_thread = RT_NULL;
rt_thread_t vofa_thread = RT_NULL;
extern rt_uint16_t angle;
float theta=2*3.14159265359;

int k = 0;

rt_base_t en1;
int adc_threrad_init();
int iic_thread_init();
int foc_thread_init();
int vofa_thread_init();
int main(void)
{
    en1 = rt_pin_get("PF.9");
    moto1_init();
    PID_init();

   rt_pin_mode(en1, PIN_MODE_OUTPUT);
   rt_pin_write(en1, 1);
   int adc = adc_thrad_init();
  // int iic = iic_thread_init();
   //setPhaseVoltage(12.0,0,(float)angle);
   int foc = foc_thread_init();
   //int vofa = vofa_thread_init();







}
void test1()
{




 while (k<21) {

     theta -=  moto1.speed;
      if (theta <= 0)//2*3.14159265359
      {
          theta+=2*3.14159265359;
          k++;
         // LOG_E("%d\n",k);

      }
     setPhaseVoltage(10,0,theta);

 }

}
void test2()
{
    //theta=0;



 while(k>=0) {
     theta +=  3.1415926/9;
     if (theta >= 2*3.14159265359)//2*3.14159265359
     {
         theta-=2*3.14159265359;
         k--;

        // LOG_E("%d\n",k);

     }
     setPhaseVoltage(1,0,theta);



 }

}
void test3()
{






     theta -=  moto1.speed;
      if (theta <= 0)//2*3.14159265359
      {
          theta+=2*3.14159265359;

         // LOG_E("%d\n",k);

      }
     setPhaseVoltage(1,0,theta);



}
void foc_entry()
{

    pwm_init();

    while(1)
    {         // test3();
               // rt_pin_write(en1, 0);
        FocContorl( &moto1, &PI_Q, &PI_D);
               rt_hw_us_delay(10);
    }
}
int adc_thrad_init()
{
    Adc_thread = rt_thread_create("adc1",
               Adc_entry,
                   RT_NULL,
                   2048,
                   15,
                   20);
           if(Adc_thread != RT_NULL)
               rt_thread_startup(Adc_thread);
           else
               return -1;

}
int iic_thread_init()
{
    IIC_thread = rt_thread_create("iic1",
                                iic_entry,
                                    RT_NULL,
                                    512,
                                    15,
                                    20);
                            if(IIC_thread != RT_NULL)
                                rt_thread_startup(IIC_thread);
                            else
                                return -1;
}
int foc_thread_init()
{
    FOC_thread = rt_thread_create("foc1",
            foc_entry,
                                        RT_NULL,
                                        2048,
                                        13,
                                        10);
                                if(FOC_thread != RT_NULL)
                                    rt_thread_startup(FOC_thread);
                                else
                                    return -1;

}
int vofa_thread_init()
{
    vofa_thread = rt_thread_create("vofa1",
            vofa_entry,
                                        RT_NULL,
                                        512,
                                        15,
                                        20);
                                if(vofa_thread != RT_NULL)
                                    rt_thread_startup(vofa_thread);
                                else
                                    return -1;

}

