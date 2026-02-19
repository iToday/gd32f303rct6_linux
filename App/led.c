/************************************************ 
* WKS Mini GD32开发板
* LED驱动代码	   
* 版本：V1.0								  
************************************************/	

#include "led.h"


//LED IO初始化
void LED_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);  //GPIOA时钟使能
		rcu_periph_clock_enable(RCU_GPIOD);  //GPIOD时钟使能
	
		gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8); //设置PA8推挽输出
		gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2); //设置PD2推挽输出
	  
	  LED0(1);                             //关闭LED0
	  LED1(1);                             //关闭LED1
}
//翻转IO口状态
void gpio_togglepin(uint32_t gpio_periph, uint32_t pin)
{
  uint32_t octl;

  octl = GPIO_OCTL(gpio_periph);

  GPIO_BOP(gpio_periph) = ((octl & pin) << 16u) | (~octl & pin);
}
