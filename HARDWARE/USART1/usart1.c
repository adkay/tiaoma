#include "sys.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "delay.h"
#include "string.h"
#include "stdarg.h"	 	 
#include "stdio.h"	
#include "24cxx.h"
#include "myiic.h"

u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 
//u8 text_x[200][50]={0};
u16 USART1_RX_STA=0;       //����״̬���	  
u8 numid[9]={0};
u8 numid1[9]={0};
u8 u1_count=0;
int nun=0;
u8 ok_count=6;
u8 xxx[9]={0xEE,0x20,0x00,0x28,0x00,0x81,0x00,0x05};
u8 Sys_Status=0x0;   // 0:�������ӱ�־λ  1�� ��ӡ��ʼ  2����ӡ�ɹ�
void u1_Send(u8 P)
	{
	
			USART1->DR=P;
			while((USART1->SR&0X40)==0);
		
	}
		
void u1_SendStr(u8 buf[],u8 len)
	{
		u8 i;
		for(i=0;i<len;i++)
		{
			USART1->DR=buf[i];
			while((USART1->SR&0X40)==0);
		}
	}
	void u1_printf(char* fmt,...)  
{  
	
	
	u8 i,len;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	len = strlen((const char*)USART1_TX_BUF);
		
		for(i=0;i<len;i++)
		{
			USART1->DR=USART1_TX_BUF[i];
			while((USART1->SR&0X40)==0);
		}
}
void u1_main(void)
{
	int len,i,k=0;
	u8 y,n,d,h,m,s;
	u8 count[9]={0};
	if(USART1_RX_STA&0x8000)
	{
		len=USART1_RX_STA&0x3FFF;
		if(USART1_RX_BUF[1]==0x32)
		{
			if(USART1_RX_BUF[3]==0x31)  //~0201  ��ȡ �������Ź����Ϣ
			{
					if(USART1_RX_BUF[4]==0x2A) //
					{
						u3_sendStr(xxx,8);
						for(i=0;i<len-5;i++)u3_send(USART1_RX_BUF[i+4]);
						u3_send(0xFF);
						u3_send(0xFC);
						u3_send(0xFF);
						u3_send(0xFF);
					}
			}
			else if(USART1_RX_BUF[3]==0x32)   //~0202 ��ȡĳ��ID����ϸ����
			{
				                   //�ֽ���߹���
				for(i=0;i<len;i++)
				{
					if(USART1_RX_BUF[i]==0x2A)
					{
						count[k]=i;
						k++;
					}
				}
				for(i=0;i<5;i++)id[i]=0;
				for(i=0;i<19;i++)pihao[i]=0;
				for(i=0;i<19;i++)guige[i]=0;
				for(i=0;i<9;i++)shase[i]=0;
				for(i=0;i<9;i++)guanse[i]=0;
				for(i=0;i<9;i++)pinming[i]=0;
				for(i=0;i<9;i++)dengji[i]=0;
				
				AT24CXX_WriteOneByte(0,(count[1]-count[0]-1));
				for(i=0;i<(count[1]-count[0]-1);i++)
				{
					id[i]=USART1_RX_BUF[i+count[0]+1];
					AT24CXX_WriteOneByte(1+i,USART1_RX_BUF[i+count[0]+1]);
				}
				AT24CXX_WriteOneByte(40,(count[2]-count[1]-1));
				for(i=0;i<(count[2]-count[1]-1);i++)
				{
					pihao[i]=USART1_RX_BUF[i+count[1]+1];
					AT24CXX_WriteOneByte(41+i,USART1_RX_BUF[i+count[1]+1]);
				}
				AT24CXX_WriteOneByte(80,(count[3]-count[2]-1));
				for(i=0;i<(count[3]-count[2]-1);i++)
				{
					guige[i]=USART1_RX_BUF[i+count[2]+1];
					AT24CXX_WriteOneByte(81+i,USART1_RX_BUF[i+count[2]+1]);
				}
				AT24CXX_WriteOneByte(120,(count[4]-count[3]-1));
				for(i=0;i<(count[4]-count[3]-1);i++)
				{
					shase[i]=USART1_RX_BUF[i+count[3]+1];
					AT24CXX_WriteOneByte(121+i,USART1_RX_BUF[i+count[3]+1]);
				}
				AT24CXX_WriteOneByte(160,(count[5]-count[4]-1));
				for(i=0;i<(count[5]-count[4]-1);i++)
				{
					guanse[i]=USART1_RX_BUF[i+count[4]+1];
					AT24CXX_WriteOneByte(161+i,USART1_RX_BUF[i+count[4]+1]);
				}
				AT24CXX_WriteOneByte(200,(count[6]-count[5]-1));
				for(i=0;i<(count[6]-count[5]-1);i++)
				{
					pinming[i]=USART1_RX_BUF[i+count[5]+1];
					AT24CXX_WriteOneByte(201+i,USART1_RX_BUF[i+count[5]+1]);
				}
				AT24CXX_WriteOneByte(240,(count[7]-count[6]-1));
				for(i=0;i<(count[7]-count[6]-1);i++)
				{
					dengji[i]=USART1_RX_BUF[i+count[6]+1];
					AT24CXX_WriteOneByte(241+i,USART1_RX_BUF[i+count[6]+1]);
				}
				
			}
		}
		else if(USART1_RX_BUF[1]==0x33)
		{
			if(USART1_RX_BUF[3]==0x32)    //~0302  ��ȡ���ص�ID
			{
					for(i=0;i<len-5;i++)
						{
							numid[i]=USART1_RX_BUF[i+4];
						}
					u1_count=1;
					//u2_printf("%s",USART1_RX_BUF);
			}
			else if(USART1_RX_BUF[3]==0x31)    //~0301   ��ȡ���ص�ID
			{
// 				for(i=0;i<len-5;i++)
// 				{
// 					numid1[i]=USART1_RX_BUF[i+4];
// 					//nun=(int)(numid1);

// 				}
			}
			else if(USART1_RX_BUF[3]==0x33)    //~0303  �������һ����¼
			{
				u3_send(0xEE);
				u3_send(0xB1);
				u3_send(0x10);
				u3_send(0x00);
				u3_send(0x00);
				u3_send(0x00);
				u3_send(0x21);
				for(i=0;i<len-5;i++)
				{
						u3_send(USART1_RX_BUF[i+4]);
				}
				//u3_printf("\r\nɾ���ɹ�");
				u3_send(0xFF);
				u3_send(0xFC);
				u3_send(0xFF);
				u3_send(0xFF);
				
			}

		}
		else if(USART1_RX_BUF[1]==0x31)
		{
			if(USART1_RX_BUF[3]==0x31)  //~0101   ��ȡʱ��
			{
				//����ʱ��
				riqi[0]=USART1_RX_BUF[5];
				riqi[1]=USART1_RX_BUF[6];
				riqi[3]=USART1_RX_BUF[7];
				riqi[4]=USART1_RX_BUF[8];
				riqi[6]=USART1_RX_BUF[9];
				riqi[7]=USART1_RX_BUF[10];
				y=((USART1_RX_BUF[5]-0x30)<<4)+(USART1_RX_BUF[6]-0x30);
				n=((USART1_RX_BUF[7]-0x30)<<4)+(USART1_RX_BUF[8]-0x30);
				d=((USART1_RX_BUF[9]-0x30)<<4)+(USART1_RX_BUF[10]-0x30);
				h=((USART1_RX_BUF[11]-0x30)<<4)+(USART1_RX_BUF[12]-0x30);
				m=((USART1_RX_BUF[13]-0x30)<<4)+(USART1_RX_BUF[14]-0x30);
				s=((USART1_RX_BUF[15]-0x30)<<4)+(USART1_RX_BUF[16]-0x30);
				u3_send(0xEE);
				u3_send(0x81);
				u3_send(s);
				u3_send(m);
				u3_send(h);
				u3_send(d);
				u3_send(0x00);
				u3_send(n);
				u3_send(y);
				u3_send(0xFF);
				u3_send(0xFC);
				u3_send(0xFF);
				u3_send(0xFF);
				
			}
			else if(USART1_RX_BUF[3]==0x32)   //~0102   ������ͨ�� ����Ϸ���
			{
				u8 x=0,y=0;   //~0102*12*23#
				u8 a[3]={0};
				ok_count=0;
				for(i=0;i<len;i++)
				{
					if(USART1_RX_BUF[i]==0x2A)
					{
						a[y]=i;
						y++;
					}
				}
				u3_send(0xEE);
				u3_send(0xB1);
				u3_send(0x10);
				u3_send(0x00);
				u3_send(0x00);
				u3_send(0x00);
				u3_send(0x28);
				for(i=0;i<(a[1]-a[0]-1);i++)u3_send(USART1_RX_BUF[i+a[0]+1]);
				u3_send(0xFF);
				u3_send(0xFC);
				u3_send(0xFF);
				u3_send(0xFF);
				
				u3_send(0xEE);
				u3_send(0xB1);
				u3_send(0x10);
				u3_send(0x00);
				u3_send(0x00);
				u3_send(0x00);
				u3_send(0x2A);
				for(i=0;i<(len-a[1]-2);i++)u3_send(USART1_RX_BUF[i+a[1]+1]);
				u3_send(0xFF);
				u3_send(0xFC);
				u3_send(0xFF);
				u3_send(0xFF);
			}
		}
		
		USART1_RX_STA=0;
		 
	}
	
}

void takenum(u8 buf[],u8 len)
{
	u8 i;

	
	
}
void TakeStrOut(u8 buf[],u16 len)
{

}
void USART1_IRQHandler(void)
{
	u8 res;	

	if(USART1->SR&(1<<5))//���յ�����
	{	 
		res=USART1->DR; 
		if((USART1_RX_STA&0x8000)==0)//����δ���
		{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
			{
				USART1_RX_BUF[USART1_RX_STA&0x3FFF]=res;
				USART1_RX_STA++;
				if(res==0x23)USART1_RX_STA|=0x8000;
				
			}else //��û�յ�0X0D
			{	
				if(res==0x7E)USART1_RX_STA|=0x4000;
					 
			}
		}  		 									     
	}

} 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��   PA9 TX  B  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 		 PA10 RX  8
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬����
		  
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ. 
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQChannel,2);//��2��������ȼ� 

}

