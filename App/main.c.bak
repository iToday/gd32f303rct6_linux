/************************************************
* WKS Mini GD32开发板
* LVGL 实验
************************************************/
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "touch.h"
#include "timer.h"
#include "24cxx.h"
#include "malloc.h"	  
#include "lvgl.h"
#include "lv_port_indev_template.h"
#include "lv_port_disp_template.h"
#include "lvgl_demo.h"


int main(void)
{ 			  
	  delay_init(120);                  //初始化延时函数 
	  usart_init(115200);               //初始化串口
	  LED_Init();							          //初始化LED
    lcd_init();                       //初始化显示窗口
	  KEY_Init();                       //初始化按键
    tp_dev.init();				   	        //触摸屏初始化
	  mem_init();		                	  //初始化内存池

  	lvgl_demo();                      //运行例程  
}

