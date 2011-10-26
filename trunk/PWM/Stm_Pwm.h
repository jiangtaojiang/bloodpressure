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

#ifdef Stm_Pwm_c
void PWM_Init(void);

#else
extern void PWM_Init(void);
#endif

#endif