#ifndef __USART4_H
#define __USART4_H	 
#include "sys.h"  

#define USART4_MAX_RECV_LEN		200					
#define USART4_MAX_SEND_LEN		200					
			

extern u8  USART4_RX_BUF[USART4_MAX_RECV_LEN]; 	
extern u8  USART4_TX_BUF[USART4_MAX_SEND_LEN]; 		
extern u16 USART4_RX_STA;   						
extern float weight;
extern float jinzhong;
void uart4_Init(u32 pclk2,u32 bound);				
void TIM4_Set(u8 sta);
void TIM4_Init(u16 arr,u16 psc);
void u4_printf(char* fmt, ...);
void USART4_SendStr(u16 buf[],u8 len);
void USART4_Send(u16 P);
void u4_main(void);
#endif













