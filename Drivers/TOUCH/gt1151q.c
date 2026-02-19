/************************************************ 
* WKS Mini GD32开发板
* 电容触摸屏-GT1151 驱动代码		   
* 版本：V1.0								  
************************************************/	

#include "gt1151q.h"
#include "touch.h"
#include "ctiic.h"
#include "usart.h"
#include "delay.h" 
#include "string.h" 
#include "lcd.h"

//向GT1151写入一次数据
//reg:起始寄存器地址
//buf:数据缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
uint8_t GT1151_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
		uint8_t i;
		uint8_t ret=0;
		CT_IIC_Start();	
		CT_IIC_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
		CT_IIC_Wait_Ack();
		CT_IIC_Send_Byte(reg>>8);   	  //发送高8位地址
		CT_IIC_Wait_Ack(); 	 										  		   
		CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
		CT_IIC_Wait_Ack();  
		for(i=0;i<len;i++)
		{	   
				CT_IIC_Send_Byte(buf[i]);  	//发数据
				ret=CT_IIC_Wait_Ack();
				if(ret)break;  
		}
		CT_IIC_Stop();					//产生一个停止条件	    
		return ret; 
}

//从GT1151读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void GT1151_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
		uint8_t i; 
		CT_IIC_Start();	
		CT_IIC_Send_Byte(GT_CMD_WR);   //发送写命令 	 
		CT_IIC_Wait_Ack();
		CT_IIC_Send_Byte(reg>>8);   	 //发送高8位地址
		CT_IIC_Wait_Ack(); 	 										  		   
		CT_IIC_Send_Byte(reg&0XFF);    //发送低8位地址
		CT_IIC_Wait_Ack();  
		CT_IIC_Start();  	 	   
		CT_IIC_Send_Byte(GT_CMD_RD);   //发送读命令		   
		CT_IIC_Wait_Ack();	   
		for(i=0;i<len;i++)
		{	   
				buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
		} 
		CT_IIC_Stop();  //产生一个停止条件  
} 

//初始化GT1151触摸屏
//返回值:0,初始化成功;1,初始化失败 
uint8_t GT1151_Init(void)
{
		uint8_t temp[5];  
	
	  rcu_periph_clock_enable(RCU_GPIOC);  //GPIOC时钟使能

	  gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13); //设置PC13推挽输出
	 
	  gpio_init(GPIOC, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_1); //设置PC1上拉输入
	
		CT_IIC_Init();      	//初始化电容屏的I2C总线  
		GT_RST(0);				    //复位
		delay_ms(50);
		GT_RST(1);				    //释放复位 
		delay_ms(50);
	
	  gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_1); //设置PC1浮空输入
		
		delay_ms(100);
		GT1151_RD_Reg(GT_PID_REG,temp,4); //读取产品ID
		temp[4]=0;
		printf("CTP ID:%s\r\n",temp);	    //打印ID
		if(strcmp((char*)temp,"1158")==0) //ID==1158
		 return 0;
    else
		 return 1;
}

const uint16_t GT1151_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.电容屏未用到该参数, 为了兼容电阻屏
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t GT1151_Scan(uint8_t mode)
{
		uint8_t buf[4];
		uint8_t i=0;
		uint8_t res=0;
		uint8_t temp;
		static uint8_t t=0;  //控制查询间隔,从而降低CPU占用率   
		t++;
		if((t%10)==0||t<10)  //空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
		{
			GT1151_RD_Reg(GT_GSTID_REG,&mode,1);  //读取触摸点的状态 
			
			if((mode&0XF)&&((mode&0XF)<6))
			{
				temp=0XFF<<(mode&0XF);  //将点的个数转换为1的位数,匹配tp_dev.sta定义 
				tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
				for(i=0;i<5;i++)
				{
					if(tp_dev.sta&(1<<i))		//查询有没有触摸按下
					{
						GT1151_RD_Reg(GT1151_TPX_TBL[i],buf,4);	//读取XY坐标值
						if(tp_dev.touchtype&0X01)  //横屏
						{
							tp_dev.x[i] = lcddev.width - (((uint16_t)buf[3] << 8) + buf[2]);
              tp_dev.y[i] = ((uint16_t)buf[1] << 8) + buf[0];        
						}else
						{
							tp_dev.x[i]=((uint16_t)buf[1]<<8)+buf[0];
							tp_dev.y[i]=((uint16_t)buf[3]<<8)+buf[2];
						}  
			//	printf("x[%d]:%d,y[%d]:%d\r\n", i, tp_dev.x[i], i, tp_dev.y[i]);  
					}			
				} 
				res=1;
				if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//读到的数据都是0,则忽略此次数据
				t=0;		//触发一次,则会最少连续监测10次,从而提高命中率
			}
			if(mode&0X80&&((mode&0XF)<6))
			{
				temp=0;
				GT1151_WR_Reg(GT_GSTID_REG,&temp,1); //清标志 		
			}
		}
		if((mode&0X8F)==0X80)//无触摸点按下
		{ 
			if(tp_dev.sta&TP_PRES_DOWN)	     //之前是被按下的
			{
				tp_dev.sta&=~TP_PRES_DOWN;	   //标记按键松开
			}else						    //之前就没有被按下
			{ 
				tp_dev.x[0]=0xffff;
				tp_dev.y[0]=0xffff;
				tp_dev.sta&=0XE0;	//清除点有效标记	
			}	 
		} 	
		if(t>240)t=10; //重新从10开始计数
		return res;
}
 



























