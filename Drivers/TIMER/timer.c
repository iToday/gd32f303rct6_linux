/************************************************ 
* WKS Mini GD32开发板
* 定时器中断 驱动代码	   
* 版本：V1.0								  
************************************************/	

#include "timer.h"
#include "led.h"
//#include "../../lvgl.h"

//通用定时器2中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//APB1时钟为60MHz，TIMER2时钟选择为APB1的2倍，因此，TIMER2时钟为120MHz
void TIM2_Int_Init(uint16_t arr,uint16_t psc)
{
	timer_parameter_struct timer_initpara;               //timer_initpara用于存放定时器的参数

  //使能RCU相关时钟 
  rcu_periph_clock_enable(RCU_TIMER2);                 //使能TIMER2的时钟

  //复位TIMER2
  timer_deinit(TIMER2);                                //复位TIMER2
  timer_struct_para_init(&timer_initpara);             //初始化timer_initpara为默认值

  //配置TIMER2
  timer_initpara.prescaler         = psc;              //设置预分频值
  timer_initpara.counterdirection  = TIMER_COUNTER_UP; //设置向上计数模式
  timer_initpara.period            = arr;              //设置自动重装载值
  timer_initpara.clockdivision     = TIMER_CKDIV_DIV1; //设置时钟分频因子
  timer_init(TIMER2, &timer_initpara);                 //根据参数初始化定时器

  //使能定时器及其中断
  timer_interrupt_enable(TIMER2, TIMER_INT_UP);        //使能定时器的更新中断
  nvic_irq_enable(TIMER2_IRQn, 1, 3);                  //配置NVIC设置优先级，抢占优先级1，响应优先级3
  timer_enable(TIMER2);                                //使能定时器TIMER2
}

//定时器2中断服务程序
void TIMER2_IRQHandler(void)
{
  if(timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP) == SET)   //判断定时器更新中断是否发生
  {
		LED1_TOGGLE();   //LED1翻转
    //lv_tick_inc(1);     		
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);         //清除定时器更新中断标志
  }
}














