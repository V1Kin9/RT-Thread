/*
 * File      : drv_usart.c
 *
 * Copyright (c) 2018-2020, UCTECH IP 
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-4      ViKing       first implementation
 */
#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

/* CORE_BOARD LED */
#define LEDC1 0
#define LEDC2 1
#define LEDC3 2
#define LEDC4 3

/* LS_EXT LED */
#define LEDL0 4
#define LEDL1 5
#define LEDL2 6
#define LEDL3 7

/* Button */
#define SW2 8
#define SW3 9
#define SW4 10

/* Switch */
#define SW6 12
#define SW7 13
#define SW8 14
#define SW9 15

/* Pin */
#define D4 16
#define D5 17
#define D6 18
#define D7 19
#define D8 20
#define D9 21
#define D10 24
#define D11 25
#define D12 26
#define D13 27
#define D14 28
#define D15 29

void ls_pin_mode(struct rt_device *device, rt_base_t pin, rt_base_t mode);
void ls_pin_write(struct rt_device *device, rt_base_t pin, rt_base_t value);
int ls_pin_read(struct rt_device *device, rt_base_t pin);
rt_err_t ls_pin_attach_irq(struct rt_device *device, rt_int32_t pin,rt_uint32_t mode, void (*hdr)(void *args), void *args);
rt_err_t ls_pin_detach_irq(struct rt_device *device, rt_int32_t pin);
rt_err_t ls_pin_irq_enable(struct rt_device *device, rt_base_t pin, rt_uint32_t enabled);
int hw_pin_init(void);

#endif /* __DRV_GPIO_H__ */

