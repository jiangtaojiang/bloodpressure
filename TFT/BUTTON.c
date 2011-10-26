/********************************************************
**模块:BUTTON.c
**功能:键盘相关的处理
**作者:电子白菜
**更新记录:
********************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "TFTDriver.h"
#include "widget.h"
#include "window.h"

/**************************************************************
**函数名:__SetOperationalWidget
**功能:获取Button的各个常用属性,并赋值到全局变量中,使得之后的操作都针对这个BUTTON
**注意事项:仅仅用于提高效率和减少代码长度之用,注意到基本Button操作之前,必须调用此函数获取信息 
***************************************************************/
static BPRO __base_property_button_type;
//static BPRO __base_property_button_style;//暂时无用
static u8 __font_height;//字高

static void __SetOperationalWidget(DX_BUTTON* the_button)
{
	__base_property_button_type=the_button->widget.base_property&BASE_PRO_BUTTON_TYPE_MASK;
//	__base_property_button_style=the_button->widget.base_property&BASE_PRO_BUTTON_STYLE_MASK;

	if((the_button->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)//16字高
	{
		__font_height=16;
	}
	else if((the_button->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)//24字高
	{
		__font_height=24;
	}
	else if((the_button->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)//32字高 
	{
		__font_height=32;
	}
	
	//BUTTON的风格比较单一,所以不需要在这里特别设置颜色值
}

/**************************************************************
** 函数名:__DebugChecking
** 功能:调试模式下检查Check是否合理,不合理的一律while(1)死等,用于静态创建的时候测试用户设置问题
** 注意事项:根据DEBUG_GUI宏定义来决定是否编译
***************************************************************/
#ifdef DEBUG_GUI

static void __DebugChecking(DX_BUTTON *the_button)
{
	if((the_button->widget.x<MAX_X)&&(the_button->widget.y<MAX_Y))
	{
		return;
	}
	else
	{
		gui_asssert_failed(0);
		while(1);
	}
}
#else

#define __DebugChecking(x) 

#endif

/**************************************************************
** 函数名:__DrawFrame
** 功能:画BUTTON的外框
** 注意事项:暂时BUTTON的画框在__Draw上完成,因为BUTTON某些变化,框也会有改变
***************************************************************/
static void __DrawFrame(DX_BUTTON* the_button)
{
	//如果是隐形的,则不需要画框
	if((*(the_button->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}
}
/**************************************************************
** 函数名:__Draw
** 功能:重绘按键
** 注意事项:
***************************************************************/
static void __Draw(DX_BUTTON *the_button)
{
	u8 str_length;
	LOC_X x_str,y_str;
	//如果是隐形,则需要用父窗口背景色填充
	if((*(the_button->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		TFTRectFill(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height,(the_button->widget.far_win)->widget.color);
		return;
	}
	//BMP模式
	if(__base_property_button_type==BASE_PRO_BUTTON_TYPE_BMP)
	{
		if ((*(the_button->widget.p_ext_property))&EXT_PRO_FOCUS) //焦点(分按下与弹起)
		{
			//按下
			if ((*(the_button->p_self_property))&SELF_PRO_PUSH_DOWN)
			{	
				if(the_button->bmp_push!=NULL)
				{
					TFTShowBmp(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height,the_button->bmp_push);
				}
			}
			//弹起
			else
			{	
				//焦点按键BMP
				if(the_button->bmp_focus!=NULL)
				{
					TFTShowBmp(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height,the_button->bmp_focus);
				}
				//焦点按键BMP为正常态的反色
				else
				{
					//TFTShowBmp(x0,y0,the_button->widget.length,the_button->widget.height,the_button->bmp_normal);//*_*这要要进行反色处理
				}
			}
		}
		else
		{
			if(the_button->bmp_normal!=NULL)
			{
				TFTShowBmp(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height,the_button->bmp_normal);
			}
		}
	}
	//字符按键
	else if(__base_property_button_type==BASE_PRO_BUTTON_TYPE_STRING)
	{

		//字符键盘不能太小,否则字体不能显示.这里不作判断,用户自己注意
		//画背景色-
		if ((*(the_button->widget.p_ext_property))&EXT_PRO_FLASH)//&&(闪烁标志有效)	//闪烁态
		{
			TFTRectFill(the_button->widget.x+1,the_button->widget.y+1,the_button->widget.length-2,the_button->widget.height-2,COLOR_FLASH);
			BG_COLOR=COLOR_FLASH;
			goto frame_draw;
		}
		else if ((*(the_button->widget.p_ext_property))&EXT_PRO_FOCUS) //焦点(分按下与弹起)
		{
			TFTRectFill(the_button->widget.x+2,the_button->widget.y+2,the_button->widget.length-4,the_button->widget.height-4,COLOR_FOCUS);
			BG_COLOR=COLOR_FOCUS;
		}
		else if ((*(the_button->widget.p_ext_property))&EXT_PRO_DISABLE)	 //disable
		{
			TFTRectFill(the_button->widget.x+2,the_button->widget.y+2,the_button->widget.length-4,the_button->widget.height-4,COLOR_DISABLE);
			BG_COLOR=COLOR_BUTTON_BG_UNFOCUS;
		}
		else									//静态
		{
			if ((*(the_button->p_self_property))&SELF_PRO_PUSH_DOWN)
				TFTRectFill(the_button->widget.x+2,the_button->widget.y+2,the_button->widget.length-4,the_button->widget.height-4,COLOR_BUTTON_BG_UNFOCUS);
			else
				TFTRectFill(the_button->widget.x+2,the_button->widget.y+2,the_button->widget.length-4,the_button->widget.height-4,COLOR_BUTTON_BG_UNFOCUS);
			BG_COLOR=COLOR_BUTTON_BG_UNFOCUS;
		}

		frame_draw:

		str_length=__font_height/2;
		x_str=GetStringLocXFromFRAME(the_button->widget.x,the_button->widget.length,str_length,the_button->str);
		y_str=GetStringLocYFromFRAME(the_button->widget.y,the_button->widget.height,str_length*2);
		
		//画内框
		if ((*(the_button->p_self_property))&SELF_PRO_PUSH_DOWN)	 //按下状态
		{
			//画中框
			__Draw3DFrameC(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height);
			x_str++;
			y_str++;
		}
		else									//弹起状态
		{
			//凸框
			__Draw3DFrameB(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height);
		}

		//写字
		if(__font_height==16)
		{
			
			if ((*(the_button->widget.p_ext_property))&EXT_PRO_DISABLE)	 //disable,这里要写透明字,为了重叠出DISABLE的效果
			{
				TFTShowString16(x_str+1,y_str+1,the_button->str,COLOR_STRING_DISABLE1,0);
				TFTShowString16(x_str,y_str,the_button->str,COLOR_STRING_DISABLE2,1);
			}
			else
			 	TFTShowString16(x_str,y_str,the_button->str,the_button->widget.color,0);
			
		}
		if (__font_height==24)
		{
			if ((*(the_button->widget.p_ext_property))&EXT_PRO_DISABLE)	 //disable,这里要写透明字,为了重叠出DISABLE的效果
			{
				TFTShowString24(x_str+1,y_str+1,the_button->str,COLOR_STRING_DISABLE1,0);
				TFTShowString24(x_str,y_str,the_button->str,COLOR_STRING_DISABLE2,1);
			}
			else
				TFTShowString24(x_str,y_str,the_button->str,the_button->widget.color,0);
		}
		else if (__font_height==32)
		{
		/*
			if ((*(the_button->widget.p_ext_property))&EXT_PRO_DISABLE)//无效的BUTTON
			{
				TFTShowString32(x_str+1,y_str+1,the_button->str,COLOR_STRING_DISABLE1,0);
				TFTShowString32(x_str,y_str,the_button->str,COLOR_STRING_DISABLE2,1);
			}
			else
				TFTShowString32(x_str,y_str,the_button->str,the_button->widget.color,0);
		*/
		}
	}
}

/**************************************************************
** 函数名:ReDrawButton
** 功能:重绘BUTTON
** 注意事项:用于一些刷新的重绘,不改变控件自身任何标志
***************************************************************/
void ReDrawButton(DX_BUTTON* the_button)
{
	//0.获取属性
	__SetOperationalWidget(the_button);
	//1.画外框
	__DrawFrame(the_button);
	//2.重绘内容
	__Draw(the_button);
}

/**************************************************************
**函数名:ButtonDraw
**功能:画按键
**注意事项:此函数仅仅用于WINDOW出现的时候显示控件,不作重绘用
***************************************************************/
void GuiButtonDraw(DX_BUTTON *the_button)
{   
	//0.获取属性
	__SetOperationalWidget(the_button);

	//1.完整性判断
	__DebugChecking(the_button);

	//2.处理无效和隐身控件
	if ((*(the_button->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}

	//3.内部处理
	//...

	//4.画外框
	__DrawFrame(the_button);

	//5.重绘
	__Draw(the_button);
}

/**************************************************************
** 函数名:UpdateButton
** 功能:更新一次Button
** 注意事项:更新控件一般是重绘一次 *_*此函数暂时似乎没必要
***************************************************************/
void UpdateButton(DX_BUTTON *the_button)
{
	//获取属性
	__SetOperationalWidget(the_button);
	//画外框
	//..
	//画内容
	__Draw(the_button);
}

/**************************************************************
** 函数名:KeyToChangeButton
** 功能:按键对BUTTON的影响
** 注意事项:一般来说,只有回车键对BUTTON有意义 
** 			注意KeyToChangeXX系列,都不应对方向键有响应.方向键的响应又系统处理
***************************************************************/
MESSAGE_WIDGET KeyToChangeButton(DX_BUTTON* the_button,GUIKEY key)
{
	u8 ext_pro_button;
	if (key==KEY_TO_GUI_ENTER)
	{
		ext_pro_button=(*(the_button->widget.p_ext_property));
		if (!(ext_pro_button&EXT_PRO_DISABLE))
			return MESSAGE_WIDGET_ENTER;
	}
	return MESSAGE_NULL;
}
