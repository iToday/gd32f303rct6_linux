/************************************************ 
* WKS Mini GD32开发板
* 电容触摸屏驱动-IIC通信部分	代码	   
* 版本：V1.0								  
************************************************/	

#include "ctiic.h"
#include "delay.h"	 


//控制I2C速度的延时
static void CT_IIC_delay(void)
{
	  delay_us(2);
} 

//电容触摸芯片IIC接口初始化
void CT_IIC_Init(void)
{	
    rcu_periph_clock_enable(RCU_GPIOC);  //GPIOC时钟使能
	
 	  //SCL引脚模式设置,推挽输出 
   	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	  //SDA引脚模式设置,开漏输出,这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平
	  gpio_init(GPIOC, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	
	  CT_IIC_Stop();     //停止总线上所有设备
}

//产生IIC起始信号
void CT_IIC_Start(void)
{
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    CT_IIC_delay();
    CT_IIC_SDA(0);     //START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 
    CT_IIC_delay();
    CT_IIC_SCL(0);     //钳住I2C总线，准备发送或接收数据 
    CT_IIC_delay();
}	  

//产生IIC停止信号
void CT_IIC_Stop(void)
{
    CT_IIC_SDA(0);     //STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号
    CT_IIC_delay();
    CT_IIC_SCL(1);
    CT_IIC_delay();
    CT_IIC_SDA(1);     //发送I2C总线结束信号 
    CT_IIC_delay();				   
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t CT_IIC_Wait_Ack(void)
{
		uint8_t waittime = 0;
		uint8_t rack = 0;

		CT_IIC_SDA(1);       //主机释放SDA线(此时外部器件可以拉低SDA线)
		CT_IIC_delay();	    
		CT_IIC_SCL(1);       //SCL=1, 此时从机可以返回ACK    
		CT_IIC_delay();	    
	
		while(CT_READ_SDA)       //等待应答 
		{
			waittime++;
			if(waittime>250)
			{
				CT_IIC_Stop();
				rack = 1;
				break;      //没有收到应答信号
			}
			CT_IIC_delay();	
		}
	
		CT_IIC_SCL(0);     //SCL=0, 结束ACK检查  
		CT_IIC_delay();	
		return rack;  
} 

//产生ACK应答
void CT_IIC_Ack(void)
{
    CT_IIC_SDA(0);     //SCL 0 -> 1  时 SDA = 0,表示应答 
    CT_IIC_delay();
    CT_IIC_SCL(1);     //产生一个时钟 
    CT_IIC_delay();
    CT_IIC_SCL(0);
    CT_IIC_delay();
    CT_IIC_SDA(1);     //主机释放SDA线 
    CT_IIC_delay();
}

//不产生ACK应答		    
void CT_IIC_NAck(void)
{
    CT_IIC_SDA(1);     //SCL 0 -> 1  时 SDA = 1,表示不应答 
    CT_IIC_delay();
    CT_IIC_SCL(1);     //产生一个时钟 
    CT_IIC_delay();
    CT_IIC_SCL(0);
    CT_IIC_delay();
}			

//IIC发送一个字节
//data: 要发送的数据	   
void CT_IIC_Send_Byte(uint8_t data)
{                        
    uint8_t t;   
	
    for(t=0;t<8;t++)
    {              
       CT_IIC_SDA((data & 0x80) >> 7);    //高位先发送 
       CT_IIC_delay();
       CT_IIC_SCL(1);
       CT_IIC_delay();
       CT_IIC_SCL(0);
       data <<= 1;      //左移1位,用于下一位发送 
    }	 
		CT_IIC_SDA(1);      //发送完成, 主机释放SDA线 
} 	   

//IIC读取一个字节
//ack:ack=1时，发送ACK，ack=0，发送nACK   
//返回值:接收到的数据 
uint8_t CT_IIC_Read_Byte(uint8_t ack)
{
	  uint8_t i,receive=0;
	
    for(i=0;i<8;i++ )     //接收1个字节数据 
	  {
        receive <<= 1;    //高位先输出,所以先收到的数据位要左移
        CT_IIC_SCL(1);
        CT_IIC_delay();

        if (CT_READ_SDA)
        {
            receive++;
        }
        
        CT_IIC_SCL(0);
        CT_IIC_delay();
    }					 
    if (!ack)
        CT_IIC_NAck();    //发送nACK
    else
        CT_IIC_Ack();     //发送ACK   
		
    return receive;       //返回读到的数据
}




























