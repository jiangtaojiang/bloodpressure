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
//#define DEBUG 1
#define null	0
#define DEBUG 1

#define LED1_ON		GPIOC->BSRR=GPIO_Pin_10
#define LED1_OFF 	GPIOC->BRR=GPIO_Pin_10

#define LED2_ON		GPIOC->BSRR=GPIO_Pin_11
#define LED2_OFF 	GPIOC->BRR=GPIO_Pin_11

//输入宏定义
#define GET_S1()	((GPIOA->IDR&GPIO_Pin_0))
#define GET_S2()	((GPIOC->IDR&GPIO_Pin_7))
#define GET_S3()	((GPIOC->IDR&GPIO_Pin_8))
#define GET_S4()	((GPIOC->IDR&GPIO_Pin_9))
//字库在FLASH中的起始位置
#define HZ_DOT16_START_ADDR		0
#define HZ_DOT24_START_ADDR		0x34e00
//用户FLASH的起始位置
#define USR_FLASH_START_ADDR	0xabd80
//用户FLASH的末端(注意不能等于这个值)
#define USR_FLASH_END_ADDR		0x100000

#ifdef  main_c
u8 rtc_sec_flag;
u8 recont_flag;
u8 ms10_flag;
u32 ms_count = 0;
u16 ms100 = 0;
u8 ADC_DMA_TRSMIT_OK = 0;
u8 rxdata;
u8 start_flag = 0;
u8 change_wait = 0;
//#define LED4_ON	        GPIO_ResetBits(GPIOC, GPIO_Pin_5)
//#define LED4_OFF 	GPIO_SetBits(GPIOC, GPIO_Pin_5)

int Write_Log(const char *format,...);
#else
extern u8 rtc_sec_flag;
extern u16 ms_count;
extern u16 ms100;
extern u8 recont_flag;
extern u8 ms10_flag;
extern u8 ADC_DMA_TRSMIT_OK;
extern u8 rxdata;
extern u8 start_flag;
extern u8 change_wait;
extern int Write_Log(const char *format,...);
#endif