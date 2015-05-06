#include "delay.h"
#include "usart4.h"
#include "usart1.h"
#include "math.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	   
#include <stdlib.h>
//串口发送缓存区 	
__align(8) u8 USART4_TX_BUF[USART4_MAX_SEND_LEN];								//如果使能了接收   	  
//串口接收缓存区 	
u8 USART4_RX_BUF[USART4_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u16 USART4_RX_STA=0;
float weight=0;
float jinzhong=0;
void UART4_IRQHandler(void)
{
	u8 res;	    
	if(UART4->SR&(1<<5))//接收到数据
	{	 
		res=UART4->DR; 			 
		if((USART4_RX_STA&0x8000)==0)
		{
			if(USART4_RX_STA&0x4000)
			{
				if(res!=0x20)
				{
					if(res==0x6B)
					{
						USART4_RX_STA|=0X8000;
					}else
					{
					USART4_RX_BUF[USART4_RX_STA&0x3FFF]=res;
					USART4_RX_STA++;
					 if((USART4_RX_STA&0X3FFF)>8)USART4_RX_STA=0;
					}
				}
			}
			else
			{
				if(res==0x20)USART4_RX_STA|=0x4000;
			}
		}
		
	}  											 
}   

void u4_main(void)
{
	u8 i,j,k,a=0,b=0;
	u8 len;
	len=USART4_RX_STA&0x3FFF;
	if(USART4_RX_STA&0X8000)
	{
		//u4_printf("%s",USART4_RX_BUF);
		for(i=0;i<len;i++)
		{
			if(USART4_RX_BUF[i]==0x2E)break;
			a++;
		}
		b=len-a-1;
		switch (a)
		{
			case 1:weight=USART4_RX_BUF[0]-0x30;break;
			case 2:weight=(USART4_RX_BUF[1]-0x30)+(USART4_RX_BUF[0]-0x30)*10;break;
			case 3:weight=(USART4_RX_BUF[2]-0x30)+(USART4_RX_BUF[1]-0x30)*10+(USART4_RX_BUF[0]-0x30)*100;break;
			
		}
		switch (b)
		{
			case 1:
				weight=weight+(USART4_RX_BUF[a+1]-0x30)*0.1;
				break;
			case 2:
			{
				if((USART4_RX_BUF[a+2]-0x30)<5)
				{
					weight=weight+(USART4_RX_BUF[a+1]-0x30)*0.1;
				}else
				{
					weight=weight+(USART4_RX_BUF[a+1]-0x30)*0.1+0.1;
				}
				
				break;
			}
		}
		//u4_printf("%f",weight);
		USART4_RX_STA=0;
	}
}
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void uart4_Init(u32 pclk1,u32 bound)
{  	 		 

	RCC->APB2ENR|=1<<4;   	//使能PORTA口时钟  
	GPIOC->CRH&=0XFFFF00FF;	//IO状态设置
	GPIOC->CRH|=0X00008B00;	//IO状态设置	 
	RCC->APB1ENR|=1<<19;  	//使能串口时钟 	 
	RCC->APB1RSTR|=1<<19;   //复位串口2
	RCC->APB1RSTR&=~(1<<19);//停止复位	   	   
	
 	UART4->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
	UART4->CR1|=0X200C;  	//1位停止,无校验位.
	UART4->CR1|=1<<8;    	//PE中断使能
	UART4->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,UART4_IRQChannel,2);//组2，最低优先级 
	TIM4_Init(99,7199);		//10ms中断
	USART4_RX_STA=0;		//清零
	TIM4_Set(0);			//关闭定时器4
									  	
}
//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u4_printf(char* fmt,...)  
{  
	
	
	u8 i,len;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART4_TX_BUF,fmt,ap);
	va_end(ap);
	len = strlen((const char*)USART4_TX_BUF);
		
		for(i=0;i<len;i++)
		{
			UART4->DR=USART4_TX_BUF[i];
			while((UART4->SR&0X40)==0);
		}
}
//定时器4中断服务程序		    
void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//是更新中断
	{	 			   
		USART4_RX_STA|=1<<15;	//标记接收完成
		TIM4->SR&=~(1<<0);		//清除中断标志位		   
		TIM4_Set(0);			//关闭TIM4  
	}	    
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
    	TIM4->CNT=0;         //计数器清空
		TIM4->CR1|=1<<0;     //使能定时器4
	}else TIM4->CR1&=~(1<<0);//关闭定时器4	   
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
 	TIM4->ARR=arr;  	//设定计数器自动重装值   
	TIM4->PSC=psc;  	//预分频器
 	TIM4->DIER|=1<<0;   //允许更新中断				
 	TIM4->CR1|=0x01;  	//使能定时器4	  	   
	MY_NVIC_Init(1,3,TIM4_IRQChannel,2);//抢占2，子优先级3，组2	在2中优先级最低								 
} 
///////////////////////////////////////USART2 DMA发送配置部分//////////////////////////////////	   		    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址    
// void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
// {
//  	RCC->AHBENR|=1<<0;			//开启DMA1时钟
// 	delay_us(5);
// 	DMA_CHx->CPAR=cpar; 		//DMA1 外设地址 
// 	DMA_CHx->CMAR=cmar; 		//DMA1,存储器地址	 
// 	DMA_CHx->CCR=0X00000000;	//复位
// 	DMA_CHx->CCR|=1<<4;  		//从存储器读
// 	DMA_CHx->CCR|=0<<5;  		//普通模式
// 	DMA_CHx->CCR|=0<<6;  		//外设地址非增量模式
// 	DMA_CHx->CCR|=1<<7;  		//存储器增量模式
// 	DMA_CHx->CCR|=0<<8;  		//外设数据宽度为8位
// 	DMA_CHx->CCR|=0<<10; 		//存储器数据宽度8位
// 	DMA_CHx->CCR|=1<<12; 		//中等优先级
// 	DMA_CHx->CCR|=0<<14; 		//非存储器到存储器模式		  	
// } 
// //开启一次DMA传输
// void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len)
// {
// 	DMA_CHx->CCR&=~(1<<0);       //关闭DMA传输 
// 	DMA_CHx->CNDTR=len;          //DMA1,传输数据量 
// 	DMA_CHx->CCR|=1<<0;          //开启DMA传输
// }	   
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 									 



void USART4_SendStr(u16 buf[],u8 len)
	{
		u8 i;
		for(i=0;i<len;i++)
		{
			UART4->DR=buf[i];
			while((UART4->SR&0X40)==0);
		}
	}

void USART4_Send(u16 P)
	{
		
			UART4->DR=P;
			while((UART4->SR&0X40)==0);
		
	}


















