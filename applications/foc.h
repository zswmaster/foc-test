/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-11     7510       the first version
 */
#ifndef APPLICATIONS_FOC_H_
#define APPLICATIONS_FOC_H_
#include <math.h>
#include <rtthread.h>
#define _PI 3.14159265359
#define _PI_2 1.57079632679
#define _PI_3 1.0471975512
#define _2PI 6.28318530718
#define _3PI_2 4.71238898038
#define _PI_6 0.52359877559
#define _SQRT3 1.73205080757

#define pluse_open 100000

struct motor
{
float iAlpha,iBeta,ic;
float id,iq;
float speed;
float cosVal,sinVal;

}moto1;
struct PI
{
    float KP;           //比例

    float KI;          //积分

    float KD;         //微分

    float bias;       //当前差

    float last_bias;  //上次差

    float last_old_bias;  //上上次差

    float out_value;  //输出的控制变量
    float out_max;


}PI_D,PI_Q;

void setPhaseVoltage(float Uq,float Ud,float angle_el);
float _normalizeAngle(float angle);
extern void moto1_init();
void FocContorl(struct motor * moto, struct PI * PID, struct PI * PIQ);
#endif /* APPLICATIONS_FOC_H_ */
