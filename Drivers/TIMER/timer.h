/************************************************ 
* WKS Mini GD32开发板
* 定时器中断 驱动代码	   
* 版本：V1.0								  
************************************************/	

#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"


void TIM2_Int_Init(uint16_t arr,uint16_t psc);  //通用定时器2中断初始化
#endif

