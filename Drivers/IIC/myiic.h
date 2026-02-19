/************************************************ 
* WKS Mini GD32开发板
* IIC驱动 代码		   
* 版本：V1.0								  
************************************************/	

#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"


//IO操作
#define IIC_SCL(x)          gpio_pin_set(GPIOC, GPIO_PIN_12, x)    //SCL
#define IIC_SDA(x)          gpio_pin_set(GPIOC, GPIO_PIN_11, x)    //SDA
#define IIC_READ_SDA        gpio_input_bit_get(GPIOC,GPIO_PIN_11)  //读取SDA

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				        //发送IIC开始信号
void IIC_Stop(void);	  			      //发送IIC停止信号
void IIC_Ack(void);					        //IIC发送ACK信号
void IIC_NAck(void);				        //IIC不发送ACK信号
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Send_Byte(uint8_t data);		//IIC发送一个字节
uint8_t IIC_Read_Byte(uint8_t ack); //IIC读取一个字节
 
#endif

