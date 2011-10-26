/******************************************************************************
* file Name     : Stm_Pwm.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#define  Stm_Pwm_c
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include "main.h"

uint16_t CCR1_Val = 2047;
uint16_t CCR2_Val = 1535;
uint16_t CCR3_Val = 1023;
uint16_t CCR4_Val = 511;
/*******************************************************************************
* Function Name  : PWM_GPIOConfig
* Description    : PWM_GPIOConfig
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void PWM_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//| GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : PWM_TIMConfig
* Description    : PWM_TIMConfig
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void PWM_TIMConfig(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM1_BDTRInitStructure;
    
     /* TIM1 clock enable */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    /* TIM1 Peripheral Configuration */ 
    TIM_DeInit(TIM1);
   
    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 7200;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
     /* Channel 1, 2,3 and 4 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //PWM功能使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 180;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    /* Automatic Output enable, Break, dead time and lock configuration*/
/*    TIM1_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM1_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM1_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1; 
    TIM1_BDTRInitStructure.TIM_DeadTime = 0x75;
    TIM1_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
    TIM1_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM1_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;

    TIM_BDTRConfig(TIM1,&TIM1_BDTRInitStructure);*/
    
//    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
//    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

//    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
//    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

//    TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
//    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

  /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
/*******************************************************************************
* Function Name  : PWM_Init
* Description    : PWM_Init
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void PWM_Init(void)
{
    PWM_GPIOConfig();
    PWM_TIMConfig();
}
