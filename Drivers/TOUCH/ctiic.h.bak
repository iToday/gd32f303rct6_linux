/************************************************ 
* WKS Mini GD32开发板
* 电容触摸屏驱动-IIC通信部分	代码	   
* 版本：V1.0								  
************************************************/	

#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "sys.h"	    



//IO操作函数	 
#define CT_IIC_SCL(x)    gpio_pin_set(GPIOC, GPIO_PIN_0, x) 		  	//SCL     
#define CT_IIC_SDA(x)    gpio_pin_set(GPIOC, GPIO_PIN_3, x) 			  //SDA	 
#define CT_READ_SDA      gpio_input_bit_get(GPIOC,GPIO_PIN_3)  			//读取SDA 
 

//IIC所有操作函数
void CT_IIC_Init(void);                	//初始化IIC的IO口				 
void CT_IIC_Start(void);				        //发送IIC开始信号
void CT_IIC_Stop(void);	  				      //发送IIC停止信号
void CT_IIC_Ack(void);					        //IIC发送ACK信号
void CT_IIC_NAck(void);					        //IIC不发送ACK信号
uint8_t CT_IIC_Wait_Ack(void); 				  //IIC等待ACK信号
void CT_IIC_Send_Byte(uint8_t data);		//IIC发送一个字节
uint8_t CT_IIC_Read_Byte(uint8_t ack);	//IIC读取一个字节

#endif







