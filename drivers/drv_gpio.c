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

#include <rtdevice.h>

#include <encoding.h>
#include <platform.h>
#include "gpio.h"

#ifdef RT_USING_PIN

void ls_pin_mode(struct rt_device *device, rt_base_t pin, rt_base_t mode)
{
    unsigned int gpio = pin;

    if (PIN_MODE_OUTPUT == mode)
    {
        gpio_function(gpio, 1);
    }
    else
    {
        gpio_function(gpio, 0);
    }

    return ;
}

void ls_pin_write(struct rt_device *device, rt_base_t pin, rt_base_t value)
{
    unsigned int gpio = pin;

    if (PIN_LOW == value)
    {
        gpio_reset((gpio / 8), (gpio % 8));
    }
    else
    {
        gpio_set((gpio / 8), (gpio % 8));

    }

    return ;
}

int ls_pin_read(struct rt_device *device, rt_base_t pin)
{
    unsigned int gpio = pin;
    int value = PIN_LOW;

    if (0 == gpio_read_bit((gpio / 8), (gpio % 8)))
    {
        value = PIN_LOW;
    }
    else
    {
        value = PIN_HIGH;
    }

    return value;
}

rt_err_t ls_pin_attach_irq(struct rt_device *device, rt_int32_t pin,
                             rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    /* LS-Ext has no interrupt for gpio */
    return RT_EOK;
}

rt_err_t ls_pin_detach_irq(struct rt_device *device, rt_int32_t pin)
{
    return RT_EOK;
}

rt_err_t ls_pin_irq_enable(struct rt_device *device, rt_base_t pin, rt_uint32_t enabled)
{

    return RT_EOK;
}

const static struct rt_pin_ops _ls_pin_ops =
{
    ls_pin_mode,
    ls_pin_write,
    ls_pin_read,

    ls_pin_attach_irq,
    ls_pin_detach_irq,
    ls_pin_irq_enable,
};

int hw_pin_init(void)
{
    int ret = RT_EOK;

    ret = rt_device_pin_register("pin", &_ls_pin_ops, RT_NULL);

    return ret;
}
INIT_BOARD_EXPORT(hw_pin_init);

#endif /*RT_USING_PIN */

