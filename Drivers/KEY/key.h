/************************************************ 
* WKS Mini GD32开发板
* 按键输入 驱动代码	   
* 版本：V1.0								  
************************************************/	

#ifndef _KEY_H
#define _KEY_H
#include "sys.h"


#define KEY0        gpio_input_bit_get(GPIOA,GPIO_PIN_13)  //读取KEY0引脚
#define KEY1        gpio_input_bit_get(GPIOA,GPIO_PIN_15)  //读取KEY1引脚
#define WK_UP       gpio_input_bit_get(GPIOA,GPIO_PIN_0)   //读取WKUP引脚


#define KEY0_PRES 	1             //KEY0按下
#define KEY1_PRES	  2             //KEY1按下
#define WKUP_PRES   3             //KEY_UP按下(即WK_UP)

void KEY_Init(void);              //按键初始化函数
uint8_t KEY_Scan(uint8_t mode);   //按键扫描函数
#endif
