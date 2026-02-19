/************************************************ 
* WKS Mini GD32开发板
* LED驱动代码	   
* 版本：V1.0								  
************************************************/	

#ifndef _LED_H
#define _LED_H
#include "sys.h"

/* LED端口定义 */
#define LED0(x)   do{ x ? \
                      gpio_bit_write(GPIOA, GPIO_PIN_8, SET) : \
                      gpio_bit_write(GPIOA, GPIO_PIN_8, RESET); \
                  }while(0)      

#define LED1(x)   do{ x ? \
                      gpio_bit_write(GPIOD, GPIO_PIN_2, SET) : \
                      gpio_bit_write(GPIOD, GPIO_PIN_2, RESET); \
                  }while(0)     

/* LED取反定义 */
#define LED0_TOGGLE()   do{ gpio_togglepin(GPIOA, GPIO_PIN_8); }while(0)        /* 翻转LED0 */
#define LED1_TOGGLE()   do{ gpio_togglepin(GPIOD, GPIO_PIN_2); }while(0)        /* 翻转LED1 */

void LED_Init(void);                                      //初始化LED
void gpio_togglepin(uint32_t gpio_periph, uint32_t pin);  //翻转IO口状态
#endif
