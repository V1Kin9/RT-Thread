/*
 * File      : platform.h
 *
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-20	  ViKing       first implementation
 */

#ifndef _RISCV_PLATFORM_H
#define _RISCV_PLATFORM_H

// Some things missing from the official encoding.h
#define MCAUSE_INT         0x80000000
#define MCAUSE_CAUSE       0x7FFFFFFF

#include "clint.h"
#include "plic.h"

#include "const.h"

//#include "/gpio.h"
#include "uart.h"
#include "vga.h"
#include "can.h"

/****************************************************************************
 * Platform definitions
 *****************************************************************************/

// Memory map
#define CLINT_CTRL_ADDR _AC(0x02000000,UL)
#define PLIC_CTRL_ADDR _AC(0x0C000000,UL)
#define GPIO_CTRL_ADDR _AC(0x60000000,UL)
#define UART0_CTRL_ADDR _AC(0x60000010,UL)
#define VGA_CTRL_ADDR _AC(0x60003000,UL)
#define LS1C_REG_BASE_CAN0	_AC(0x60000040,UL)


// Interrupt numbers
#define INT_RESERVED 0
#define INT_UART0_BASE 1
#define LS1C_CAN0_IRQ 5
#define INT_VGA 9

// Helper functions
#define _REG8(p, i) (*(volatile uint8_t *) ((p) + (i)))
#define _REG32(p, i) (*(volatile uint32_t *) ((p) + (i)))
#define _REG32P(p, i) ((volatile uint32_t *) ((p) + (i)))
#define CLINT_REG(offset) _REG32(CLINT_CTRL_ADDR, offset)
#define GPIO_REG(offset) _REG32(GPIO_CTRL_ADDR, offset)
#define PLIC_REG(offset) _REG32(PLIC_CTRL_ADDR, offset)
#define UART0_REG(offset) _REG8(UART0_CTRL_ADDR, offset)
#define VGA_REG(offset)   _REG32(VGA_CTRL_ADDR,offset)

// Misc

#include <stdint.h>


#define PLIC_NUM_INTERRUPTS 16
#define PLIC_NUM_PRIORITIES 7

#define RTC_FREQ 48000


#endif /* _RISCV_PLATFORM_H */
