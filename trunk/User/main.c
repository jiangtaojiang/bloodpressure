/******************************************************************************
* Project Name  :                                                                            
* Description   : Main program body
* MCU           ：stm32103fvc
* Data          : 2011-3-22                                              
* Version       ：Ver 1.0.0.3                                                                   
* Programmer    ：chen                                                                           
* Updata        ：chen   
* FWLib version : V3.4
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#define  main_c
#include "stm32f10x.h"
//#include "stm32_eval.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "stm32f10x_conf.h"
#include "main.h"
#include "RCC_config.h"
#include "usart.h"
#include "usr.h"
#include "time.h"
#include "rtc.h"
#include "hal.h"
#include "DX_GUI.h"
#include "Stm32_ADC.h"
#include "24c256.h"
#include "Stm_Pwm.h"
//#include "hw_config.h"
bool Touch_Adj=FALSE;
volatile bool Ms_20=FALSE;
volatile bool Ms_1=FALSE;
volatile bool RTC_1S=FALSE;
//extern void LCD_Init(void);
/* --------------------------------------------------------------------------*/
void GetTime(void)
{
    T_STRUCT time;
    struct tm Rtime;
    Rtime = Time_GetCalendarTime();
    
    time.year=Rtime.tm_year;
	time.month=Rtime.tm_mon;
	time.day=Rtime.tm_mday;
	time.hour=Rtime.tm_hour;
	time.minute=Rtime.tm_min;
	time.sec=Rtime.tm_sec;
	time.date=Rtime.tm_wday;
    //GetRTCTime(&time);
	SetRTCBar(&time);
}
/*******************************************************************************
* Function Name  : Main program body
* Description    : .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
//    int n = 0;
//    u16 id = 0x0;
    T_STRUCT time;
    system_init();
    GuiInit();
    Write_Log("system init...\r\n");

//	LED1_OFF;
//	LED2_OFF;
//    SysTickDelay(100);
    LED1_ON;
    LED2_ON;
//    id = SST25ReadID();   
//	printf("ID = %4x",id);
    GetRTCTime(&time);
	SetRTCBar(&time);
    UsrInterface();
    while(1)
    {
        if(rtc_sec_flag==1)
        {
            rtc_sec_flag = 0;
//            ADC_Test();
            UpdateRTCBar();				//刷新时间栏
           
        }
//        if(ADC_DMA_TRSMIT_OK==1)
        {
//            ADC_DMA_TRSMIT_OK = 0;
//            ADC_Test();
            if(ms10_flag==1)
            {
                ms10_flag = 0;
                
//                if(start_flag==1)
                {
                    Draw_wave();              
                }
            }
        }
        if(duty_change==1)
        {
            duty_change = 0;
            TIM1->CCR4 = ARR_Period-173; 
            change_wait = 2;
        }
        if(duty_change==2)
        {
            duty_change = 0;
            change_wait = 3;
//            TIM1->CCR1 = ARR_Period-240;
//            TIM1->CCR4 = ARR_Period-192;
        }
        if(duty_change==3)
        {
            duty_change = 0;
            TIM1->CCR4 = 0;              //气泵关
            TIM1->CCR1 = ARR_Period+1; //气阀开，快速放气
            change_wait = 0;
        }
        if(GET_S1())
        {
            SysTickDelay(10);
            if(GET_S1())
            {    
                TIM_Cmd(TIM1, ENABLE);
                
                TIM1->CCR1 = 0;  //气阀关
                SysTickDelay(1000);
                TIM1->CCR4 = ARR_Period-100;  //气泵开
                change_wait = 1;
//                SysTickDelay(3500);
//                TIM1->CCR4 = 60; 
            }
        }
        if(GET_S2())
        {
            SysTickDelay(10);
            if(GET_S2())
            {
                TIM1->CCR4 = 0; //气泵关
//                TIM1->CCR4 = 0;  //气泵开
//                TIM1->CCR1 = 15;
                SysTickDelay(10);
//                TIM_Cmd(TIM1, DISABLE);//DISABLE
            }
        }
        if(GET_S3())
        {
            SysTickDelay(10);
            if(GET_S3())
            {    
                TIM_Cmd(TIM1, ENABLE);
                TIM1->CCR1 = 0; //气阀开，缓慢放气
            }
        }
        if(GET_S4())
        {
            SysTickDelay(10);
            if(GET_S4())
            {    
                TIM1->CCR1 = ARR_Period+1; //气阀开，快速放气
            }
        }
    }
}

/*******************************************************************************
* Function Name  : fputc
* Description    : 自定义fputc()函数，用于printf()函数,从串口输出信息。
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
#ifdef DEBUG
int fputc(int ch, FILE *f)
{
	while(!(USART3->SR & USART_FLAG_TXE));
	USART3->DR = (u8) ch;

	return(ch);
}
#endif


int Write_Log(const char *format,...)
{
#ifdef DEBUG
//    curr_time();
//    printf(format);
    return 1;
#else
    return 0;
#endif   
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
