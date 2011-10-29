/******************************************************************************
* file Name     : stm32_adc.h                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#ifndef _Stm32_ADC_H
#define _Stm32_ADC_H

#include "integer.h"
/*define the constant for display digital char*/
#define	D0		0
#define	D1		1
#define	D2		2
#define	D3		3
#define	D4		4
#define	D5		5
#define	D6		6
#define	D7		7
#define	D8		8
#define	D9		9
#define	DPoint	10	//"."
#define DDash	20	//"-"
#define DColon	21	//":"

#define ADC1_DR_ADDRESS    ((uint32_t)0x4001244C) 

#ifdef Stm32_ADC_c
u8 duty_change = 0;
int AD_value;
char value[5];
uint16_t ADC_ValueTab[3];

void Adc_Init(void);
void Adc_GPIOConfig(void);
void Adc_Config(void);
void ADC_Test(void);

#else
extern u8 duty_change;
extern int AD_value;
extern char value[5];
extern void Draw_xy(void);
extern void Draw_wave(void);
extern void ADC_Test(void);

extern void Adc_Init(void);
#endif

#endif