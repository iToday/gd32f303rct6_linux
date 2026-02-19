/************************************************ 
* WKS Mini GD32开发板
* IIC驱动 代码		   
* 版本：V1.0								  
************************************************/	

#include "myiic.h"
#include "delay.h"


//初始化IIC
void IIC_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOC);  //GPIOC时钟使能
    
	  //SCL引脚模式设置,推挽输出 
   	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	  //SDA引脚模式设置,开漏输出,这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平
	  gpio_init(GPIOC, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    
    IIC_Stop();     //停止总线上所有设备   
}


//IIC延时函数,用于控制IIC读写速度
static void IIC_delay(void)
{
    delay_us(2);    //2us的延时
}

//产生IIC起始信号
void IIC_Start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    IIC_delay();
    IIC_SDA(0);     //START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 
    IIC_delay();
    IIC_SCL(0);     //钳住I2C总线，准备发送或接收数据 
    IIC_delay();
}	  

//产生IIC停止信号
void IIC_Stop(void)
{
    IIC_SDA(0);     //STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号
    IIC_delay();
    IIC_SCL(1);
    IIC_delay();
    IIC_SDA(1);     //发送I2C总线结束信号 
    IIC_delay();				   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
		uint8_t waittime = 0;
		uint8_t rack = 0;

		IIC_SDA(1);       //主机释放SDA线(此时外部器件可以拉低SDA线)
		IIC_delay();	    
		IIC_SCL(1);       //SCL=1, 此时从机可以返回ACK    
		IIC_delay();	    
	
		while(IIC_READ_SDA)       //等待应答 
		{
			waittime++;
			if(waittime>250)
			{
				IIC_Stop();
				rack = 1;
				break;      //没有收到应答信号
			}
		}
	
		IIC_SCL(0);     //SCL=0, 结束ACK检查  
		IIC_delay();	
		return rack;  
} 

//产生ACK应答
void IIC_Ack(void)
{
    IIC_SDA(0);     //SCL 0 -> 1  时 SDA = 0,表示应答 
    IIC_delay();
    IIC_SCL(1);     //产生一个时钟 
    IIC_delay();
    IIC_SCL(0);
    IIC_delay();
    IIC_SDA(1);     //主机释放SDA线 
    IIC_delay();
}

//不产生ACK应答		    
void IIC_NAck(void)
{
    IIC_SDA(1);     //SCL 0 -> 1  时 SDA = 1,表示不应答 
    IIC_delay();
    IIC_SCL(1);     //产生一个时钟 
    IIC_delay();
    IIC_SCL(0);
    IIC_delay();
}		

//IIC发送一个字节
//data: 要发送的数据	  
void IIC_Send_Byte(uint8_t data)
{                        
    uint8_t t;   
	
    for(t=0;t<8;t++)
    {              
       IIC_SDA((data & 0x80) >> 7);    //高位先发送 
       IIC_delay();
       IIC_SCL(1);
       IIC_delay();
       IIC_SCL(0);
       data <<= 1;      //左移1位,用于下一位发送 
    }	 
		IIC_SDA(1);         //发送完成, 主机释放SDA线 
} 	    

//IIC读取一个字节
//ack:ack=1时，发送ACK，ack=0，发送nACK   
//返回值:接收到的数据
uint8_t IIC_Read_Byte(uint8_t ack)
{
	  uint8_t i,receive=0;
	
    for(i=0;i<8;i++ )     //接收1个字节数据 
	  {
        receive <<= 1;    //高位先输出,所以先收到的数据位要左移
        IIC_SCL(1);
        IIC_delay();

        if (IIC_READ_SDA)
        {
            receive++;
        }
        
        IIC_SCL(0);
        IIC_delay();
    }					 
    if (!ack)
        IIC_NAck();       //发送nACK
    else
        IIC_Ack();        //发送ACK   
		
    return receive;       //返回读到的数据
}


