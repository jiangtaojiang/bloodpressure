/********************************************************
**模块:COMBO.c
**功能:COMBOBOX相关的处理
**作者:电子白菜
**更新记录:这里的COMBO和传统PC上的COMBO不同,简单很多,仅用于多选情况.其双选的属性和CHECK差不多
********************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
//#include "combo.h"
#include "window.h"

/**************************************************************
**函数名:__SetOperationalWidget
**功能:获取COMBO的各个常用属性,并赋值到全局变量中,使得之后的操作都针对这个COMBO
**注意事项:仅仅用于提高效率和减少代码长度之用,注意到基本COMBO操作之前,必须调用此函数获取信息 
***************************************************************/
static BPRO __base_property_combo_type;
static BPRO __base_property_combo_style;
static u8 __font_height;//字高
static COLOR __color_string;

static void __SetOperationalWidget(DX_COMBO* the_combo)
{
	__base_property_combo_type=the_combo->widget.base_property&BASE_PRO_COMBO_TYPE_MASK;
	__base_property_combo_style=the_combo->widget.base_property&BASE_PRO_COMBO_STYLE_MASK;

	if((the_combo->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)//16字高
	{
		__font_height=16;
	}
	else if((the_combo->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)//24字高
	{
		__font_height=24;
	}
	else if((the_combo->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)//24字高 
	{
		__font_height=32;
	}

	//自动修复tmp值
	if ( (*(the_combo->p_tmp))>the_combo->max_data-1)//当前项目号不可超max
	{
		*(the_combo->p_tmp)=the_combo->max_data-1;
	}

	//确定背景色
	if ((*(the_combo->widget.p_ext_property))&EXT_PRO_FLASH)			//闪烁
	{
		BG_COLOR=COLOR_FLASH;
	}
	else if((*(the_combo->widget.p_ext_property))&EXT_PRO_FOCUS)		//焦点
	{
		BG_COLOR=COLOR_FOCUS;
	}
	else if((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)		//无效
	{
		BG_COLOR=COLOR_DISABLE;
	}
	else
	{
		if(__base_property_combo_style==BASE_PRO_COMBO_STYLE_3D)			//3D风格用自己规定的底色-SYS色
		    BG_COLOR=COLOR_SYS_FILL_RECT;
		else if(__base_property_combo_style==BASE_PRO_COMBO_STYLE_2D)	//2D用2d色
		{	
			//2D底色使用color_2d
			BG_COLOR=the_combo->color2d;
		}
		else
			BG_COLOR=the_combo->widget.far_win->widget.color;			//NULL则用窗口底色
	}

	//获取字体色
	__color_string=the_combo->widget.color;					//COMBO字色,3D风格除外
}

/**************************************************************
** 函数名:__DebugChecking
** 功能:调试模式下检查Combo是否合理,不合理的一律while(1)死等,用于静态创建的时候测试用户设置问题
** 注意事项:根据DEBUG_GUI宏定义来决定是否编译
***************************************************************/
#ifdef DEBUG_GUI

static void __DebugChecking(DX_COMBO *the_combo)
{
	if(the_combo->p_tmp==NULL)
	{
		gui_asssert_failed(0);
		while(1);
	}
	if (the_combo->max_data==0)//max_data不能为0
	{
		gui_asssert_failed(0);
		while(1);
	}
	if((the_combo->widget.x<MAX_X)&&(the_combo->widget.y<MAX_Y))
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
** 功能:画COMBO的外框
** 注意事项:
***************************************************************/
static void __DrawFrame(DX_COMBO *the_combo)
{
	//如果是隐形,则不需要画框
	if((*(the_combo->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}

	//非BMP的COMBO才可有背景色和外框
	if ((__base_property_combo_type)!=BASE_PRO_COMBO_TYPE_BMP)
	{
		//根据风格画外框
		if (__base_property_combo_style==BASE_PRO_COMBO_STYLE_3D)//3D风格
		{
			__Draw3DFrameB(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height);
		}
		else if (__base_property_combo_style==BASE_PRO_COMBO_STYLE_2D)//2D风格
		{
			TFTRect(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height,COLOR_FRAME_2D);
		}
		else if (__base_property_combo_style==BASE_PRO_COMBO_STYLE_NULL)//NULL风格-
		{
			//NULL风格就是没有框色
		}
	}
}


/**************************************************************
** 函数名 :__Draw
** 功能:刷新COMBO空间中内容
** 注意事项:
***************************************************************/
static void __Draw(DX_COMBO* the_combo)
{
	LOC_X x_str,y_str;
	u8 str_length;
	char* str;
	COLOR* bmp_buf;
	u32 align;

	//如果是隐形,则需要用父窗口背景色填充
	if((*(the_combo->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		TFTRectFill(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height,(the_combo->widget.far_win)->widget.color);
		return;
	}

	//字符串类型
	if (__base_property_combo_type==BASE_PRO_COMBO_TYPE_STRING)
	{
		//填充矩形
		if(__base_property_combo_style==BASE_PRO_COMBO_STYLE_NULL)
		{
			TFTRectFill(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height,BG_COLOR);
		}
		else//如果是2D或者3D模式,则填充留框位
		{
			TFTRectFill(the_combo->widget.x+2,the_combo->widget.y+2,the_combo->widget.length-4,the_combo->widget.height-4,BG_COLOR);
		}
		//获取字体高度
		str_length=__font_height/2;
		
		//获取字符串
		str=(((char**)the_combo->obj)[*(the_combo->p_tmp)]);
		//获取字符串位置
		y_str=GetMiddleFromLine(the_combo->widget.y,the_combo->widget.y+the_combo->widget.height+1,__font_height);
		align=the_combo->widget.base_property&BASE_PRO_ALIGN_MASK;
		//默认中间对齐
		if((align==BASE_PRO_ALIGN_DEFAULT)||(align==BASE_PRO_ALIGN_MIDDLE))
		{
			x_str=GetMiddleFromLine(the_combo->widget.x,the_combo->widget.x+the_combo->widget.length+1,GetStringSize(str)*str_length);
		}
		//左对齐
		else if(align==BASE_PRO_ALIGN_LEFT)
		{
			x_str=the_combo->widget.x+3;
		}
		//右对齐
		else 
		{
			x_str=the_combo->widget.x+the_combo->widget.length-GetStringSize(str)*str_length-2;
		}
		
		if(__font_height==16)
		{
			
			if((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)
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
			if((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)
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
			if((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString32(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString32(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString32(x_str,y_str,str,__color_string,0);
			}
			*/
		}
	}
	//BMP类型
	else if(__base_property_combo_type==BASE_PRO_COMBO_TYPE_BMP)
	{
		bmp_buf=(((COLOR**)the_combo->obj)[*(the_combo->p_tmp)]);
		TFTShowBmp(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height,bmp_buf);
	}
}

/**************************************************************
** 函数名:ReDrawCombo
** 功能:重绘COMBO
** 注意事项:用于一些刷新的重绘,不改变控件自身任何标志
***************************************************************/
void ReDrawCombo(DX_COMBO* the_combo)
{
	//0.获取属性
	__SetOperationalWidget(the_combo);
	//1.画外框
	__DrawFrame(the_combo);
	//2.重绘内容
	__Draw(the_combo);
}

/**************************************************************
** 函数名:GuiComboDraw
** 功能:画COMBO控件
** 注意事项:此函数仅仅用于WINDOW出现的时候显示控件,不作重绘用
**  
***************************************************************/
void GuiComboDraw(DX_COMBO *the_combo)
{

	//0.获取属性
	__SetOperationalWidget(the_combo);
	//1.完整性判断
	__DebugChecking(the_combo);
	//2.HIDE属性处理
	if ((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)
	{
		return;
	}
	//3.内部标志处理
	// ...
	
	//4.画外框
	__DrawFrame(the_combo);

	//5.重绘内容
	__Draw(the_combo);
}


/**************************************************************
** 函数名:SetComboTmp/GetComboTmp
** 功能:设置COMBO的显示值
** 注意事项:COMBO的值是U8类型的,set函数考虑先判断值是否有变化,有变化才更新.
***************************************************************/
void SetComboTmp(DX_COMBO* the_combo,u8 tmp)
{
	if (the_combo!=NULL)
	{
		*(the_combo->p_tmp)=tmp;
		if(IsWidgetAvailable((DX_WIDGET*)the_combo))
		{
			__SetOperationalWidget(the_combo);
			__Draw(the_combo);
		}
	}
}
u8 GetComboTmp(DX_COMBO* the_combo)
{
	if (the_combo!=NULL)
	{
		return *(the_combo->p_tmp);
	}
	else
		return 0;
}

/**************************************************************
** 函数名:UpdateCombo()
** 功能:tmp值和data值的相互转换
** 注意事项:TRUE则data->tmp,并刷新;FALSE tmp=>data 
** 		如果data指针为NULL,则不作更新,如果tmp为NULL,基本不太可能
***************************************************************/
void UpdateCombo(DX_COMBO* the_combo,bool dir)
{
	if(the_combo->p_data==NULL)
		return;

	if(dir==TRUE)
	{
		SetComboTmp(the_combo,*the_combo->p_data);
	}
	else
	{
		*(the_combo->p_data)=*(the_combo->p_tmp);
	}
}

/**************************************************************
** 函数名:KeyToChangeCombo
** 功能:COMBO对按键的反应
** 注意事项:注意KeyToChangeXX系列,都不应对方向键有响应.方向键的响应又系统处理
***************************************************************/
MESSAGE_WIDGET KeyToChangeCombo(DX_COMBO* the_combo,GUIKEY key)
{
	MESSAGE_WIDGET msg_widget=MESSAGE_NULL;
	bool reflash=FALSE;

	if ((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)
		return MESSAGE_NULL;

	//回车
	if (key==KEY_TO_GUI_ENTER)
	{
		msg_widget=MESSAGE_WIDGET_ENTER;
		reflash=FALSE;
	}
	//编辑
	else if((KEY_TO_GUI_0<key)&&(KEY_TO_GUI_9))
	{
		if(++(*(the_combo->p_tmp))>=the_combo->max_data)
		{
			*(the_combo->p_tmp)=0;
		}
		msg_widget=MESSAGE_WIDGET_EDIT;
		reflash=TRUE;
	}

	//重绘
	if(reflash==TRUE)
	{
		//获取基本属性
		__SetOperationalWidget(the_combo);
		//重绘
		__Draw(the_combo);
	}

	return msg_widget;
}