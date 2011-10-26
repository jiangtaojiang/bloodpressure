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

#define USART2_DR_Base 0x40004404
#ifdef  usart_c
void Usart_Init(void);
void Usart_Config(USART_TypeDef *uart, uint32_t bps);
void USART2_Puts(char * str);
void DMA_Configuration(void);
void DMA_TxConfig(BYTE *BufferSRC, uint32_t BufferSize);
#else
extern void Usart_Init(void);
extern void USART2_Puts(char * str);
extern void DMA_Configuration(void);
extern void DMA_TxConfig(BYTE *BufferSRC, uint32_t BufferSize);
#endif