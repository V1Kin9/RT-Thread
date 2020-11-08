/*******************************************************************
*
*   PROJECT:    W01
*
*   FILENAME:   gpio.h
*
*   FUNCTION:   define the interface to operate the GPIO device
*
*   AUTHOR:     liaoll
*
*   DATE:       2017/08/16
*
*   IS_FINISH:  NO
*
********************************************************************/

/************************************************************************
_________________________________________
|       |           |
|GPIO bits  |   GPIO class  |   
|----------------------------------------
|       |           |
|7-0        |   GPIOA       |
|----------------------------------------
|       |           |
|15-8       |   GPIOB       |
|----------------------------------------
|       |           |
|23-16      |   GPIOC       |
|----------------------------------------
|       |           |
|31-24      |   GPIOD       |
|----------------------------------------
*************************************************************************/

/* Define to prevent recursive inclusion --------------------------*/
#ifndef SDJ2102_GPIO_H
#define SDJ2102_GPIO_H

/* Exported type --------------------------------------------------*/
/* Exported constants ---------------------------------------------*/
/* Exported variables ---------------------------------------------*/
/* Exported macro -------------------------------------------------*/

/*device memory map*/
#define GPIO_CTRL_OFFSET    0x4
#define GPIO_DATA_OFFSET    0x0

/*gpio class and pin define */
#define GPIOA       0
#define GPIOB       8
#define GPIOC       16
#define GPIOD       24

#define GPIO1           0x00
#define GPIO2           0x01
#define GPIO3           0x02
#define GPIO4           0x03 

#define GPIO_PIN_0  0x1<<0
#define GPIO_PIN_1  0x1<<1
#define GPIO_PIN_2  0x1<<2
#define GPIO_PIN_3  0x1<<3
#define GPIO_PIN_4  0x1<<4
#define GPIO_PIN_5  0x1<<5
#define GPIO_PIN_6  0x1<<6
#define GPIO_PIN_7  0x1<<7


/* Exported functions ---------------------------------------------*/
void gpio_function(unsigned int gpio_class ,unsigned int function);
void gpio_set(unsigned int gpio_class,unsigned int gpio_pin);
void gpio_reset(unsigned int gpio_class,unsigned int gpio_pin);
unsigned int gpio_read_bit(unsigned int gpio_class,unsigned int gpio_pin); 
unsigned int gpio_read_byte(unsigned int gpio_class);
void gpio_write_byte(unsigned int gpio_class,unsigned int value);


#endif /*SDJ2102_GPIO_H*/
