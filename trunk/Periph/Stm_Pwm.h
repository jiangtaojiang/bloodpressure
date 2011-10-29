/******************************************************************************
* file Name     : Stm_Pwm.h                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#ifndef _Stm_PWM_H
#define _Stm_PWM_H

#include "integer.h"

#define ARR_Period  480

#ifdef Stm_Pwm_c

void PWM_Init(void);

#else

extern void PWM_Init(void);
extern void TIM1_Period_Change(uint16_t Period,uint16_t Prescaler);
extern void Duty_CH1(uint16_t duty);
extern void Duty_CH4(uint16_t duty);
#endif

#endif