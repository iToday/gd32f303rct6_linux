#include "sys.h"
#include "usart.h"


/* 如果使用os,则包括下面的头文件即可. */
#if SYS_SUPPORT_OS
#include "includes.h" /* os 使用 */
#endif

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");  /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* MDK下需要重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
	while(RESET == usart_flag_get(USART0, USART_FLAG_TC));       /* 等待上一个字符发送完成 */
	
	usart_data_transmit(USART0, (uint8_t)ch);                    /* 将要发送的字符 ch 写入到DR寄存器 */  
  return ch;
}
#endif
/******************************************************************************************/

#if USART_EN_RX /*如果使能了接收*/

/* 接收缓冲, 最大USART_REC_LEN个字节. */
uint8_t USART_RX_BUF[USART_REC_LEN];

/*  接收状态
 *  bit15，      接收完成标志
 *  bit14，      接收到0x0d
 *  bit13~0，    接收到的有效字节数目
*/
uint16_t USART_RX_STA = 0;



//串口0初始化函数
//bound: 波特率, 根据自己需要设置波特率值
void usart_init(uint32_t bound)
{
	//使能GPIO时钟和复用时钟
  rcu_periph_clock_enable(RCU_GPIOA);     //使能GPIOA时钟
  rcu_periph_clock_enable(RCU_AF);        //使能复用时钟
  rcu_periph_clock_enable(RCU_USART0);    //使能串口时钟

  //配置TX的GPIO
  gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

  //配置RX的GPIO
  gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

  //配置USART的参数
  usart_deinit(USART0);                                 //RCU配置恢复默认值
  usart_baudrate_set(USART0, bound);                    //设置波特率
  usart_stop_bit_set(USART0, USART_STB_1BIT);           //一个停止位
  usart_word_length_set(USART0, USART_WL_8BIT);         //字长为8位数据格式
  usart_parity_config(USART0, USART_PM_NONE);           //无奇偶校验位
  usart_receive_config(USART0, USART_RECEIVE_ENABLE);   //使能接收
  usart_transmit_config(USART0, USART_TRANSMIT_ENABLE); //使能发送
  usart_interrupt_enable(USART0, USART_INT_RBNE);       //使能接收缓冲区非空中断
  //配置NVIC，并设置中断优先级
  nvic_irq_enable(USART0_IRQn, 3, 3);                   //抢占优先级3，子优先级3

  //使能串口
  usart_enable(USART0);	
}

void USART0_IRQHandler(void)
{
	uint8_t Res;
#if SYSTEM_SUPPORT_OS 		                              //如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
		if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =usart_data_receive(USART0);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
#endif
