#ifndef __USART2_H
#define __USART2_H

#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h> 
#include "stdio.h"	 


#define USART2_REC_LEN	200
extern u8  USART2_RX_BUF[USART2_REC_LEN];
extern u16 USART2_RX_STA;
extern u8 USART2_TX_BUF[1024];


















void send_u2(void);
void uart2_init(u32 pclk2,u32 bound);
	void u2_printf(char* fmt,...);
void u2_sendstr(u8 buf[],u16 len);
void u2_send(u8 P);
#endif


