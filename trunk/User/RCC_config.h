/******************************************************************************
* file Name     : RCC_config.h                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#include "stm32f10x.h"
//#include "ili932x.h"
#ifdef  RCC_config_c

void GPIO_Clock_config(void);
void system_init(void);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void NVIC_Configuration(void);
void Tim_Init(void);
void PVD_config(void);
void Systick_Configuration(void);
void SysTickDelay(u16);
#else
extern void GPIO_Clock_config(void);
extern void system_init(void);
extern void GPIO_Configuration(void);
extern void EXTI_Configuration(void);
extern void NVIC_Configuration(void);
extern void Tim_Init(void);
extern void PVD_config(void);
extern void SysTickDelay(u16);
#endif