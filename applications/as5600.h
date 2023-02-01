/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-09     小恐龙       the first version
 */
#ifndef APPLICATIONS_AS5600_H_
#define APPLICATIONS_AS5600_H_

#include <rtdevice.h>

#define HALL_I2C_PITCH_NAME     "i2c1"


#define Slave_Addr 0x36
#define Write_Bit 0
#define Read_Bit 1
#define Angle_Hight_Register_Addr 0x1b
#define Angle_Low_Register_Addr 0x1c

rt_err_t write_reg(struct rt_i2c_bus_device *bus);
rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf);
void read_angle();
void hall_init(const char *name);
void iic_entry();

#endif /* APPLICATIONS_AS5600_H_ */
