/********************************************************
**模块:CHECK.c
**功能:CHECKBOX相关的处理
**作者:电子白菜
**更新记录:
********************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
#include "window.h"

/**************************************************************
**函数名:__SetOperationalWidget
**功能:获取CHECK的各个常用属性,并赋值到全局变量中,使得之后的操作都针对这个CHECK
**注意事项:仅仅用于提高效率和减少代码长度之用,注意到基本CHECK操作之前,必须调用此函数获取信息 
***************************************************************/
static BPRO __base_property_check_type;
static BPRO __base_property_check_style;
static u8 __font_height;//字高
static COLOR __color_string;

static void __SetOperationalWidget(DX_CHECK* the_check)
{
	__base_property_check_type=the_check->widget.base_property&BASE_PRO_CHECK_TYPE_MASK;
	__base_property_check_style=the_check->widget.base_property&BASE_PRO_CHECK_STYLE_MASK;
	
	if((the_check->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)//16字高
	{
		__font_height=16;
	}
	else if((the_check->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)//24字高
	{
		__font_height=24;
	}
	else if((the_check->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)//24字高 
	{
		__font_height=32;
	}
	
	//确定背景色
	if ((*(the_check->widget.p_ext_property))&EXT_PRO_FLASH)			//闪烁
	{
		BG_COLOR=COLOR_FLASH;
	}
	else if((*(the_check->widget.p_ext_property))&EXT_PRO_FOCUS)		//焦点
	{
		BG_COLOR=COLOR_FOCUS;
	}
	else if((*(the_check->widget.p_ext_property))&EXT_PRO_DISABLE)		//无效
	{
		BG_COLOR=COLOR_DISABLE;
	}
	else
	{
		if(__base_property_check_type==BASE_PRO_CHECK_TYPE_HOOK)		//钩模式,底色必须是SYS色
		{
			BG_COLOR=COLOR_SYS_FILL_RECT;
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_3D)	//3D风格
		{    
			BG_COLOR=the_check->color2d;
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_2D)	//2D用2d色
		{	
			BG_COLOR=the_check->color2d;
		}
		else
			BG_COLOR=the_check->widget.far_win->widget.color;			//NULL则用窗口底色
	}

	//获取字体色
	__color_string=the_check->widget.color;					//CHECK字色,3D风格除外
}

/**************************************************************
** 函数名:__DebugChecking
** 功能:调试模式下检查Check是否合理,不合理的一律while(1)死等,用于静态创建的时候测试用户设置问题
** 注意事项:根据DEBUG_GUI宏定义来决定是否编译
***************************************************************/
#ifdef DEBUG_GUI

static void __DebugChecking(DX_CHECK *the_check)
{
	if(the_check->p_tmp==NULL)
	{
		gui_asssert_failed(0);
		while(1);
	}
	if((the_check->widget.x<MAX_X)&&(the_check->widget.y<MAX_Y))
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
** 功能:画外框
** 注意事项:调用前要先获取属性
***************************************************************/
static void __DrawFrame(DX_CHECK *the_check)
{
	LOC_X length;
	LOC_Y height;

	///如果是隐形,则不需要画框
	if((*(the_check->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}
	if(__base_property_check_type==BASE_PRO_CHECK_TYPE_HOOK)//钩模式(这个模式也有2D,3D,NULL之分)
	{
		//画钩模式的框-钩方式,框大小由字体高度定
		if(__font_height==16)
		{
			 length=14;
		     height=14;
		}
		else if(__font_height==24)
		{
			length=20;
			height=20;
		}
		else if(__font_height==32)
		{
			length=28;
			height=28;
		}

		if(__base_property_check_style==BASE_PRO_CHECK_STYLE_2D)//2D
		{
			TFTRect(the_check->widget.x,the_check->widget.y,length,height,COLOR_FRAME_2D);
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_3D)//3D
		{
			__Draw3DFrameA(the_check->widget.x,the_check->widget.y,length,height);
		}
	}
	//BMO模式没有框
	//..
	else if(__base_property_check_type==BASE_PRO_CHECK_TYPE_STRING)//字符模式
	{
		//画框
		if(__base_property_check_style==BASE_PRO_CHECK_STYLE_2D)	//2D
		{
			TFTRect(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height,BLACK);
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_3D)//3D
		{
			__Draw3DFrameB(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height);
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_NULL)//无框-无框风格一般用于显示
		{
		}
	}
}

/**************************************************************
** 函数名:__Draw
** 功能:重绘CHECK
** 注意事项:只根据标志重绘,不改变标志位和信息位.调用前要先获取属性
***************************************************************/
static void __Draw(DX_CHECK *the_check)
{
	COLOR *bmp;
	char *str;
	LOC_X x_str;
	LOC_Y y_str;
	///如果是隐形,则需要用父窗口背景色填充
	if((*(the_check->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		if(__base_property_check_type==BASE_PRO_CHECK_TYPE_HOOK)//钩模式
		{
			if(__font_height==16)
			{
			 	TFTRectFill(the_check->widget.x,the_check->widget.y,14,14,(the_check->widget.far_win)->widget.color);
			}
			else if(__font_height==24)
			{
				TFTRectFill(the_check->widget.x,the_check->widget.y,20,20,(the_check->widget.far_win)->widget.color);
			}
			else if(__font_height==32)
			{
				TFTRectFill(the_check->widget.x,the_check->widget.y,28,28,(the_check->widget.far_win)->widget.color);
			}
		}
		else
			TFTRectFill(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height,(the_check->widget.far_win)->widget.color);
		return;
	}

	if(__base_property_check_type==BASE_PRO_CHECK_TYPE_HOOK)//钩类型-框大小由字体决定
	{
		//画钩模式的框-钩方式,填充大小由字体高度定
		if(__font_height==16)
		{
			TFTRectFill(the_check->widget.x+2,the_check->widget.y+2,14-4,14-4,BG_COLOR);
		}
		else if(__font_height==24)
		{
			TFTRectFill(the_check->widget.x+2,the_check->widget.y+2,20-4,20-4,BG_COLOR);
		}
		else if(__font_height==32)
		{
			TFTRectFill(the_check->widget.x+2,the_check->widget.y+2,28-4,28-4,BG_COLOR);
		}
		
		//画钩
		if((*(the_check->p_tmp))==TRUE)
		{
			if(__font_height==16)
			{
				x_str=the_check->widget.x+3;
				y_str=the_check->widget.y+5;
				TFTVLine(x_str++,y_str++,3,BLACK);
				TFTVLine(x_str++,y_str++,3,BLACK);

				TFTVLine(x_str++,y_str--,3,BLACK);
				TFTVLine(x_str++,y_str--,3,BLACK);
				TFTVLine(x_str++,y_str--,3,BLACK);
				TFTVLine(x_str++,y_str--,3,BLACK);
				TFTVLine(x_str++,y_str,3,BLACK);
			}
			else if(__font_height==24)
			{
				x_str=the_check->widget.x+4;
				y_str=the_check->widget.y+7;
				TFTVLine(x_str++,y_str++,4,BLACK);
				TFTVLine(x_str++,y_str++,4,BLACK);
				TFTVLine(x_str++,y_str++,4,BLACK);
				TFTVLine(x_str++,y_str++,4,BLACK);

				TFTVLine(x_str++,y_str--,4,BLACK);
				TFTVLine(x_str++,y_str--,4,BLACK);
				TFTVLine(x_str++,y_str--,4,BLACK);
				TFTVLine(x_str++,y_str--,4,BLACK);
				TFTVLine(x_str++,y_str--,4,BLACK);
				TFTVLine(x_str++,y_str--,4,BLACK);
				TFTVLine(x_str++,y_str,4,BLACK);
			}
			else if (__font_height==32)
			{
				x_str=the_check->widget.x+7;
				y_str=the_check->widget.y+10;
				TFTVLine(x_str++,y_str++,6,BLACK);
				TFTVLine(x_str++,y_str++,6,BLACK);
				TFTVLine(x_str++,y_str++,6,BLACK);
				TFTVLine(x_str++,y_str++,6,BLACK);
				TFTVLine(x_str++,y_str++,6,BLACK);
				TFTVLine(x_str++,y_str--,6,BLACK);

				TFTVLine(x_str++,y_str--,6,BLACK);
				TFTVLine(x_str++,y_str--,6,BLACK);
				TFTVLine(x_str++,y_str--,6,BLACK);
				TFTVLine(x_str++,y_str--,6,BLACK);
				TFTVLine(x_str++,y_str--,6,BLACK);
				TFTVLine(x_str++,y_str--,6,BLACK);
				TFTVLine(x_str++,y_str--,6,BLACK);
				TFTVLine(x_str++,y_str,6,BLACK);

			}			 		 
		}				 		 
	}					 		 
	else if(__base_property_check_type==BASE_PRO_CHECK_TYPE_BMP)//BMP类型
	{
		if((*(the_check->p_tmp))==TRUE)
			bmp=((COLOR**)(the_check->obj))[0];
		else
			bmp=((COLOR**)(the_check->obj))[1];
		TFTShowBmp(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height,bmp);
	}
	else
	{
			//填充
			if(__base_property_check_style==BASE_PRO_CHECK_STYLE_2D)	//2D
			{
				TFTRectFill(the_check->widget.x+2,the_check->widget.y+2,the_check->widget.length-4,the_check->widget.height-4,BG_COLOR);
			}
			else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_3D)//3D
			{
				TFTRectFill(the_check->widget.x+2,the_check->widget.y+2,the_check->widget.length-4,the_check->widget.height-4,BG_COLOR);
			}
			else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_NULL)//无框-无框风格一般用于显示
			{
				TFTRectFill(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height,BG_COLOR);
			}
	
			if((*(the_check->p_tmp))==TRUE)
				str=((char**)(the_check->obj))[0];
			else
				str=((char**)(the_check->obj))[1];
			
			x_str=GetStringLocXFromFRAME(the_check->widget.x,the_check->widget.length,__font_height/2,str);
			y_str=GetMiddleFromLine(the_check->widget.y,the_check->widget.y+the_check->widget.height,__font_height);
	
			//写字
			if(__font_height==16)
			{
				
				if((*(the_check->widget.p_ext_property))&EXT_PRO_DISABLE)
				{
					TFTShowString16(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
					TFTShowString16(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
				}
				else
				{	
					TFTShowString16(x_str,y_str,str,__color_string,0);
				} 
				 
			}
			if (__font_height==24)
			{
				if((*(the_check->widget.p_ext_property))&EXT_PRO_DISABLE)
				{
					TFTShowString24(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
					TFTShowString24(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
				}
				else
				{	
					TFTShowString24(x_str,y_str,str,__color_string,0);
				}
			}
			else if (__font_height==32)
			{
				/*
				if((*(the_check->widget.p_ext_property))&EXT_PRO_DISABLE)
				{
					TFTShowString32(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
					TFTShowString32(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
				}
				else
					TFTShowString32(x_str,y_str,str,__color_string,0);
				*/
			}
	}
}

/**************************************************************
** 函数名:ReDrawCheck
** 功能:重绘控件
** 注意事项:用于一些刷新的重绘,不改变控件自身任何标志
***************************************************************/
void ReDrawCheck(DX_CHECK *the_check)
{
	//0.获取属性
	__SetOperationalWidget(the_check);
	//1.画外框
	__DrawFrame(the_check);
	//2.重绘内容
	__Draw(the_check);

}

/**************************************************************
** 函数名:GuiCheckDraw
** 功能:显示CHECK控件
** 注意事项:此函数仅仅用于WINDOW出现的时候显示控件,不作重绘用
***************************************************************/
void GuiCheckDraw(DX_CHECK *the_check)
{
	//0.获取属性
	__SetOperationalWidget(the_check);
	//1.完整性判断
	__DebugChecking(the_check);
	//2.HIDE属性处理
	if ((*(the_check->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}
	//3.内部处理
	// ..
	
	//4.画外框
	__DrawFrame(the_check);
	
	//5.重绘
	__Draw(the_check);
}

/**************************************************************
** 函数名:SetCheckTmp/GetCheckTmp
** 功能:设置/获取Check的显示值
** 注意事项:
***************************************************************/
void SetCheckTmp(DX_CHECK* the_check,bool tmp)
{
	*(the_check->p_tmp)=tmp;
	if(IsWidgetAvailable((DX_WIDGET*)the_check))
	{
		__SetOperationalWidget(the_check);
		__Draw(the_check);
	}
}

bool GetCheckTmp(DX_CHECK* the_check)
{
	if(the_check!=NULL)
	{
		return(*(the_check->p_tmp));
	}
	else
		return FALSE;
}

/**************************************************************
** 函数名:UpdateCheck()
** 功能:tmp值和data值的相互转换
** 注意事项:TRUE则data->tmp,并刷新;FALSE tmp=>data 
** 		如果data指针为NULL,则不作更新,如果tmp为NULL,基本不太可能
***************************************************************/
void UpdateCheck(DX_CHECK* the_check,bool dir)
{
	if(the_check->p_data==NULL)
		return;

	if(dir==TRUE)
	{
		SetCheckTmp(the_check,*the_check->p_data);
	}
	else
	{
		*(the_check->p_data)=*(the_check->p_tmp);
	}
}

/**************************************************************
** 函数名:KeyToChangeCheck
** 功能:按键对CHECK的改变
** 注意事项: *_*本函数还负责了重绘控件,可考虑不在这里重绘
** 			注意KeyToChangeXX系列,都不应对方向键有响应.方向键的响应又系统处理
***************************************************************/
MESSAGE_WIDGET KeyToChangeCheck(DX_CHECK* the_check,GUIKEY key)
{
	MESSAGE_WIDGET msg_widget=MESSAGE_NULL;
	bool reflash=FALSE;

	//DISABLE控件不处理
	//if ((*(the_check->widget.p_ext_property))&EXT_PRO_DISABLE)
	//	return MESSAGE_NULL;

	//回车
	if (key==KEY_TO_GUI_ENTER)
	{
		msg_widget=MESSAGE_WIDGET_ENTER;
		reflash=FALSE;
	}
	//编辑
	else if((KEY_TO_GUI_0<key)&&(KEY_TO_GUI_9))
	{
		if(*(the_check->p_tmp)==TRUE)
		{
			*(the_check->p_tmp)=FALSE;
		}
		else
		{
			*(the_check->p_tmp)=TRUE;
		}
		msg_widget=MESSAGE_WIDGET_EDIT;
		reflash=TRUE;
	}

	//重绘
	if(reflash==TRUE)
	{
		//获取基本属性
		__SetOperationalWidget(the_check);
		//重绘
		__Draw(the_check);
	}

	return msg_widget;
}

