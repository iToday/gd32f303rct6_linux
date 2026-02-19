/************************************************ 
* WKS Mini GD32开发板
* 24CXX 驱动代码		   
* 版本：V1.0								  
************************************************/	

#include "myiic.h"
#include "24cxx.h"
#include "delay.h"


//初始化IIC接口
void AT24CXX_Init(void)
{
	  IIC_Init();     //IIC的IO口初始化
}

//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{				  
	  uint8_t temp=0;		  	    																 
    IIC_Start();           //产生起始信号
	
	  /* 根据不同的24CXX型号, 发送高位地址
     * 1, 24C16以上的型号, 分2个字节发送地址
     * 2, 24C16及以下的型号, 分1个低字节地址 + 占用器件地址的bit1~bit3位 用于表示高位地址, 最多11位地址
     *    对于24C01/02, 其器件地址格式(8bit)为: 1  0  1  0  A2  A1  A0  R/W
     *    对于24C04,    其器件地址格式(8bit)为: 1  0  1  0  A2  A1  a8  R/W
     *    对于24C08,    其器件地址格式(8bit)为: 1  0  1  0  A2  a9  a8  R/W
     *    对于24C16,    其器件地址格式(8bit)为: 1  0  1  0  a10 a9  a8  R/W
     *    R/W      : 读/写控制位 0,表示写; 1,表示读;
     *    A0/A1/A2 : 对应器件的1,2,3引脚(只有24C01/02/04/8有这些脚)
     *    a8/a9/a10: 对应存储整列的高位地址, 11bit地址最多可以表示2048个位置,可以寻址24C16及以内的型号
     */  
		if(EE_TYPE>AT24C16)             //24C16以上的型号, 分2个字节发送地址 
		{
				IIC_Send_Byte(0XA0);	      //发送写命令, IIC规定最低位是0, 表示写入
				IIC_Wait_Ack();             //每次发送完一个字节,都要等待ACK
				IIC_Send_Byte(ReadAddr>>8); //发送高字节地址	    
		}else 
		{
		    IIC_Send_Byte(0XA0 + ((ReadAddr >> 8) << 1));   //发送器件 0XA0 + 高位a8/a9/a10地址,写数据 	   
		}
		
		IIC_Wait_Ack();                //每次发送完一个字节,都要等待ACK
		IIC_Send_Byte(ReadAddr%256);   //发送低位地址
		IIC_Wait_Ack();	               //等待ACK, 此时地址发送完成了 
		
		IIC_Start();  	 	             //重新发送起始信号
		IIC_Send_Byte(0XA1);           //进入接收模式, IIC规定最低位是0, 表示读取		   
		IIC_Wait_Ack();	               //每次发送完一个字节,都要等待ACK
		temp=IIC_Read_Byte(0);		     //读数据，发送nACK
		IIC_Stop();                    //产生一个停止条件	    
		return temp;
}

//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{				   	  	    																 
    IIC_Start();                   //产生起始信号
	
		if(EE_TYPE>AT24C16)            //24C16以上的型号, 分2个字节发送地址   
		{
			IIC_Send_Byte(0XA0);	       //发送写命令, IIC规定最低位是0, 表示写入
			IIC_Wait_Ack();              //每次发送完一个字节,都要等待ACK
			IIC_Send_Byte(WriteAddr>>8); //发送高字节地址	  
		}else 
		{
			IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件 0XA0 + 高位a8/a9/a10地址,写数据 	 
		}
		
	  IIC_Wait_Ack();                //每次发送完一个字节,都要等待ACK
		IIC_Send_Byte(WriteAddr%256);  //发送低位地址
		IIC_Wait_Ack();	               //等待ACK, 此时地址发送完成了 
		
		/* 因为写数据的时候,不需要进入接收模式了,所以这里不用重新发送起始信号了 */
		IIC_Send_Byte(DataToWrite);     //发送字节							   
		IIC_Wait_Ack();                 //等待ACK 		    	   
		IIC_Stop();                     //产生一个停止条件 
		delay_ms(10);	                  //注意:EEPROM 写入比较慢,每写一次要等待一段时间，否则写失败！
}

//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{  	
		uint8_t t;
		for(t=0;t<Len;t++)
		{
			AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
		}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{  	
		uint8_t t;
		uint32_t temp=0;
		for(t=0;t<Len;t++)
		{
			temp<<=8;
			temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
		}
		return temp;												    
}

//检查AT24CXX是否正常
//检测原理: 在器件的末地址写入0X55, 然后再读取, 如果读取值为0X55
//则表示检测正常. 否则,则表示检测失败.
//返回0:检测成功
//返回1:检测失败
uint8_t AT24CXX_Check(void)
{
	uint8_t temp;
	uint16_t addr = EE_TYPE;
	temp=AT24CXX_ReadOneByte(addr); //避免每次开机都写AT24CXX			   
	
	if(temp==0X55)      //读取数据正常
	{
		return 0;
	}		
	else   //排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(addr,0X55);   //先写入数据
	  temp=AT24CXX_ReadOneByte(addr);	   //再读取数据
		
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  

//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
