/******************************************************************************
* file Name     : usart.h                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#include "stm32f10x.h"
#include "integer.h"
#include "RTC.h"
#include <stdio.h>

#define DEBUG 1
#if DEBUG == 1
#define _log(x) do{curr_time();printf(x);}while(0)
#else
	#define _log(x) 
#endif

#define USART2_DR_Base 0x40004404
#define cm_uart USART1
#define bt_uart USART2

#ifdef  usart_c
void Usart_Init(void);
void Usart_Config(USART_TypeDef *uart, uint32_t bps);
void USART2_Puts(char * str);
void DMA_Configuration(void);
void DMA_TxConfig(BYTE *BufferSRC, uint32_t BufferSize);
void cm_out(unsigned char ch);
void xtcp_outb(unsigned char ch);
void xtcp_outs(void *p,unsigned short len);
void USART3_Putc(unsigned char c);
#else
extern void Usart_Init(void);
extern void Usart_Config(USART_TypeDef *uart, uint32_t bps);
extern void USART2_Puts(char * str);
extern void DMA_Configuration(void);
extern void DMA_TxConfig(BYTE *BufferSRC, uint32_t BufferSize);
extern void cm_out(unsigned char ch);
extern void xtcp_outb(unsigned char ch);
extern void xtcp_outs(void *p,unsigned short len);
extern void USART3_Putc(unsigned char c);
extern void USART3_Puts(char *str,unsigned char len);
#endif