#ifndef __USART3_H
#define __USART3_H

#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h> 
#include "stdio.h"	 

#define USART3_REC_LEN	200
extern u8  USART3_RX_BUF[USART3_REC_LEN];
extern u16 USART3_RX_STA;
extern u8 id[6];
extern u8 pihao[20];
extern u8 guige[20];
extern u8 shase[10];
extern u8 guanse[10];
extern u8 pinming[10];
extern u8 dengji[10];
extern u8 riqi[9];
extern int tongshu;
extern float tongzong;
extern u8 wifi_state;
extern u8 screen;


void uart3_init(u32 pclk2,u32 bound);
void u3_sendStr(u8 buf[],u8 len);
void u3_send(u8 P);
void u3_main(void);
	void u3_printf(char* fmt,...);
void setNum(u8 buf[],u8 len);
void setid(void);
void confprint(void);
void goscreen1(void);
void setGNum(u8 buf[],u8 len);
void goscreen0(void);
void brushscreen(void);
void goscreen(u8 id);
void delete_(void);
void show_thelast(void);
void takenum(u8 buf[],u8 len);
void showcontex(u8 screenid,u8 conid,u8 buf[]);
void showconnum(u8 screenid,u8 conid,int p);
void showconnumf(u8 screenid,u8 conid,float p);
void shangye(void);
void xiaye(void);
void clear(void);
#endif

 

