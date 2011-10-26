/**************************************************************
** 模块名 :window.c
** 功能 :窗口管理
** 更新记录 :2009-11-19 决定把WINDOW都归类进控件行列.有控件固有的一系列属性
***************************************************************/
#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
#include "window.h"

/**************************************************************
	拼接窗体的构成为一个活跃的pNow_Window和非活跃的Z_Axis,注意两者之间没有交集
***************************************************************/
DX_WINDOW *pNow_Window=NULL;		//指向当前活动窗口,为了提高效率,本变量允许GUI内部全局调用,但一般不应由用户调用
DX_WINDOW *Z_Axis[MAX_Z];				//伪Z轴,DX-GUI不支持层叠窗口,但支持拼接窗口,Z_Axis代表当前可视面上有效的非活动窗体
static bool __widget_ok=FALSE;

/**************************************************************
** 函数名:GuiWindowDraw/Add
** 功能:画窗口
** 注意事项:画窗口的同时也是画控件,根据上一个WINDOW的情况运行响应函数 
**  		10.2.25:在窗体的HIDE响应之前更新pNow_Window,使得意义上,运行旧窗体HIDE函数的时候,当前有效窗体已经指向了新的窗体
**  				故在窗体HIDE函数中,可以获知下一个新窗体是什么.
**  
**  		GuiWindowDraw画出的窗体会成为新的pNow_Window,之前的pNow_Window则调用HIDE函数
** 			GuiWindowAdd 与其不同之处是,它会增加一个拼接窗体,并自动位于Z轴,在Z轴上的窗体都是非活动窗体
***************************************************************/
void GuiWindowDraw(DX_WINDOW* the_window)
{
	DX_WINDOW* old_window;
	u8 i;
	bool focus_exit=FALSE;

	if(the_window!=NULL)
	{
		old_window=pNow_Window;
		pNow_Window=the_window;				//刷新当前窗口指针
	}
	else
		return;

	//上一个窗口的最小化调用hide:注意如果重复调用GuiWindowDraw画同一个窗体,HIDE函数不生效
	__widget_ok=FALSE;
	if((old_window!=NULL)&&(old_window!=pNow_Window))
	{
		GuiWidgetReponse((DX_WIDGET*)old_window,WIDGET_PARAM_HIDE);
	}

	
	pFocus_Widget=NULL;					//初始化焦点控件指针
	Focus_Cursor_State=FOCUS_CURSOR_ALL;//初始化光标
	BG_COLOR=the_window->widget.color;		//初始化背景色


	//画窗体
	if(!(the_window->widget.base_property&BASE_PRO_NOT_FILL))
	{
		TFTRectFill(the_window->widget.x,the_window->widget.y,the_window->widget.length,the_window->widget.height,BG_COLOR);
	}

	//调用show函数
	GuiWidgetReponse((DX_WIDGET*)the_window,WIDGET_PARAM_SHOW);
		
	/**************************************************************
	自动修复焦点机制 
	0.清除当前 pFocus_Widget
	1.确定首个焦点号 
	2.余下的焦点复位一次扩展属性的 EXT_PRO_FOCUS位 
	3.这个动作只在第一次画WINDOW的时候调用 (修复后,WINDOW的扩展属性位EXT_PRO_FOCUS要置位)
	4.如果没找到焦点,则人为找一个可置焦点的控件置焦点 
	5.如果窗体没有焦点,保持pFocus_Widget为NULL 
	***************************************************************/
	pFocus_Widget=NULL;
	//之前没自动初始化焦点
	if(!((*(the_window->widget.p_ext_property))&EXT_PRO_FOCUS))
	{
		for(i=0;i<the_window->max_widget;i++)
		{
			//确定焦点的编号
			if(*(the_window->p_child[i]->p_ext_property)&EXT_PRO_FOCUS)
			{
				pFocus_Widget=the_window->p_child[i];
				*(the_window->p_focu_no)=i;
				focus_exit=TRUE;
				break;
			}
		}

		if(focus_exit==TRUE)
		{
			for(i+=1;i<the_window->max_widget;i++)
			{
				//修复多焦点问题(实际上是把余下的控件都复位一下ext_property属性位EXT_PRO_FOCUS)
				*(the_window->p_child[i]->p_ext_property)&=~EXT_PRO_FOCUS;
			}
		}
		else//没有初始化焦点,则人为寻找首个可置焦点的控件
		{
			for(i=0;i<the_window->max_widget;i++)
			{
				if(!(the_window->p_child[i]->base_property&BASE_PRO_FORBID_FOCUS))
				{
					pFocus_Widget=the_window->p_child[i];
					*(the_window->p_child[i]->p_ext_property)|=EXT_PRO_FOCUS;
					*(the_window->p_focu_no)=i;
					break;
				}
			}
		}

		//修复完毕,以后不用再修复了
		*(the_window->widget.p_ext_property)|=EXT_PRO_FOCUS;
	}
	else //之前已经初始化了焦点
	{
		i=*(the_window->p_focu_no);
		pFocus_Widget=the_window->p_child[i];
	}
	//画控件
	for(i=0;i<the_window->max_widget;i++)
	{
		GuiWidgetDraw(the_window->p_child[i]);
	}
	__widget_ok=TRUE;
}
void GuiWindowAdd(DX_WINDOW* the_window)
{
	u8 i;

	//1.不允许加上当前的已经活跃的窗体
	if(pNow_Window==the_window)
		return;

	//2.这里不检查窗体是否有重叠了,实际使用中用户自己注意
	//..

	//3.加入Z轴
	for(i=0;i<MAX_Z;i++)
	{
		if(Z_Axis[i]!=NULL)
		{
			//不需要重复加入
			if(Z_Axis[i]==the_window)
			{
				return;
			}
		}
		else
		{
			Z_Axis[i]=the_window;
			break;
		}
	}
	
	if(i==MAX_Z)//Z轴满了则不再加入
	{
		return;
	}

	BG_COLOR=the_window->widget.color;		//初始化背景色


	//画窗体
	if(!(the_window->widget.base_property&BASE_PRO_NOT_FILL))
	{
		TFTRectFill(the_window->widget.x,the_window->widget.y,the_window->widget.length,the_window->widget.height,BG_COLOR);
	}
	
	__widget_ok=FALSE;
	//调用show函数
	GuiWidgetReponse((DX_WIDGET*)the_window,WIDGET_PARAM_SHOW);
		
	/**************************************************************
	拼接窗体中不允许有焦点控件,这里会自动修复掉,修复完的窗体,其EXT_PRO_FOCUS会被清零
	***************************************************************/
	for(i=0;i<the_window->max_widget;i++)
	{
		*(the_window->p_child[i]->p_ext_property)&=~EXT_PRO_FOCUS;	
	}
	*(the_window->widget.p_ext_property)&=~EXT_PRO_FOCUS;
	
	//画控件
	for(i=0;i<the_window->max_widget;i++)
	{
		GuiWidgetDraw(the_window->p_child[i]);
	}
	__widget_ok=TRUE;
}

void GuiWindowDec(DX_WINDOW* the_window)
{
	u8 i,j;

	//允许消除当前的已经活跃的窗体
	if(pNow_Window==the_window)
	{
		pNow_Window=NULL;
	}
	//或Z轴中剔除
	else
	{
		for(i=0;i<MAX_Z;i++)
		{
			if(Z_Axis[i]==the_window)
			{
				break;
			}
		}
		
		if(i==MAX_Z)//整个Z轴都没有,则没有可消除的窗体
		{
			return;
		}
		//调整Z轴
		for(j=i+1;j<MAX_Z;j++,i++)
		{
			if(Z_Axis[j]!=NULL)
			{
				Z_Axis[i]=Z_Axis[j];
			}
			else
			{
				Z_Axis[i]=NULL;
			}
				break;
		}
	}
	BG_COLOR=the_window->widget.color;		//初始化背景色

	//填充色,暂时填充窗体自身颜色,似乎没必要填充,之后在实际应用中再修改*_*
	//TFTRectFill(the_window->widget.x,the_window->widget.y,the_window->widget.length,the_window->widget.height,BG_COLOR);

	//调用hide函数
	GuiWidgetReponse((DX_WIDGET*)the_window,WIDGET_PARAM_HIDE);
}
/**************************************************************
** 函数名:GetNowWindow
** 功能:获取当前界面
** 注意事项:返回当前界面的指针.如果是GUI底层内,请直接引用pNow_Window
***************************************************************/

DX_WINDOW* GetNowWin(void)
{
	return pNow_Window;
}


/**************************************************************
** 函数名:IsWindowAvailable
** 功能:判断窗体是否可用
** 注意事项:所谓可用,就是是pNow_Window或者在Z轴上
***************************************************************/
bool IsWindowAvailable(DX_WINDOW* the_window)
{
	u8 i;
	if(the_window==pNow_Window)
	{
		return TRUE;
	}

	for(i=0;i<MAX_Z;i++)
	{
		if(Z_Axis[i]==the_window)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**************************************************************
** 函数名:IsWidgetAvailable
** 功能:判断控件是否可用
** 注意事项:所谓可用,就是自身的WIN可用并且已描绘并且非HIDE
**************************************************************/
bool IsWidgetAvailable(DX_WIDGET* the_widget)
{
	if((IsWindowAvailable(the_widget->far_win)==TRUE)&&(! ((*(the_widget->p_ext_property))&EXT_PRO_HIDE))&&(__widget_ok==TRUE))
	{
		return TRUE;
	}
	else
		return FALSE;
}


/**************************************************************
** 函数名 :UpdateWindow
** 功能 :tmp值和data值的相互转换
** 注意事项:TRUE则data->tmp,并刷新;FALSE tmp=>data 
***************************************************************/
void UpdateWindowData(DX_WINDOW* the_window,bool dir)
{
	u8 i;
	u8 widget_type;
	
	for(i=0;i<the_window->max_widget;i++)
	{
		widget_type=the_window->p_child[i]->base_property&BASE_PRO_TYPE_MASK; //获取控件类型
		switch(widget_type)
		{
			case BASE_PRO_EDIT:
				UpdateEdit((DX_EDIT*)(the_window->p_child[i]),dir);
				break;
			case BASE_PRO_COMBO:
				UpdateCombo((DX_COMBO*)(the_window->p_child[i]),dir);
				break;
			case BASE_PRO_CHECK:
				UpdateCheck((DX_CHECK*)(the_window->p_child[i]),dir);
				break;
			//进度栏暂时忽略
			//case BASE_PRO_PROGRESS
			default:
				break;

		}
	}
}