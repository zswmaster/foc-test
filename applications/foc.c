/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-11     7510       the first version
 */
#include <foc.h>
#include <pwm.h>
#include <math.h>
#include <vofa_plus.h>
#include <adcread.h>
#include <math.h>
/******************************************/
float voltage_power_supply=12;/*直流的母线电压24v*/
float sensor_offset=0;
float zero_electric_angle=0;
float Ta,Tb,Tc;
extern rt_uint32_t angle;
extern struct rt_device_pwm *pwm_dev;
extern rt_uint32_t pluseA;
extern rt_uint32_t pluseB;
extern rt_uint32_t pluseC;
extern int k;
extern float theta;
int pp=0;
struct motor moto1={
         {0},
         {0},
         3.14159265359/12,
         0

};
 struct PI PI_Q={0,0,0,0,0,0,0,200};
 struct PI PI_D={0.01,0.01,0,0,0,0,0,200};
//将角度规定在一个范围内
 void moto1_init()
 {
     moto1.iAlpha=0;
     moto1.iBeta=0;
     moto1.ic=0;
     moto1.id=0;
     moto1.iq=0;
     moto1.speed=3.14159265359/180;

 }
 void PID_init()
 {

     PI_Q.KP= -4.0F;
     PI_Q.KI= -40.0F;
     PI_D.KP= -4.0F;
     PI_D.KI= -40.0F;
     PI_D.out_max = 200;
     PI_Q.out_max = 200;
 }

float _normalizeAngle(float angle)
{
    //printf("anglereal=%f\n",angle);
    float a=fmod(angle,_2PI);/*È¡Ä£*/
    return a>=0 ? a:(a + _2PI);
}
/******************************************/
void Clarke_Park(struct motor * moto, rt_uint32_t currA, rt_uint32_t currC)
{
    moto->iAlpha = (float)currA;
    moto->iBeta = (float)(currA + 2 * currC) * 0.5773502692f;

    moto->id = moto->iAlpha * moto->cosVal + moto->iBeta * moto->sinVal;
    moto->iq = -moto->iAlpha * moto->sinVal + moto->iBeta * moto->cosVal;

}
static void CurrentPIControlID(struct PI * PID, float target)
{

    PID->bias=target-PID->last_bias;

      PID->out_value=PID->KP*(PID->bias-PID->last_bias)+PID->KI*PID->bias;

      PID->last_bias=PID->bias;


      if (PID->out_value > fabs(PID->out_max)) {
          PID->out_value = fabs(PID->out_max);
      }

      if (PID->out_value < -fabs(PID->out_max)) {
          PID->out_value = -fabs(PID->out_max);
      }
}
static void CurrentPIControlIQ(struct PI * PID,float target)
{

    PID->bias=target-PID->last_bias;

    PID->out_value=PID->KP*(PID->bias-PID->last_bias)+PID->KI*PID->bias;

    PID->last_bias=PID->bias;


    if (PID->out_value > fabs(PID->out_max)) {
        PID->out_value = fabs(PID->out_max);
    }

    if (PID->out_value < -fabs(PID->out_max)) {
        PID->out_value = -fabs(PID->out_max);
    }
}
//FOC核心函数，输入Ud、Uq和电角度，输出PWM
void setPhaseVoltage(float Uq,float Ud,float angle_el)
{
    float Uref;
    float U_alpha,U_beta;

    rt_uint32_t sector;


    float T0,T1,T2;



    float V1,V2,V3;

    int A,B,C;/*计算扇区的中间变量，布尔类型*/
    int N;/*判断扇区的变量*/


//（1）计算Vref
    angle_el=_normalizeAngle(angle_el+_PI_2);
    moto1.cosVal = cos(angle_el);
    moto1.sinVal = sin(angle_el);
    U_alpha=Ud*cos(angle_el)-Uq*sin(angle_el);
    U_beta= Ud*sin(angle_el)+Uq*cos(angle_el);

    Uref=sqrt(U_alpha*U_alpha+U_beta*U_beta)/12;
    if(Uref>0.577)Uref=0.577;
    if(Uref<-0.577)Uref=-0.577;
 //   rt_kprintf("1ok\n");

//(2）判断参考电压的扇区
    sector=(angle_el / _PI_3)+1;
  //  rt_kprintf("2ok\n");

//（3）计算相邻电压矢量和零矢量作用的时间
    T1=_SQRT3*sin(sector*_PI_3-angle_el)*Uref;
    T2=_SQRT3*sin(angle_el-(sector-1.0)*_PI_3)*Uref;
    T0=1-T1-T2;

  //  rt_kprintf("3ok\n");
//（4）计算各相桥臂开关时间
    switch(sector){

        case 1:
                Ta=T1+T2+T0/2;
                Tb=T2+T0/2;
                Tc=T0/2;
                break;
        case 2:
                Ta=T1+T0/2;
                Tb=T1+T2+T0/2;
                Tc=T0/2;
                break;
        case 3:
                Ta=T0/2;
                Tb=T1+T2+T0/2;
                Tc=T2+T0/2;
                break;
        case 4:
                Ta=T0/2;
                Tb=T1+T0/2;
                Tc=T1+T2+T0/2;
                break;
        case 5:
                Ta=T2+T0/2;
                Tb=T0/2;
                Tc=T1+T2+T0/2;
                break;
        case 6:
                Ta=T1+T2+T0/2;
                Tb=T0/2;
                Tc=T1+T0/2;
                break;
        default://possible error state
            Ta=0;
            Tb=0;
            Tc=0;
    }

    //PWM信号输出，占空比配置

    pluseA=(rt_uint32_t)(Ta*pluse_open);
    pluseB=(rt_uint32_t)(Tb*pluse_open);
    pluseC=(rt_uint32_t)(Tc*pluse_open);
    rt_pwm_set(pwm_dev, 1, pluse_open, pluseA);
    rt_pwm_set(pwm_dev, 2, pluse_open, pluseB);
    rt_pwm_set(pwm_dev, 3, pluse_open, pluseC);
    rt_thread_mdelay(1);





    //printf("anglerrrr:%d\n",pp);
//    printf("angle:%f\n",angle_el);
//    printf("UREF:%f\n",Uref);
   // printf("%f,%f,%f\n",Ta*100,Tb*100,Tc*100);
    //printf("speed:%f",moto1.speed);
//    printf("%f\n",Tb);
//    printf("%f\n",Tc);
   // printf("\n");
    //rt_thread_mdelay(1);

}
//void FOCIQControl(float angle_el,float Uq,float Ud)
//{
//    float Uref;
//    float U_alpha,U_beta;
//
//    rt_uint32_t sector;
//
//
//    float T0,T1,T2;
//
//
//
//    float V1,V2,V3;
//
//    int A,B,C;/*计算扇区的中间变量，布尔类型*/
//    int N;/*判断扇区的变量*/
//
//
////（1）计算Vref
//    angle_el=_normalizeAngle(angle_el+_PI_2);
//
//    U_alpha=Ud*cos(angle_el)-Uq*sin(angle_el);;
//    U_beta= Ud*sin(angle_el)+Uq*cos(angle_el);
//
//    Uref=sqrt(U_alpha*U_alpha+U_beta*U_beta)/12;
//    if(Uref>0.577)Uref=0.577;
//    if(Uref<-0.577)Uref=-0.577;
// //   rt_kprintf("1ok\n");
//
////(2）判断参考电压的扇区
//    sector=(angle_el / _PI_3)+1;
//  //  rt_kprintf("2ok\n");
//
////（3）计算相邻电压矢量和零矢量作用的时间
//    T1=_SQRT3*sin(sector*_PI_3-angle_el)*Uref;
//    T2=_SQRT3*sin(angle_el-(sector-1.0)*_PI_3)*Uref;
//    T0=1-T1-T2;
//
//  //  rt_kprintf("3ok\n");
////（4）计算各相桥臂开关时间
//    switch(sector){
//
//        case 1:
//                Ta=T1+T2+T0/2;
//                Tb=T2+T0/2;
//                Tc=T0/2;
//                break;
//        case 2:
//                Ta=T1+T0/2;
//                Tb=T1+T2+T0/2;
//                Tc=T0/2;
//                break;
//        case 3:
//                Ta=T0/2;
//                Tb=T1+T2+T0/2;
//                Tc=T2+T0/2;
//                break;
//        case 4:
//                Ta=T0/2;
//                Tb=T1+T0/2;
//                Tc=T1+T2+T0/2;
//                break;
//        case 5:
//                Ta=T2+T0/2;
//                Tb=T0/2;
//                Tc=T1+T2+T0/2;
//                break;
//        case 6:
//                Ta=T1+T2+T0/2;
//                Tb=T0/2;
//                Tc=T1+T0/2;
//                break;
//        default://possible error state
//            Ta=0;
//            Tb=0;
//            Tc=0;
////    //PWM信号输出，占空比配置
////
//    pluseA=(rt_uint32_t)(Ta*pluse_open);
//    pluseB=(rt_uint32_t)(Tb*pluse_open);
//    pluseC=(rt_uint32_t)(Tc*pluse_open);
//    rt_pwm_set(pwm_dev, 1, pluse_open, pluseA);
//    rt_pwm_set(pwm_dev, 2, pluse_open, pluseB);
//    rt_pwm_set(pwm_dev, 3, pluse_open, pluseC);
//    rt_thread_mdelay(1);
//
//}
void FocContorl(struct motor * moto, struct PI * PID, struct PI * PIQ)
{
    //0.获取电气角度

    while(1)
    {
    theta -=  moto1.speed;
          if (theta <= 0)//2*3.14159265359
          {
              theta+=2*3.14159265359;


          }
    Clarke_Park(&moto1,I1,I3);
    //2.做PID闭环
    CurrentPIControlID(PID,0);
    CurrentPIControlIQ(PIQ,0.5);
   // FOCIQControl(theta, PIQ->out_value, PID->out_value);
    setPhaseVoltage(PIQ->out_value,PID->out_value,theta);

    }

    //4.输出SVPWM
    //pFOC->SvpwmGenerate(PI_Q->out_max,PI_D->out_max);
}

