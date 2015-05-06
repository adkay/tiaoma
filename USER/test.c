#include "sys.h"
//#include "usartall.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h"	 	 
#include "key.h"	
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart4.h"
#include "timer.h"
#include "24cxx.h"
#include "myiic.h"
#include "dma.h"
#include "string.h"

/*

usart1  ͨ��wifiģ������λ��ͨѶ  ����תTCP/IP  PA9  PA10   TX RX
usart2  ��ӡ��ͨѶ     PA2 PA3
usart3  ���ڴ�����ͨѶ PB10  PB11
uart4   ���ӳ�ͨѶ  PC10 PC11

stm32   ---->> PC  0201   ��ȡ����������Ϣ  ���س��ı�  
stm32  ----->>pc	 0202   ��������ID     ����ID��ʾ�����ž�����Ϣ 
stm32  ----->>pc	 ~0301*1001*22*25*10010122*  	���ʹ����Ϣ��ʽΪ~0302*ID*ë��*����*��� 
									 0302   ���ص�ǰ����ļ�¼



*/



void in_it(void)
{
	u8 i,len;
	u8 k,addr;
	len=AT24CXX_ReadOneByte(0);
	for(i=0;i<len;i++)id[i]=AT24CXX_ReadOneByte(1+i);
	len=AT24CXX_ReadOneByte(40);
	for(i=0;i<len;i++)pihao[i]=AT24CXX_ReadOneByte(41+i);
	len=AT24CXX_ReadOneByte(80);
	for(i=0;i<len;i++)guige[i]=AT24CXX_ReadOneByte(81+i);
	len=AT24CXX_ReadOneByte(120);
	for(i=0;i<len;i++)shase[i]=AT24CXX_ReadOneByte(121+i);
	len=AT24CXX_ReadOneByte(160);
	for(i=0;i<len;i++)guanse[i]=AT24CXX_ReadOneByte(161+i);
	len=AT24CXX_ReadOneByte(200);
	for(i=0;i<len;i++)pinming[i]=AT24CXX_ReadOneByte(201+i);
	len=AT24CXX_ReadOneByte(240);
	for(i=0;i<len;i++)dengji[i]=AT24CXX_ReadOneByte(241+i);
	
}

int main(void)
{			
	u16 count=0,i;
	u32 t1,t2;
	Stm32_Clock_Init(9);	
	delay_init(72);	 
	uart1_init(72,57600);	
	uart2_init(36,57600);	
	uart3_init(36,115200);	
	TIM3_Int_Init(4999,7199);
	uart4_Init(36,9600);	
	AT24CXX_Init();
	MYDMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)USART2_TX_BUF,508);
	  	 	//��ʱ��ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	BEEP_Init();         	//��ʼ���������˿�
	KEY_Init();         	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	t1=time;
	t2=time;

	delay_ms(100);
	in_it();
	delay_ms(100);
	u1_printf("~0101");
	delay_ms(100);
	
	goscreen(0x02);
	
	
 	while(1)
	{
		u1_main();  //wifi �������ݴ���
		u4_main();  //���ӳ� ���ݴ���
		u3_main();  //������
		if((t2-t1)>0)  //1Sһ��ִ��
		{
			t1=time;
		  LED0=!LED0;
			LED1=!LED1;
			

			send_u2();
			u1_printf("~0102%s#",id);
			
			brushscreen();
			
		}
		t2=time;
   

				 
				 
			
	}

}


