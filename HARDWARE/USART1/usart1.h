#ifndef __USART1_H
#define __USART1_H

#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h> 
#include "stdio.h"	 

#define USART1_REC_LEN 4048	
#define USART1_MAX_SEND_LEN		200					
//extern u8 text_x[100][70];
extern u8  USART1_RX_BUF[USART1_REC_LEN];
extern u16 USART1_RX_STA;     
extern u8 numid[9];
extern u8 numid1[9];
extern u8 u1_count;
extern int nun;
extern u8 Sys_Status;
extern u8 ok_count;
void uart1_init(u32 pclk2,u32 bound);
void u1_Send(u8 P);
void u1_printf(char* fmt,...);
	void u1_SendStr(u8 buf[],u8 len);
	void u1_main(void);
	void TakeStrOut(u8 buf[],u16 len);
void takenum(u8 buf[],u8 len);

	
#endif








