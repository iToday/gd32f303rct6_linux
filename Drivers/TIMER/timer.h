/************************************************ 
* WKS Mini GD32¿ª·¢°å
* ¶¨Ê±Æ÷ÖĞ¶Ï Çı¶¯´úÂë	   
* °æ±¾£ºV1.0								  
************************************************/	

#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"


/* å®šæ—¶å™¨ä¸­æ–­æ ‡å¿—ä½ - ç”¨äºåœ¨ä¸­æ–­å’Œä»»åŠ¡é—´ä¼ é€’ä¿¡å· */
extern volatile uint8_t timer2_interrupt_flag;


void TIM2_Int_Init(uint16_t arr,uint16_t psc);  //Í¨ÓÃ¶¨Ê±Æ÷2ÖĞ¶Ï³õÊ¼»¯
#endif

