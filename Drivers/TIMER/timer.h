/************************************************ 
* WKS Mini GD32开发板
* 定时器中断 驱动代码	   
* 版本：V1.0								  
************************************************/	

#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"


/* 瀹氭椂鍣ㄤ腑鏂爣蹇椾綅 - 鐢ㄤ簬鍦ㄤ腑鏂拰浠诲姟闂翠紶閫掍俊鍙� */
extern volatile uint8_t timer2_interrupt_flag;


void TIM2_Int_Init(uint16_t arr,uint16_t psc);  //通用定时器2中断初始化
#endif

