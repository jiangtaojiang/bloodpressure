#ifndef GUIDEF_H
#define GUIDEF_H

#include "color.h"
#include "TFTdriver.h"
#include "key.h"

#ifndef NULL
#define NULL	0
#endif


#define GUIKEY	u8		//键变量
#define MAX_Z	5		//最大允许的拼接窗口数


/**************************************************************
	控件消息
***************************************************************/
//控件声明
#define MESSAGE_WIDGET	u8

#define MESSAGE_NULL					0		//无消息
#define MESSAGE_WIDGET_FOCUS_LOST		1		//失去焦点
#define MESSAGE_WIDGET_FOCUS_GET		2		//获得焦点
#define MESSAGE_WIDGET_EDIT				3		//数值被改变
#define MESSAGE_WIDGET_ENTER			4		//控件被按下ENTER
#define MESSAGE_WIDGET_TOUCH_DOWN		5		//控件被触摸
#define MESSAGE_WIDGET_TOUCH_UP			6		//控件从触摸状态到放开


/**************************************************************
	系统颜色
***************************************************************/
#define COLOR_EDIT_CURSOR	YELLOW

/**************************************************************
	调试功能 以后用的上
***************************************************************/
#define DEBUG_GUI 1

#ifdef DEBUG_GUI

#define gui_asssert_failed(x) 	/*错误处理暂时定义为无,等用户自己跟踪错误所在,可用__FILE__ __LINE__关键字通过串口输出错误位置*/

#endif

#ifdef GuiData_c

#else
extern void UsrInterface(void);
#endif

#endif

