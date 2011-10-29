#include "common.h"
#include "usart.h"
#include "string.h"
#include "24c256.h"
#include <time.h>
#include "business.h"
#include "bt.h"
#include "rtc.h"


//-------------------------------------------------------------------
//事件标志变量  参考common.h说明
volatile unsigned long evt_state = 0;
//volatile unsigned char send_flag = 0;

//延时delay_ms需要使用  定时器使其1ms加1
volatile unsigned long msTicks = 0;	 	
//-------------------------------------------------------------------

//延时 n ms  采用中断方式
void delay_ms(unsigned long dlyTicks)
{	unsigned long curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks) ;
}

//延时 n s  中断方式
void delay_sec(unsigned long dlys)
{	while(dlys--)
	{	delay_ms(1000);
	}
}

//延时n毫秒 没采用中断
void delay(unsigned long t)
{	volatile unsigned long tt = 18000;//MDK测试 1ms
	while(t--)
	{	tt = 18000;
		while(tt--)
		{	tt=tt;
		}
	}
}

//显示系统时间
void disp_time(void)
{
	uint32_t curr_time = 0;
	uint8_t ts[30] = {0};	
	
	curr_time += (uint32_t)(RTC->CNTH)<<16;
	curr_time +=  RTC->CNTL;
	strftime((char *)ts,30,"%Y-%m-%d  %H:%M:%S  ",localtime((time_t *)&curr_time));
	printf("current time: %s\r\n",ts); 
}

//获取item 存入和发送
/*
void disp_item(void)
{	printf("send_items: %d\r\n",get_send_item());
	printf("save_items: %d\r\n",get_save_item());
	return;
}
*/

void standby(void)
{
	
}