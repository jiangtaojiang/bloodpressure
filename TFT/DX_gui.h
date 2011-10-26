/**************************************************************
模块:tiny BC GUI:微型的白菜版GUI
功能:不支持层叠窗口,合适只需要单个窗体的带控件的GUI,支持触摸屏和键盘移动 ,窗体和控件的背景色为系统预编译.可进行宏修改
***************************************************************/

#ifndef BC_GUI_H
#define BC_GUI_H

#include "GuiDef.h"
/**************************************************************
	资源相关
***************************************************************/
#include "widget.h"
#include "button.h"
#include "edit.h"
#include "combo.h"
#include "check.h"
#include "window.h"

//暂时没用的
#define WIDGET_PARAM_Q_SIZE	100	/*定义控件响应队列的最大响应数量,建议至少要大于用户一个窗体拥有的最大控件数*/


/*************************************************************
	触摸相关
*************************************************************/
#include "dx_touch.h"



extern void GuiInit(void);
extern void KeyToGui(GUIKEY key);
extern void GuiSysTick20ms(void);

extern u8 TouchAdj(void);
extern bool NeedTouchAdj(void);

extern void ManageWPQ(void);					//响应队列的管理
#endif
