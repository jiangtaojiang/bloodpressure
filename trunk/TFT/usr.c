/**************************************************************
用户功能函数.属于上层应用的一些函数,一部分放在MAIN.C上,一部分放这里 
有待整理 
***************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usr.h"
#include "DX_gui.h"
#include <stdio.h>
#include "hal.h"
//#include "my375lib.h"
#include "main.h"

extern DX_EDIT BAR_WIN_TIME_YEAR;  //年
extern DX_EDIT BAR_WIN_TIME_MONTH;	//月
extern DX_EDIT BAR_WIN_TIME_DAY;	//日
extern DX_EDIT BAR_WIN_TIME_HOUR;	//时
extern DX_EDIT BAR_WIN_TIME_MINUTE;	//分
extern DX_EDIT BAR_WIN_TIME_SECOND;	//秒
extern DX_COMBO BAR_WIN_TIME_DATE;	//星期

/**************************************************************
** 函数名:SetRTCBar
** 功能:设置RTC栏目的时间
** 注意事项:与UpdateRTCBar不同,此函数直接把每个值都刷新一次,一般在特定情况下调用.如开机的时候
***************************************************************/
void SetRTCBar(T_STRUCT* time)
{
	SetEditTmp(&BAR_WIN_TIME_YEAR,time->year);//年
	SetEditTmp(&BAR_WIN_TIME_MONTH,time->month);//月
	SetEditTmp(&BAR_WIN_TIME_DAY,time->day);//日
	SetEditTmp(&BAR_WIN_TIME_HOUR,time->hour);//时
	SetEditTmp(&BAR_WIN_TIME_MINUTE,time->minute);//分
	SetEditTmp(&BAR_WIN_TIME_SECOND,time->sec);//秒
	SetComboTmp(&BAR_WIN_TIME_DATE,time->date);//星期
#ifdef DEBUG    
//    printf("20%d年%d月%d日%d:%d:%d \r\n",time->year,time->month,time->day,time->hour,time->minute,time->sec);
#endif
}

/**************************************************************
**函数名:UpdateRTCBar 
**功能:刷新时间显示
**注意事项:先获取时间结构体,再更新时间.为提高执行效率,此函数根据需求而更新时间,并且应该在循环中每一个RTC中断标志调用一次
***************************************************************/
void UpdateRTCBar(void)
{
	T_STRUCT time;
	GetRTCTime(&time);
	//关秒中断
	RTC_ITConfig(RTC_IT_SEC, DISABLE);
	//秒
	if (GetEditTmp(&BAR_WIN_TIME_SECOND)!=time.sec)
	{
		SetEditTmp(&BAR_WIN_TIME_SECOND,time.sec);

		//分
		if (GetEditTmp(&BAR_WIN_TIME_MINUTE)!=time.minute)
		{
			SetEditTmp(&BAR_WIN_TIME_MINUTE,time.minute);

			//时
			if (GetEditTmp(&BAR_WIN_TIME_HOUR)!=time.hour)
			{
				SetEditTmp(&BAR_WIN_TIME_HOUR,time.hour);

				//日
				if (GetEditTmp(&BAR_WIN_TIME_DAY)!=time.day)
				{
					SetEditTmp(&BAR_WIN_TIME_DAY,time.day);
					SetComboTmp(&BAR_WIN_TIME_DATE,time.date);

					//月
					if (GetEditTmp(&BAR_WIN_TIME_MONTH)!=time.month)
					{
						SetEditTmp(&BAR_WIN_TIME_MONTH,time.month);

						//年
						if (GetEditTmp(&BAR_WIN_TIME_YEAR)!=time.year)
						{
							SetEditTmp(&BAR_WIN_TIME_YEAR,time.year);
						}
					}
				}
			}
		}
	}
	//开秒中断
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
}


/**************************************************************
** 函数名:SetIcon
** 功能:设置用户界面上的一个图标
** 注意事项:被网络部分调用,实际是一个网络控制TFT的简单例子
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
** 函数名:UpdateAdc
** 功能:根据AD值刷新进度条
** 注意事项:最好隔一个时间调用一次
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
** 函数名:UpdateUDisk
** 功能:刷新U盘状态
** 注意事项:简单的显示U盘是否插入
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
		控件的响应函数在此定义.
***************************************************************/

/**************************************************************
** 函数名:RspButtonLed
** 功能:测试键的响应函数
** 注意事项:按下后LED1翻转,若果离开,则LED1熄灭
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



