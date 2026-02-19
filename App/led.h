/************************************************
* WKS Mini GD32������
* LED��������
* �汾��V1.0
************************************************/

#ifndef _LED_H
#define _LED_H
#include "sys.h"

/* LED�˿ڶ��� */
#define LED0(x)   do{ x ? \
                      gpio_bit_write(GPIOA, GPIO_PIN_8, SET) : \
                      gpio_bit_write(GPIOA, GPIO_PIN_8, RESET); \
                  }while(0)

#define LED1(x)   do{ x ? \
                      gpio_bit_write(GPIOD, GPIO_PIN_2, SET) : \
                      gpio_bit_write(GPIOD, GPIO_PIN_2, RESET); \
                  }while(0)

/* LEDȡ������ */
#define LED0_TOGGLE()   do{ gpio_togglepin(GPIOA, GPIO_PIN_8); }while(0)        /* ��תLED0 */
#define LED1_TOGGLE()   do{ gpio_togglepin(GPIOD, GPIO_PIN_2); }while(0)        /* ��תLED1 */

void LED_Init(void);                                      //��ʼ��LED
void gpio_togglepin(uint32_t gpio_periph, uint32_t pin);  //��תIO��״̬
#endif
