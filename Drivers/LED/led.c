/*
 * LED driver moved to Drivers/LED for proper driver layering.
 */

#include "led.h"


// LED IO 初始化
void LED_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);  //GPIOA 时钟使能
    rcu_periph_clock_enable(RCU_GPIOD);  //GPIOD 时钟使能

    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8); // 配置PA8为推挽输出
    gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2); // 配置PD2为推挽输出

    hal_led_set(0, 1);  // 关 LED0 (硬件为低有效时请调整)
    hal_led_set(1, 1);  // 关 LED1
}

// gpio_togglepin moved to HAL (Drivers/HAL/hal_gpio.c)
