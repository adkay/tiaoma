#include "sys.h"
#include "usart1.h"
#include "usart3.h"
#include "usart4.h"
#include "delay.h"
#include "math.h"
#include "usart2.h"
#include "timer.h"
#include "24cxx.h"
#include "myiic.h"
#include <stdlib.h>

#include "string.h"
#include "stdarg.h"	 	 
#include "stdio.h"	

u8 num_ok=0;
u8 USART3_RX_BUF[USART3_REC_LEN];
u16 USART3_RX_STA;
u8 datalast[5]={0xff,0xfc,0xff,0xff};
u8 USART3_TX_BUF[200];
u8 data2[6]={0x7E,0x30,0x33,0x30,0x32};
u8 data1[6]={0x7E,0x30,0x33,0x30,0x31};
u8 data3[]={0xEE,0x20,0x00,0x64,0x00,0x64,0x00,0x03,0xC9,0xCF,0xB4,0xAB,0xB3,0xC9,0xB9,0xA6,0xFF,0xFC,0xFF,0xFF};
u8 data4[]={0xEE,0x20,0x00,0x64,0x00,0x64,0x00,0x03,0xCD,0xF8,0xC2,0xE7,0xB4,0xED,0xCE,0xF3,0xFF,0xFC,0xFF,0xFF};
u8 chenggong[9]={0xC9,0xCF,0xB4,0xAB,0xB3,0xC9,0xB9,0xA6};
u8 erro0[19]={0xEE,0xB1,0x10,0x00,0x00,0x00,0x1C,0xCD,0xF8,0xC2,0xE7,0xB4,0xED,0xCE,0xF3,0xFF,0xFC,0xFF,0xFF};
u8 good0[19]={0xEE,0xB1,0x10,0x00,0x00,0x00,0x1C,0xC1,0xAC,0xBD,0xD3,0xB3,0xC9,0xB9,0xA6,0xFF,0xFC,0xFF,0xFF};
u8 erro2[19]={0xEE,0xB1,0x10,0x00,0x02,0x00,0x05,0xCD,0xF8,0xC2,0xE7,0xB4,0xED,0xCE,0xF3,0xFF,0xFC,0xFF,0xFF};
u8 good2[19]={0xEE,0xB1,0x10,0x00,0x02,0x00,0x05,0xC1,0xAC,0xBD,0xD3,0xB3,0xC9,0xB9,0xA6,0xFF,0xFC,0xFF,0xFF};

u8 id[6]={0};
u8 pihao[20]={0};
u8 guige[20]={0};
u8 shase[10]={0};
u8 guanse[10]={0};
u8 pinming[10]={0};
u8 dengji[10]={0};
u8 xianghao[40]={0};
u8 riqi[9]={0x39,0x39,0x2D,0x39,0x39,0x2D,0x39,0x39};
u8 yeshu=0;
int tongshu=6;
float xiangzhong=1.3;
float tongzhong=0.2;
float tongzong=2.5;
u8 screen=2;
u8 lockcount=0;
u8 wifi_state=0;
void u3_sendStr(u8 buf[],u8 len)
	{
		u8 i;
		for(i=0;i<len;i++)
		{
			USART3->DR=buf[i];
			while((USART3->SR&0X40)==0);
		}
	}
void u3_send(u8 P)
	{
		
			USART3->DR=P;
			while((USART3->SR&0X40)==0);
		
	}
	void u3_printf(char* fmt,...)  
{  
	
	
	u8 i,len;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	len = strlen((const char*)USART3_TX_BUF);
		
		for(i=0;i<len;i++)
		{
			USART3->DR=USART3_TX_BUF[i];
			while((USART3->SR&0X40)==0);
		}
}	
	void USART3_IRQHandler(void)
		{
			u8 res;
			if(USART3->SR&(1<<5))
			{
				res=USART3->DR;
				if((USART3_RX_STA&0x8000)==0)
				{
					if(USART3_RX_STA&0x4000)
					{
						if(res==0xFF)
						{
							USART3_RX_STA|=0x8000;
						}else
						{
							USART3_RX_BUF[USART3_RX_STA&0x3FFF]=res;
							USART3_RX_STA++;
							if((USART3_RX_STA&0X3FFF)>USART3_REC_LEN-1)USART3_RX_STA=0;
						}
						
					}else
					{
						if(res==0xEE)USART3_RX_STA|=0X4000;
					
					}
				}
			}
		}

void u3_main(void)
{
	u8 i,len;
	if(USART3_RX_STA&0x8000)
	{
		len=USART3_RX_STA&0x3FFF;
		//u1_Send(len);
		if(USART3_RX_BUF[1]==0x11)
		{
			if(USART3_RX_BUF[3]==0x00)
			{
				if(USART3_RX_BUF[5]==0x12)    //设置筒数 按钮
				{
					setNum(USART3_RX_BUF,len);
				}
				else if(USART3_RX_BUF[5]==0x25)// 删除数据按钮
				{
					delete_();
				}
				else if(USART3_RX_BUF[5]==0x18)// 确认打印按钮
				{
					confprint();
					show_thelast();
				}
				else if(USART3_RX_BUF[5]==0x13) //切换第二屏
				{
					if(wifi_state==1)goscreen1();
					
				}
				
			}
			else if(USART3_RX_BUF[3]==0x01)
			{
				if(USART3_RX_BUF[5]==0x04)   // 设置工艺编号
				{
					setGNum(USART3_RX_BUF,len);
				}
				else if(USART3_RX_BUF[5]==0x06)  //确认返回 第一屏
				{
					goscreen0();
					
				}
				else if(USART3_RX_BUF[5]==0x09) 
				{
					xiaye();
				}
				else if(USART3_RX_BUF[5]==0x0A) 
				{
					shangye();
				}
				
			}
			else if(USART3_RX_BUF[3]==0x02)
			{
				if(USART3_RX_BUF[5]==0x03)
				{
					if(wifi_state==1)goscreen0();
				}
			}
			
		}
		
		USART3_RX_STA=0;
	}
	if(num_ok==1)
	{
		if(u1_count==1)
		{
			int a=0;
			float b=0;
			a=jinzhong;
			b=(jinzhong-a)*10;
			u1_printf("%s",data1);
			u1_Send(0x2A);
			u1_printf("%s",id);
			u1_Send(0x2A);
			u1_printf("%.1f",weight);
			u1_Send(0x2A);
			u1_printf("%.1f",jinzhong);
			u1_Send(0x2A);
			u1_printf("%s",id);
			u1_printf("%s",numid);
			u1_printf("Z");
			u1_printf("%d",a);
			u1_printf("K");
			u1_printf("%d",(int)(b));
		
			send_u2();
			for(i=0;i<9;i++)numid[i]=chenggong[i];
			u1_count=0;
			num_ok=0;
	
			show_thelast();
			
		}

		
	}
}
void show_thelast(void)
{
	u1_printf("~0303");
}
void delete_(void)
{
	u1_printf("~0304%s#",id);
	delay_ms(100);
	show_thelast();
}
void brushscreen(void)
{
	jinzhong=weight-tongzong;
	if(jinzhong<0)jinzhong=0;
	showconnum(0,18,tongshu);
	showconnumf(0,16,weight);
	showconnumf(0,17,jinzhong);
	showcontex(0,15,numid);
	showcontex(0,14,riqi);
	//showcontex(0,14,numid1);
	
	
	if(ok_count>3)
	{
		u3_sendStr(erro0,19);
		u3_sendStr(erro2,19);
		wifi_state=0;
	}
	else 
	{

		u3_sendStr(good0,19);
		u3_sendStr(good2,19);
		wifi_state=1;

	}
	ok_count++;
}
void showconnumf(u8 screenid,u8 conid,float p)
{
	u3_send(0xEE);
	u3_send(0xB1);
	u3_send(0x10);
	u3_send(0x00);
	u3_send(screenid);
	u3_send(0x00);
	u3_send(conid);
	u3_printf("%.1f",p);
	u3_sendStr(datalast,4);
	
	
}
void showconnum(u8 screenid,u8 conid,int p)
{
		u3_send(0xEE);
	u3_send(0xB1);
	u3_send(0x10);
	u3_send(0x00);
	u3_send(screenid);
	u3_send(0x00);
	u3_send(conid);
	u3_printf("%d",p);
	u3_sendStr(datalast,4);
	
	
}
void showcontex(u8 screenid,u8 conid,u8 buf[])
{
	u3_send(0xEE);
	u3_send(0xB1);
	u3_send(0x10);
	u3_send(0x00);
	u3_send(screenid);
	u3_send(0x00);
	u3_send(conid);
	u3_printf("%s",buf);
	u3_sendStr(datalast,4);
}

void goscreen(u8 id)
{
	u3_send(0xEE);
	u3_send(0xB1);
	u3_send(0x00);
	u3_send(0x00);
	u3_send(id);
	u3_sendStr(datalast,4);
	
}
void setNum(u8 buf[],u8 len)
{
	u8 i;
	//len=sizeof(&buf);
	u1_Send(len);
	if(len<11)
	{
		switch (len)
		{
			case 9:tongshu=buf[7]-0x30;break;
			case 10:tongshu=buf[8]-0x30+(buf[7]-0x30)*10;break;
			//default :tongshu=6;
		}
	
	}
	tongzong=xiangzhong+tongzhong*tongshu;
	u1_printf("%d",tongshu);
}
void setid(void)
{
		
		lockcount=1;
}
void confprint(void)
{
	if(wifi_state==1)
	{
		u8 i;
				u3_send(0xEE);
				u3_send(0xB1);
				u3_send(0x10);
				u3_send(0x00);
				u3_send(0x00);
				u3_send(0x00);
				u3_send(0x21);
				u3_send(0xFF);
				u3_send(0xFC);
				u3_send(0xFF);
				u3_send(0xFF);
		
		for(i=0;i<9;i++)numid[i]=0;
		u1_printf("~0302");
		num_ok=1;
		
	}
	
}
void goscreen0(void)
{
	
	goscreen(0x00);
	u1_printf("~0101");
	screen=0;
	showcontex(0,10,pihao);
	showcontex(0,11,guige);
	showcontex(0,12,shase);
	showcontex(0,13,guanse);
	
	showcontex(0,30,pinming);
	showcontex(0,32,dengji);
	show_thelast();
}
void goscreen1(void)
{
	goscreen(0x01);
	u1_printf("~02010");
	yeshu=0;
	screen=1;
	
}
void clear(void)
{
	u3_send(0xEE);
	u3_send(0x01);
	u3_send(0xFF);
	u3_send(0xFF);
	u3_send(0xFC);
	u3_send(0xFF);
	
}
void shangye(void)
{
	if(yeshu<9)
	{
	goscreen(0x00);
	yeshu++;
	goscreen(0x01);
	u1_printf("~0201%d",yeshu);
	}
}

void xiaye(void)
{
	if(yeshu>0)
	{
	
	yeshu--;
	goscreen(0x00);
	goscreen(0x01);
	u1_printf("~0201%d",yeshu);
	}
}
void setGNum(u8 buf[],u8 len)
{
	//u2_send(len);
	//u2_sendstr(buf,len);
	if(len==12)
	{
		u1_printf("~0202*");
		u1_Send(buf[7]);
		u1_Send(buf[8]);
		u1_Send(buf[9]);
		u1_Send(buf[10]);
		
	}
	else if(len==13)
	{
		u1_printf("~0202*");
		u1_Send(buf[7]);
		u1_Send(buf[8]);
		u1_Send(buf[9]);
		u1_Send(buf[10]);
		u1_Send(buf[11]);
		
	}
	
}



void uart3_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<3;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口时钟 
	GPIOB->CRH&=0XFFFF00FF;//IO状态设置  PB 10 11
	GPIOB->CRH|=0X00008B00;//IO状态设置
		  
	RCC->APB1RSTR|=1<<18;   //复位串口1
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART3_IRQChannel,2);//组2，最低优先级 

}
