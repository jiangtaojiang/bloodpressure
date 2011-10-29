#include "common.h"
#include "usart.h"
#include "string.h"
#include "24c256.h"
#include <time.h>
#include "business.h"
#include "bt.h"
#include "rtc.h"


//-------------------------------------------------------------------
//�¼���־����  �ο�common.h˵��
volatile unsigned long evt_state = 0;
//volatile unsigned char send_flag = 0;

//��ʱdelay_ms��Ҫʹ��  ��ʱ��ʹ��1ms��1
volatile unsigned long msTicks = 0;	 	
//-------------------------------------------------------------------

//��ʱ n ms  �����жϷ�ʽ
void delay_ms(unsigned long dlyTicks)
{	unsigned long curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks) ;
}

//��ʱ n s  �жϷ�ʽ
void delay_sec(unsigned long dlys)
{	while(dlys--)
	{	delay_ms(1000);
	}
}

//��ʱn���� û�����ж�
void delay(unsigned long t)
{	volatile unsigned long tt = 18000;//MDK���� 1ms
	while(t--)
	{	tt = 18000;
		while(tt--)
		{	tt=tt;
		}
	}
}

//��ʾϵͳʱ��
void disp_time(void)
{
	uint32_t curr_time = 0;
	uint8_t ts[30] = {0};	
	
	curr_time += (uint32_t)(RTC->CNTH)<<16;
	curr_time +=  RTC->CNTL;
	strftime((char *)ts,30,"%Y-%m-%d  %H:%M:%S  ",localtime((time_t *)&curr_time));
	printf("current time: %s\r\n",ts); 
}

//��ȡitem ����ͷ���
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