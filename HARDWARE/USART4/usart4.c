#include "delay.h"
#include "usart4.h"
#include "usart1.h"
#include "math.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	   
#include <stdlib.h>
//���ڷ��ͻ����� 	
__align(8) u8 USART4_TX_BUF[USART4_MAX_SEND_LEN];								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART4_RX_BUF[USART4_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u16 USART4_RX_STA=0;
float weight=0;
float jinzhong=0;
void UART4_IRQHandler(void)
{
	u8 res;	    
	if(UART4->SR&(1<<5))//���յ�����
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
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void uart4_Init(u32 pclk1,u32 bound)
{  	 		 

	RCC->APB2ENR|=1<<4;   	//ʹ��PORTA��ʱ��  
	GPIOC->CRH&=0XFFFF00FF;	//IO״̬����
	GPIOC->CRH|=0X00008B00;	//IO״̬����	 
	RCC->APB1ENR|=1<<19;  	//ʹ�ܴ���ʱ�� 	 
	RCC->APB1RSTR|=1<<19;   //��λ����2
	RCC->APB1RSTR&=~(1<<19);//ֹͣ��λ	   	   
	
 	UART4->BRR=(pclk1*1000000)/(bound);// ����������	 
	UART4->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	UART4->CR1|=1<<8;    	//PE�ж�ʹ��
	UART4->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,UART4_IRQChannel,2);//��2��������ȼ� 
	TIM4_Init(99,7199);		//10ms�ж�
	USART4_RX_STA=0;		//����
	TIM4_Set(0);			//�رն�ʱ��4
									  	
}
//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
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
//��ʱ��4�жϷ������		    
void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//�Ǹ����ж�
	{	 			   
		USART4_RX_STA|=1<<15;	//��ǽ������
		TIM4->SR&=~(1<<0);		//����жϱ�־λ		   
		TIM4_Set(0);			//�ر�TIM4  
	}	    
}
//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
    	TIM4->CNT=0;         //���������
		TIM4->CR1|=1<<0;     //ʹ�ܶ�ʱ��4
	}else TIM4->CR1&=~(1<<0);//�رն�ʱ��4	   
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM4_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ   
	TIM4->PSC=psc;  	//Ԥ��Ƶ��
 	TIM4->DIER|=1<<0;   //��������ж�				
 	TIM4->CR1|=0x01;  	//ʹ�ܶ�ʱ��4	  	   
	MY_NVIC_Init(1,3,TIM4_IRQChannel,2);//��ռ2�������ȼ�3����2	��2�����ȼ����								 
} 
///////////////////////////////////////USART2 DMA�������ò���//////////////////////////////////	   		    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ    
// void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
// {
//  	RCC->AHBENR|=1<<0;			//����DMA1ʱ��
// 	delay_us(5);
// 	DMA_CHx->CPAR=cpar; 		//DMA1 �����ַ 
// 	DMA_CHx->CMAR=cmar; 		//DMA1,�洢����ַ	 
// 	DMA_CHx->CCR=0X00000000;	//��λ
// 	DMA_CHx->CCR|=1<<4;  		//�Ӵ洢����
// 	DMA_CHx->CCR|=0<<5;  		//��ͨģʽ
// 	DMA_CHx->CCR|=0<<6;  		//�����ַ������ģʽ
// 	DMA_CHx->CCR|=1<<7;  		//�洢������ģʽ
// 	DMA_CHx->CCR|=0<<8;  		//�������ݿ��Ϊ8λ
// 	DMA_CHx->CCR|=0<<10; 		//�洢�����ݿ��8λ
// 	DMA_CHx->CCR|=1<<12; 		//�е����ȼ�
// 	DMA_CHx->CCR|=0<<14; 		//�Ǵ洢�����洢��ģʽ		  	
// } 
// //����һ��DMA����
// void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u8 len)
// {
// 	DMA_CHx->CCR&=~(1<<0);       //�ر�DMA���� 
// 	DMA_CHx->CNDTR=len;          //DMA1,���������� 
// 	DMA_CHx->CCR|=1<<0;          //����DMA����
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


















