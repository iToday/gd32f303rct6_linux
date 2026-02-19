#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//串口0初始化
////////////////////////////////////////////////////////////////////////////////// 
//如果想串口中断接收，请不要注释以下宏定义
#define USART_REC_LEN               200         /* 定义最大接收字节数 200 */
#define USART_EN_RX                 1           /* 使能（1）/禁止（0）串口0接收 */

extern uint8_t  USART_RX_BUF[USART_REC_LEN];    /*接收缓冲,最大USART_REC_LEN个字节.末字节为换行符*/ 
extern uint16_t USART_RX_STA;         		      /*接收状态标记*/	

void usart_init(uint32_t bound);                /* 串口初始化函数 */

#endif


