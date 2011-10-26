/****************************************
**usr.h
**用户所有要使用的函数声明一般在此定义，例如驱动某些硬件的API声明
**但本H文件与硬件部分尽量无关
****************************************/
#ifndef USR_H
#define USR_H
#include "stm32f10x.h"

//时间结构体
typedef struct _t_sct{
    u8 year;	//	0-99
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 sec;
    u8 date;
}T_STRUCT;

//没有设置时间下的默认时间
#define DEFAULT_TIME_YEAR	9
#define DEFAULT_TIME_MONTH	11
#define DEFAULT_TIME_DAY	17
#define DEFAULT_TIME_HOUR	17
#define DEFAULT_TIME_MINUTE	19
#define DEFAULT_TIME_SECOND	30

//用于时间
#define BKP_TIME_SET		BKP_DR1	//RTC设置与否标志
#define BKP_TIME_YEAR		BKP_DR2
#define BKP_TIME_MONTH		BKP_DR3
#define BKP_TIME_DAY		BKP_DR4
#define BKP_TIME_DATE		BKP_DR5

extern u16 BG_COLOR;	//全局的背景色,此变量是为了增加运行速度而设置的
//RTC
extern void GetRTCTime(T_STRUCT*);
extern void SetRTCTime(T_STRUCT*);
extern u8 GetDate(T_STRUCT* time);

/**************************************************************
用户API函数
***************************************************************/
//SET_TIME
//extern T_STRUCT Set_Time;

//U16中值滤波
extern u16 DigitFilter(u16* buf,u8 no);
//借用GUI上的数字转字符串函数
extern u8 NumToString(s32 num,char* str);




/**************************************************************
	用户功能函数
***************************************************************/
//设置时间栏的时间值
extern void SetRTCBar(T_STRUCT* time);
//刷新控件显示的时间
extern void UpdateRTCBar(void);
//设置图标的内容
extern void SetIcon(u8);
//更新ADC进度条
extern void UpdateAdc(void);
//更新U盘状态
extern void UpdateUDisk(void);

/**************************************************************
	控件响应函数
***************************************************************/
extern void RspButtonLed(u8 param);

#endif
