/******************************************************************************
* Project Name  :                                                                            
* Description   : Main program body
* MCU           ：stm32103fc8
* Data          : 2011-3-22                                              
* Version       ：Ver 1.2.0.3                                                                   
* Programmer    ：chen                                                                           
* Updata        ：chen   
* FWLib version : V3.4
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#define  main_c
#include "stm32f10x.h"
//#include "stm32_eval.h"
#include <stdio.h>
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
//#include "usb_lib.h"
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
    T_STRUCT time;
    system_init();
    GuiInit();
#ifdef DEBUG
        printf("系统初始化....\r\n");    
#endif
//    Lcd_Initialize();
//	LED1_OFF;
	LED2_OFF;
//	LED3_OFF;
//	LED4_OFF;
//	LED5_OFF;
//	LED6_OFF;
//	LED7_OFF;
//	LED8_OFF;

    GetRTCTime(&time);
	SetRTCBar(&time);
    UsrInterface();
    printf("系统初始化....\r\n");    
    while(1)
    {
//        SysTickDelay(400);
        if(rtc_sec_flag==1)
        {
            rtc_sec_flag = 0;
            ADC_Test();
            UpdateRTCBar();				//刷新时间栏
        }
        if(ADC_DMA_TRSMIT_OK==1)
        {
            ADC_DMA_TRSMIT_OK = 0;
//            ADC_Test();
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
	while(!(USART1->SR & USART_FLAG_TXE));
	USART1->DR = (u8) ch;

	return(ch);
}
#endif

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
