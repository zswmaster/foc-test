/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-09     小恐龙       the first version
 */
#include <as5600.h>

rt_uint16_t angle=0;


static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */

/* 写传感器寄存器 */
rt_err_t write_reg(struct rt_i2c_bus_device *bus)
{
    struct rt_i2c_msg msgs;
    rt_uint8_t buf = Angle_Hight_Register_Addr;

    msgs.addr = Slave_Addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = &buf;
    msgs.len = 1;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/* 读传感器寄存器数据 */
rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs;

    msgs.addr = Slave_Addr;
    msgs.flags = RT_I2C_RD;
    msgs.buf = buf;
    msgs.len = len;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

void read_angle()
{
    rt_uint8_t temp[2];

    write_reg(i2c_bus);      /* 发送命令 */
    read_regs(i2c_bus, 2, temp); /* 获取传感器数据 */

    temp[0] = temp[0] & 0xF;
     angle = temp[0]*256 + temp[1];
    rt_kprintf("angle:%d\n",angle);
}
 void hall_init(const char *name)
{
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(name);

    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("can't find %s device!\n", name);
    }
    else
    {

    }
}
 void hall_sample(int argc, char *argv[])
{

    hall_init(HALL_I2C_PITCH_NAME);

    read_angle();
}
void iic_entry()
{
    hall_init(HALL_I2C_PITCH_NAME);


    while(1)
    {
     read_angle();
     rt_thread_mdelay(1);
    }



}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(hall_sample, i2c hall sample);
