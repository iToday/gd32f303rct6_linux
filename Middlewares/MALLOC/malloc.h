/************************************************ 
* WKS Mini GD32开发板
* 内存管理 驱动代码	   
* 版本：V1.0								  
************************************************/	

#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h"


//如果没有定义NULL, 定义NULL 
#ifndef NULL
#define NULL 0
#endif

//内存参数设定.
#define MEM_BLOCK_SIZE		   	32  	  						              //内存块大小为32字节
#define MEM_MAX_SIZE			    5*1024  						              //最大管理内存 40K，GD32F303RC内部SRAM总共48KB
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	    //内存表大小
 
		 
//内存管理控制器
struct _m_mallco_dev
{
		void (*init)(void);				  //初始化
		uint8_t  (*perused)(void);	//内存使用率
		uint8_t 	*membase;					//内存池 
		uint16_t  *memmap; 					//内存管理状态表
		uint8_t   memrdy; 					//内存管理是否就绪
};

extern struct _m_mallco_dev mallco_dev;	//在mallco.c里面定义

void mem_init(void);					                   //内存管理初始化函数(外/内部调用)
uint8_t mem_perused(void);					             //获得内存使用率(外/内部调用) 
void mymemset(void *s,uint8_t c,uint32_t count); //设置内存
void mymemcpy(void *des,void *src,uint32_t n);   //复制内存     

void myfree(void *ptr);  				            //内存释放(外部调用)
void *mymalloc(uint32_t size) ;				      //内存分配(外部调用)
void *myrealloc(void *ptr,uint32_t size) ;	//重新分配内存(外部调用)
#endif













