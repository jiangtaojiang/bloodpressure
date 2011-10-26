#ifndef WIDGET_H
#define WIDGET_H

typedef u32 BPRO;
typedef u8 EPRO;
typedef u8 SPRO;

/************************
**结构体名:DX_WIDGET
**说明:控件基本特性,每个控件所拥有的共同特性
*************************/
typedef struct _dx_widget
{
	LOC_X x;
	LOC_Y y;
	u16 length;
	u16 height;							//控件的大小和位置
	BPRO base_property;					//基本属性
	EPRO* p_ext_property;				//扩展属性*
	const struct _dx_window *far_win; 	//所属的父窗口(对于窗口,这个一般为NULL)
	void (*response_fun)(u8);			//响应函数(10.1.11)
	COLOR color;						//颜色(对于控件,是字体的颜色,对于WINDOW,是窗体的颜色
}const DX_WIDGET;



/**************************************************************
	各个具体控件头文件
***************************************************************/
#include "BUTTON.h"
#include "EDIT.h"
#include "COMBO.h"
#include "CHECK.h"
#include "PROGRESS.h"

/**************************************************************
**结构体名:WIDGET_PARAM_Q
**说明:响应的消息队列元素.包括控件指针和响应类型
***************************************************************/
typedef struct
{
	DX_WIDGET *widget;
	u8 param;
}WIDGET_PARAM_Q;

/**************************************************************
	控件基础属性表
***************************************************************/

//BASE控件代号,用于控件属性中-位0,1,2,3
#define BASE_PRO_TYPE_MASK		0x0f		/*控件属性低4位决定了这个控件的类型,共支持8种控件,如下:*/
#define BASE_PRO_WINDOW			0			//窗体也属于控件之一 09-11-19
#define BASE_PRO_BUTTON			1
#define BASE_PRO_EDIT			2
#define BASE_PRO_CHECK			3
#define BASE_PRO_STRINGEDIT		4
#define BASE_PRO_COMBO			5			/**/
#define BASE_PRO_PROGRESS		6			//进度条
/*
#define BASE_PRO_RADIO			2
#define BASE_PRO_STATIC			6
#define BASE_PRO_STATICA			7
#define BASE_PRO_PROGRAMBAR		8
#define BASE_PRO_MESSAGEBOX		9
*/

//BASE控件限制标志-位4,5 
#define BASE_PRO_FORBID_FOCUS	(1<<4)		//禁止焦点
#define BASE_PRO_FORBID_TOUCH	(1<<5)		//禁止触摸

//BASE控件字体-位6,7
#define BASE_PRO_FONT_MASK		0xc0
#define BASE_PRO_FONT16			0
#define BASE_PRO_FONT24			(1<<6)
#define BASE_PRO_FONT32			(2<<6)

//BASE控件对齐 -位18,19
#define BASE_PRO_ALIGN_MASK		(0x03<<18)
#define BASE_PRO_ALIGN_DEFAULT	(0<<18)		//默认方式对齐
#define BASE_PRO_ALIGN_LEFT		(1<<18)		//左
#define BASE_PRO_ALIGN_RIGHT	(2<<18)		//右
#define BASE_PRO_ALIGN_MIDDLE	(3<<18)		//中间

/*
//BASE控件配色方案-位20,21,22
#define BASE_PRO_COLOR_STYLE_MASK		(0x07<<20)
#define BASE_PRO_COLOR_STYLE_DEFAULT	0			//默认:防WINDOW 3D色
#define BASE_PRO_COLOR_STYLE_1			(1<<20)		//浮底:BLUE_BT_4;字体:WHITE;3D凹框:BLUE_BT_7;焦点:RED
#define BASE_PRO_COLOR_STYLE_2			(2<<20)
#define BASE_PRO_COLOR_STYLE_3	 		(3<<20)  
#define BASE_PRO_COLOR_STYLE_4	 		(4<<20)  
#define BASE_PRO_COLOR_STYLE_5	 		(5<<20)  
#define BASE_PRO_COLOR_STYLE_6	 		(6<<20)  
#define BASE_PRO_COLOR_STYLE_7	 		(7<<20)  
*/
/**************************************************************
	窗体基础属性表
***************************************************************/
//BASE WIN的属性
#define BASE_PRO_NOT_FILL		(1<<4)		//不需要填充色


/**************************************************************
	控件扩展属性表
***************************************************************/
//EXT控件焦点
#define EXT_PRO_FOCUS			0x01		//焦点
#define EXT_PRO_DISABLE			0x02		//不可用,后期考虑这个是灰色控件,不可用的控件不能触摸也不能获取焦点
#define EXT_PRO_FLASH_EN		0x04		//闪烁EN
#define EXT_PRO_HIDE			0x08		//隐形
#define EXT_PRO_FLASH			0x10		//闪烁位
#define EXT_PRO_FLOCK           0x20        //锁焦
//光标
#define FOCUS_CURSOR_ALL	0xff		//光标状态满,当光标指向一个新的EDIT时,都属于满状态


/**************************************************************
	响应函数的参数
***************************************************************/
#define WIDGET_PARAM_ENTER		0	/*按下确认*/
#define WIDGET_PARAM_EDIT		1	/*用户改写*/
#define WIDGET_PARAM_SET_TMP	2	/*调用SET_TMP*/
#define WIDGET_PARAM_FOCUS_LOST	3	/*焦点离开*/
#define WIDGET_PARAM_FOCUS_GET	4	/*获取焦点*/
#define WIDGET_PARAM_SHOW		5	/*显示,包括窗体的*/
#define WIDGET_PARAM_HIDE		6	/*隐藏,包括窗体的*/
#define WIDGET_PARAM_ACTIVE		7	/*有效,主要用于窗体,而控件,则考虑是焦点状态.有效态为定时调用的任务函数*/

#define WIDGET_PARAM_TDOWN		10	/*触摸按下*/
#define WIDGET_PARAM_TUP		11	/*触摸放开*/





extern DX_WIDGET* pFocus_Widget;	//指向当前焦点控件的指针
extern u8 Focus_Cursor_State;		//当前焦点控件的光标状态此值在以后可以做到闪烁光标符的效果


extern LOC_X GetStringLocXFromFRAME(LOC_X x0,LOC_X length,u8 str_length,char*str);
extern LOC_Y GetStringLocYFromFRAME(LOC_Y y0,LOC_Y height,u8 str_height);
extern u32 GetMiddleFromLine(u32,u32,u32 length);
extern u32 GetStringSize(char*);

extern void __Draw3DFrameA(LOC_X,LOC_Y,LOC_X,LOC_Y);//凹
extern void __Draw3DFrameB(LOC_X,LOC_Y,LOC_X,LOC_Y);//凸
extern void __Draw3DFrameC(LOC_X,LOC_Y,LOC_X,LOC_Y);//凹2 
extern u8 GetNumLen(s32 num);
extern u8 NumToString(s32 num,char* str);
extern u8 NumToStringO8(s32 num,char* str);
extern bool IsWidgetAvailable(DX_WIDGET* the_widget);
extern void ReDrawWidget(DX_WIDGET *the_widget);
extern void GuiWidgetDraw(DX_WIDGET *the_widget);
extern void UpdateWidget(DX_WIDGET* the_widget);
extern void SetWidgetFlash(DX_WIDGET* the_widget,bool);
extern void SetWidgetFLock(DX_WIDGET* the_widget,bool );
extern void SetWidgetHide(DX_WIDGET* the_widget,bool);
extern void SetWidgetDisable(DX_WIDGET* the_widget,bool);
extern DX_WIDGET* SetWidgetFocus(DX_WIDGET* the_widget);
extern void SetNextFocus(DX_WIDGET* the_widget);
extern void KeyToChangeWidget(DX_WIDGET* the_widget,GUIKEY key);
extern void GuiWidgetReponse(DX_WIDGET* the_widget,u8 param);		//控件响应
extern void WidgetFlash(bool flash);

//用于美化的,包装了SAMPOO的函数

extern void GUIRectFill2DA(LOC_X x,LOC_Y y,LOC_X length,LOC_Y height,COLOR color);
extern void GUIRectFill2DB(LOC_X x,LOC_Y y,LOC_X length,LOC_Y height,COLOR color);
//extern void GUIRectFill3D(LOC_X x,LOC_Y y,LOC_X length,LOC_Y height,COLOR color);
//extern void GUIFramFillA3D(LOC_X x,LOC_Y y,LOC_X length,LOC_Y height,COLOR color1,COLOR color2);
extern void GUIHLine3D(LOC_X x,LOC_Y y,LOC_X len,COLOR color);
extern void GUIVLine3D(LOC_X x,LOC_Y y,LOC_Y len,COLOR color);
#endif
