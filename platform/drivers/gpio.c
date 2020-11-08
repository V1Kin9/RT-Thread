/*************************************************************************************
*
*   PROJECT:    SDJ2102
*
*   FILENAME:   gpio.c  
*
*   FUNCTION:   implement the interface of gpio
*
*   AUTHOR:     liaoll
*
*   DATE:       2017/08/15
*
*   ISFINISH:   NO
*
*   STATUS:     
        1.  2017/08/20 simulation success
	2.  2020/11/4  changed the gpio_function	ViKing
*************************************************************************************/
/* Includes --------------------------------------------------------*/
#include "platform.h"

/* Private define --------------------------------------------------*/
/* Private macro ---------------------------------------------------*/
/* Private variables -----------------------------------------------*/
/* Extern variables ------------------------------------------------*/
/* Private function prototypes -------------------------------------*/
/* Private functions -----------------------------------------------*/

/********************************************************************
* Function Name :   gpio_function
* Description   :   enable GPIO input/output function
* Input         :   gpio_pin: pin
*               :   function: 0: Input, 1: Output
* Output        :   none
* Return        :   none
********************************************************************/
void gpio_function(unsigned int gpio_pin , unsigned int function)
{
     if (function)
	GPIO_CTRL_REG(GPIO_CTRL_OFFSET) |= (1 << gpio_pin);
     else
	GPIO_CTRL_REG(GPIO_CTRL_OFFSET) &= ~(1 << gpio_pin);
}

/********************************************************************
* Function Name :   gpio_set
* Description   :   GPIO set bit
* Input         :   gpio_class: GPIOA or GPIOB or GPIOC or GPIOD
*                   gpio_pin: gpio_pin0 ~ gpio_pin7
* Output        :   none
* Return        :   none
********************************************************************/
void gpio_set(unsigned int gpio_class, unsigned int gpio_pin)
{
    GPIO_DATA_REG(GPIO_DATA_OFFSET + gpio_class) |= (1 << gpio_pin);
}


/********************************************************************
* Function Name :   gpio_reset
* Description   :   GPIO reset bit
* Input         :   gpio_class: GPIOA or GPIOB or GPIOC or GPIOD
*                   gpio_pin: gpio_pin0 ~ gpio_pin7
* Output        :   none
* Return        :   none
********************************************************************/
void gpio_reset(unsigned int gpio_class, unsigned int gpio_pin)
{
    GPIO_DATA_REG(GPIO_DATA_OFFSET + gpio_class) &= ~(1 << gpio_pin) ;
}

/********************************************************************
* Function Name :   gpio_read_bit
* Description   :   read GPIO pin
* Input         :   gpio_class: GPIOA or GPIOB or GPIOC or GPIOD
*                   gpio_pin: gpio_pin0 ~ gpio_pin7
* Output        :   none
* Return        :   gpio bit
********************************************************************/
unsigned int gpio_read_bit(unsigned int gpio_class,unsigned int gpio_pin)
{
    return (GPIO_DATA_REG(GPIO_DATA_OFFSET + gpio_class) & (1 << gpio_pin));
}

/********************************************************************
* Function Name :   gpio_read_byte
* Description   :   read a byte from the whole GPIO group
* Input         :   gpio_class: GPIOA or GPIOB or GPIOC or GPIOD
* Output        :   none
* Return        :   a byte from the whole GPIO group
********************************************************************/
unsigned int gpio_read_byte(unsigned int gpio_class)
{
    return GPIO_DATA_REG(GPIO_DATA_OFFSET + gpio_class);
}

/********************************************************************
* Function Name :   gpio_write_byte
* Description   :   write a byte to the whole GPIO group
* Input         :   gpio_class: GPIOA or GPIOB or GPIOC or GPIOD
*                   value: the byte you want to write
* Output        :   none
* Return        :   none
********************************************************************/
void gpio_write_byte(unsigned int gpio_class,unsigned int value)
{
    GPIO_DATA_REG((GPIO_DATA_OFFSET + (unsigned long)(gpio_class >> 3))) = 
         (GPIO_DATA_REG((GPIO_DATA_OFFSET + (unsigned long)(gpio_class >> 3))) & ~(0xff)) | (value) ;
}
 
