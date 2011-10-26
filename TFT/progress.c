/**************************************************************
** 模块名:progress
** 功能:进度条模块-进度条模块为只读,程序在初始化的时候会自动DISALBE进度条,使得焦点不会往里移动
** 更新记录:2009-11-25 创建此模块,考虑到实际一些项目需求,部分API函数和EDIT的共用,用于显示模块中的数字.  
**  
***************************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
#include "window.h"

/**************************************************************
**函数名:__SetOperationalWidget
**功能:获取PROGRESS的各个常用属性,并赋值到全局变量中,使得之后的操作都针对这个PROGRESS
**注意事项:仅仅用于提高效率和减少代码长度之用,注意到基本progress操作之前,必须调用此函数获取信息 
***************************************************************/
static BPRO __base_property_progress_type;
static BPRO __base_property_progress_dir;
static BPRO __base_property_progress_begin;
static BPRO __base_property_progress_style;	

static u8 __font_height;//引入字高*_*迟点改进
static void __SetOperationalWidget(DX_PROGRESS* the_progress)
{
	__base_property_progress_type=the_progress->widget.base_property&BASE_PRO_PROGRESS_TYPE_MASK;
	__base_property_progress_dir=the_progress->widget.base_property&BASE_PRO_PROGRESS_DIR_MASK;
	__base_property_progress_begin=the_progress->widget.base_property&BASE_PRO_PROGRESS_BEGIN_MASK;
	__base_property_progress_style=the_progress->widget.base_property&BASE_PRO_PROGRESS_STYLE_MASK;
	
	if((the_progress->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)
	{
		__font_height=16;
	}
	else if((the_progress->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)
	{
		__font_height=24;
	}
	else if((the_progress->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)
		__font_height=32;

}

/**************************************************************
** 函数名:__DebugChecking
** 功能:调试模式下检查progress是否合理,不合理的一律while(1)死等,用于静态创建的时候测试用户设置问题
** 注意事项:根据DEBUG_GUI宏定义来决定是否编译
***************************************************************/
#ifdef DEBUG_GUI

static void __DebugChecking(DX_PROGRESS *the_progress)
{
	if(the_progress->p_tmp==NULL)
	{
		gui_asssert_failed(0);
		while(1);
	}
	//背景前景色不能一样
	if(the_progress->bg_color==the_progress->fg_color)
	{
		gui_asssert_failed(0);
		while(1);
	}
	//最大值与最小值之间关系要合理
	if(the_progress->max_data<the_progress->min_data)
	{
		gui_asssert_failed(0);
		while(1);
	}
	//TMP值不能大于最大值
	if((*(the_progress->p_tmp))>the_progress->max_data)
	{
		(*(the_progress->p_tmp))=the_progress->max_data;
	}

	if(!((the_progress->widget.x<MAX_X)&&(the_progress->widget.y<MAX_Y)))
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
static void __DrawFrame(DX_PROGRESS *the_progress)
{
	if(__base_property_progress_style==BASE_PRO_PROGRESS_STYLE_3D)//
	{
		//3D凹框
		__Draw3DFrameA(the_progress->widget.x,the_progress->widget.y,the_progress->widget.length,the_progress->widget.height);
	}
	else if(__base_property_progress_style==BASE_PRO_PROGRESS_STYLE_2D)
	{
		//2D
		TFTRect(the_progress->widget.x,the_progress->widget.y,the_progress->widget.length,the_progress->widget.height,BLACK);
	}
}

/**************************************************************
** 函数名:__Draw
** 功能:重绘
** 注意事项:只根据标志重绘,不改变标志位和信息位.调用前要先获取属性
***************************************************************/
static void __Draw(DX_PROGRESS *the_progress)
{
	u8 percent;
	char str[15];//装载字符串
	u8 str_len;
	LOC_X x_str,x_progress;
	LOC_Y y_str;
	u32 tmp1,tmp2;

	///如果是隐形,则需要用父窗口背景色填充
	if((*(the_progress->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		TFTRectFill(the_progress->widget.x,the_progress->widget.y,the_progress->widget.length,the_progress->widget.height,(the_progress->widget.far_win)->widget.color);
		return;
	}

	tmp1=(*(the_progress->p_tmp))-the_progress->min_data;
	tmp2=the_progress->max_data-the_progress->min_data;

	//获取要显示的字符串
	if(__base_property_progress_type==BASE_PRO_PROGRESS_TYPE_PERCENT)//百分比
	{
		percent=(tmp1*100)/tmp2;
		str_len=NumToString(percent,str);
		str[str_len]='%';
		str[++str_len]='\0';
	}
	else if(__base_property_progress_type==BASE_PRO_PROGRESS_TYPE_NUM)//数值
	{
		if(*(the_progress->p_tmp)>=0)
		{
			str_len=NumToString(*(the_progress->p_tmp),str);
		}
		else
		{
			str_len=NumToString(*(the_progress->p_tmp),&str[1])+1;
			str[0]='-';
		}
		str[++str_len]='\0';
	}
	//计算显示字体的坐标
	y_str=GetStringLocYFromFRAME(the_progress->widget.y,the_progress->widget.height,__font_height);
	x_str=GetMiddleFromLine(the_progress->widget.x,the_progress->widget.x+the_progress->widget.length,str_len*__font_height/2);

	//计算进度条长度
	x_progress=tmp1*(the_progress->widget.length-4)/tmp2;

	//画进度条进度处
	if(x_progress)
	{
		TFTRectFill(the_progress->widget.x+2,the_progress->widget.y+2,x_progress,the_progress->widget.height-4,the_progress->fg_color);
	}
	//画空白处
	TFTRectFill(the_progress->widget.x+2+x_progress,the_progress->widget.y+2,the_progress->widget.length-4-x_progress,the_progress->widget.height-4,the_progress->bg_color);
	//显示字符
	if(__font_height==16)
	{
		TFTShowString16(x_str,y_str,str,the_progress->widget.color,1);
	}
	if(__font_height==24)
	{
		TFTShowString24(x_str,y_str,str,the_progress->widget.color,1);
	}
	else if(__font_height==32)
	{
		//TFTShowString32(x_str,y_str,str,the_progress->widget.color,1);
	}
}

/**************************************************************
** 函数名:ReDrawProgress
** 功能:重绘进度条
** 注意事项:由于progress不可焦点化,所以在画的时候系统自动置DISABLE属性
***************************************************************/
void ReDrawProgress(DX_PROGRESS *the_progress)
{
	//特殊处理DISABLE
	*(the_progress->widget.p_ext_property)|=EXT_PRO_DISABLE;
	//0.获取属性
	__SetOperationalWidget(the_progress);
	//1.画外框
	__DrawFrame(the_progress);
	//2.重绘内容
	__Draw(the_progress);
}

/**************************************************************
** 函数名:GuiProgressDraw
** 功能:画PROGRESS
** 注意事项:由于progress不可焦点化,所以在画的时候系统自动置DISABLE属性
***************************************************************/
void GuiProgressDraw(DX_PROGRESS *the_progress)
{
	//特殊处理DISABLE
	*(the_progress->widget.p_ext_property)|=EXT_PRO_DISABLE;
	//0.获取属性
	__SetOperationalWidget(the_progress);
	//1.完整性判断
	__DebugChecking(the_progress);
	//2.HIDE属性处理
	if ((*(the_progress->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}
	// 3.内部处理
	// ..
	//4.画外框
	__DrawFrame(the_progress);
	//5.重绘内容
	__Draw(the_progress);
}

/**************************************************************
** 函数名:SetProgressTmp/GetProgressTmp
** 功能:设置临时值
** 注意事项:
***************************************************************/
void SetProgressTmp(DX_PROGRESS *the_progress,s32 tmp)
{
	*(the_progress->p_tmp)=tmp;
	if(IsWidgetAvailable((DX_WIDGET*)the_progress))
	{
		__SetOperationalWidget(the_progress);
		__Draw(the_progress);
	}
}

s32 GetProgressTmp(DX_PROGRESS *the_progress)
{
	if(the_progress!=NULL)
	{
		return *(the_progress->p_tmp);
	}
	else 
		return 0;
}

void UpdateProgress(DX_PROGRESS *the_progress,bool dir)
{
	if(dir==TRUE)
	{
		if(the_progress->p_data!=NULL)
		{
			SetProgressTmp(the_progress,*(the_progress->p_data));
		}
	}
	else
	{
		if(the_progress->p_data!=NULL)
		{
			*(the_progress->p_data)=*(the_progress->p_tmp);
		}
	}
}




