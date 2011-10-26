/******************************************************************************
* file Name     : main.h                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#include "stm32f10x.h"
//#include "ili932x.h"
#define DEBUG 1

//#define LED1_OFF		GPIO_ResetBits(GPIOA, GPIO_Pin_0)
//#define LED1_ON     	GPIO_SetBits(GPIOA, GPIO_Pin_0)

//#define LED2_OFF		GPIO_ResetBits(GPIOA, GPIO_Pin_1)
//#define LED2_ON     	GPIO_SetBits(GPIOA, GPIO_Pin_1)

//#define LED7_OFF		GPIO_ResetBits(GPIOF, GPIO_Pin_6)
//#define LED7_ON     	GPIO_SetBits(GPIOF, GPIO_Pin_6)

#ifdef  main_c
u8 rtc_sec_flag;
u8 recont_flag;
u32 ms_count = 0;
u16 ms100 = 0;
u8 ADC_DMA_TRSMIT_OK = 0;
//#define LED4_ON	        GPIO_ResetBits(GPIOC, GPIO_Pin_5)
//#define LED4_OFF 	GPIO_SetBits(GPIOC, GPIO_Pin_5)


#else
extern u8 rtc_sec_flag;
extern u16 ms_count;
extern u16 ms100;
extern u8 recont_flag;
extern u8 ADC_DMA_TRSMIT_OK;
#endif