/**************************************************************
�û����ܺ���.�����ϲ�Ӧ�õ�һЩ����,һ���ַ���MAIN.C��,һ���ַ����� 
�д����� 
***************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usr.h"
#include "DX_gui.h"
#include <stdio.h>
#include "hal.h"
//#include "my375lib.h"
#include "main.h"

extern DX_EDIT BAR_WIN_TIME_YEAR;  //��
extern DX_EDIT BAR_WIN_TIME_MONTH;	//��
extern DX_EDIT BAR_WIN_TIME_DAY;	//��
extern DX_EDIT BAR_WIN_TIME_HOUR;	//ʱ
extern DX_EDIT BAR_WIN_TIME_MINUTE;	//��
extern DX_EDIT BAR_WIN_TIME_SECOND;	//��
extern DX_COMBO BAR_WIN_TIME_DATE;	//����

/**************************************************************
** ������:SetRTCBar
** ����:����RTC��Ŀ��ʱ��
** ע������:��UpdateRTCBar��ͬ,�˺���ֱ�Ӱ�ÿ��ֵ��ˢ��һ��,һ�����ض�����µ���.�翪����ʱ��
***************************************************************/
void SetRTCBar(T_STRUCT* time)
{
	SetEditTmp(&BAR_WIN_TIME_YEAR,time->year);//��
	SetEditTmp(&BAR_WIN_TIME_MONTH,time->month);//��
	SetEditTmp(&BAR_WIN_TIME_DAY,time->day);//��
	SetEditTmp(&BAR_WIN_TIME_HOUR,time->hour);//ʱ
	SetEditTmp(&BAR_WIN_TIME_MINUTE,time->minute);//��
	SetEditTmp(&BAR_WIN_TIME_SECOND,time->sec);//��
	SetComboTmp(&BAR_WIN_TIME_DATE,time->date);//����
#ifdef DEBUG    
//    printf("20%d��%d��%d��%d:%d:%d \r\n",time->year,time->month,time->day,time->hour,time->minute,time->sec);
#endif
}

/**************************************************************
**������:UpdateRTCBar 
**����:ˢ��ʱ����ʾ
**ע������:�Ȼ�ȡʱ��ṹ��,�ٸ���ʱ��.Ϊ���ִ��Ч��,�˺����������������ʱ��,����Ӧ����ѭ����ÿһ��RTC�жϱ�־����һ��
***************************************************************/
void UpdateRTCBar(void)
{
	T_STRUCT time;
	GetRTCTime(&time);
	//�����ж�
	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	//��
	if (GetEditTmp(&BAR_WIN_TIME_SECOND)!=time.sec)
	{
		SetEditTmp(&BAR_WIN_TIME_SECOND,time.sec);

		//��
		if (GetEditTmp(&BAR_WIN_TIME_MINUTE)!=time.minute)
		{
			SetEditTmp(&BAR_WIN_TIME_MINUTE,time.minute);

			//ʱ
			if (GetEditTmp(&BAR_WIN_TIME_HOUR)!=time.hour)
			{
				SetEditTmp(&BAR_WIN_TIME_HOUR,time.hour);

				//��
				if (GetEditTmp(&BAR_WIN_TIME_DAY)!=time.day)
				{
					SetEditTmp(&BAR_WIN_TIME_DAY,time.day);
					SetComboTmp(&BAR_WIN_TIME_DATE,time.date);

					//��
					if (GetEditTmp(&BAR_WIN_TIME_MONTH)!=time.month)
					{
						SetEditTmp(&BAR_WIN_TIME_MONTH,time.month);

						//��
						if (GetEditTmp(&BAR_WIN_TIME_YEAR)!=time.year)
						{
							SetEditTmp(&BAR_WIN_TIME_YEAR,time.year);
						}
					}
				}
			}
		}
	}
	//�����ж�
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
}


/**************************************************************
** ������:SetIcon
** ����:�����û������ϵ�һ��ͼ��
** ע������:�����粿�ֵ���,ʵ����һ���������TFT�ļ�����
***************************************************************/
extern DX_CHECK MAIN_WIN_CHECK_NET;
void SetIcon(u8 onoff)
{
	if(onoff)
	{
		SetCheckTmp(&MAIN_WIN_CHECK_NET,TRUE);
	}
	else
	{
		SetCheckTmp(&MAIN_WIN_CHECK_NET,FALSE);
	}
}

/**************************************************************
** ������:UpdateAdc
** ����:����ADֵˢ�½�����
** ע������:��ø�һ��ʱ�����һ��
***************************************************************/
extern DX_PROGRESS MAIN_WIN_PROGRESS;
void UpdateAdc(void)
{
	static u16 old_ad=0;
	u16 new_ad;
	static u16 buf[10];
	static u32 i=0;
	
//	buf[i++]=TestAdc();
	if(i==10)
	{
		new_ad=DigitFilter(buf,10);
		i=0;
		if(old_ad!=new_ad)
		{
			old_ad=new_ad;
			SetProgressTmp(&MAIN_WIN_PROGRESS,old_ad);
		}
		
	}
}

/**************************************************************
** ������:UpdateUDisk
** ����:ˢ��U��״̬
** ע������:�򵥵���ʾU���Ƿ����
***************************************************************/
/*
extern DX_CHECK MAIN_WIN_CHECK_UDISK;
void UpdateUDisk(void)
{
	u8 rtn;
	rtn=CheckUDisk();
	if(rtn==TRUE)
	{
		SetCheckTmp(&MAIN_WIN_CHECK_UDISK,TRUE);
	}
	else
	{
		SetCheckTmp(&MAIN_WIN_CHECK_UDISK,FALSE);
	}
}*/



/**************************************************************
		�ؼ�����Ӧ�����ڴ˶���.
***************************************************************/

/**************************************************************
** ������:RspButtonLed
** ����:���Լ�����Ӧ����
** ע������:���º�LED1��ת,�����뿪,��LED1Ϩ��
***************************************************************/
void RspButtonLed(u8 param)
{
	if(param==WIDGET_PARAM_ENTER)
	{
		LED1_ON;
	}
	else if(param==WIDGET_PARAM_FOCUS_LOST)
	{
		LED1_OFF;
	}
}



