#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "GuiDef.h"
#include "TFTdriver.h"
#include "TFTHal.h"
#include "DX_GUI.h"
#include "WIDGET.h"
#include "touch.h"
extern void Lcd_Init(void);


void GuiInit(void)
{
	BLIGHT_ON;
//    BLIGHT_OFF;
	Lcd_Init();
}

/**************************************************************
** 函数名:KeyToGui
** 功能:键盘对GUI的影响
** 注意事项:键盘对控件的影响,只限于当前WIN的焦点控件
***************************************************************/
void KeyToGui(GUIKEY key)
{
	if(pFocus_Widget!=NULL)
	{
		KeyToChangeWidget(pFocus_Widget,key);
	}

	//KeyToChangeWindow();
}

/**************************************************************
** 函数名:GuiSysTick20ms
** 功能:GUI的系统时钟调用函数
** 注意事项:主要是被调用,暂时定为20MS一次.其中光标的闪烁是500MS一次 
** 			20MS用于触摸屏的算法 (预留的,也许所有GUI系统调用应归类到一个1MS的函数上)
***************************************************************/
void GuiSysTick20ms(void)
{
	static u8 cnt_500ms=0;
	static bool flash;

	//20MS要做的事
	//...处理WIDGET_PARAM_ACTIVE
	if(pNow_Window!=NULL)
	{
		GuiWidgetReponse((DX_WIDGET*)pNow_Window,WIDGET_PARAM_ACTIVE);
	}

	//500MS要做的事情
	if(++cnt_500ms==25)
	{
		cnt_500ms=0;
		if(flash==TRUE)
			flash=FALSE;
		else
			flash=TRUE;
		WidgetFlash(flash);
		//EDIT的光标刷新-只有自由属性的非光标满的焦点EDIT有光标,需要在闪烁控制的后面
		EditCursorFlash(flash);
		
	}
}


/**************************************************************
 
		GUI的响应队列,用于处理控件的响应函数*_*暂时还没用的上,先预留这个队列功能
 
***************************************************************/
//队列用的缓冲
static WIDGET_PARAM_Q WPQ[WIDGET_PARAM_Q_SIZE];
//队列指针
static u32 WPQ_Head_Ptr=0;
static u32 WPQ_Tail_Ptr=0;

/**************************************************************
** 函数名:WPQPost
** 功能 :发送响应函数到队列
** 注意事项:成功返回TRUE
***************************************************************/
bool WPQPost(DX_WIDGET* widget,u8 param)
{
	u32 tmp;
	tmp=WPQ_Head_Ptr+1;
	if(tmp==WIDGET_PARAM_Q_SIZE)
	{
		tmp=0;
	}
	if(tmp!=WPQ_Tail_Ptr)
	{
		WPQ[WPQ_Head_Ptr].widget=widget;
		WPQ[WPQ_Head_Ptr].param=param;
		WPQ_Head_Ptr=tmp;
		return TRUE;
	}
	else
		return FALSE;
}

/**************************************************************
** 函数名:WPQPend
** 功能:获取队列中的WPQ,以FIFO形式获取
** 注意事项:没有信息,则返回NULL
***************************************************************/
WIDGET_PARAM_Q* WPQPend(void)
{
	u32 wpq_tail_ptr;
	if(WPQ_Tail_Ptr!=WPQ_Head_Ptr)
	{
		wpq_tail_ptr=WPQ_Tail_Ptr;
		if(++WPQ_Tail_Ptr==WIDGET_PARAM_Q_SIZE)
			WPQ_Tail_Ptr=0;
		return WPQ+wpq_tail_ptr;
	}
	else
	{
		return NULL;
	}
}


/**************************************************************
** 函数名:ManageWPQ**没投入使用
** 功能:处理队列
** 注意事项:建议在GUI系统时钟里调用 
**  		处理思想为,每次把现有的响应都处理,而产生的新的响应,则留到下次在处理,这样的好处是,防止因响应而
**  		引起的响应会使系统死循环
***************************************************************/
void ManageWPQ(void)
{
	u32 wpq_no;//本次要处理的响应的个数
	u32 i;
	WIDGET_PARAM_Q* p_wpq;

	if(WPQ_Head_Ptr>=WPQ_Tail_Ptr)
	{
		wpq_no=WPQ_Head_Ptr-WPQ_Tail_Ptr;
	}
	else
	{
		wpq_no=WIDGET_PARAM_Q_SIZE+WPQ_Head_Ptr-WPQ_Tail_Ptr;
	}

	if(wpq_no!=0)
	{
		//只处理本次的响应
		for(i=0;i<wpq_no;i++)
		{
			//处理响应
			p_wpq=WPQPend();
			if(p_wpq!=NULL)
			{
				//安全判断,实际上为了加快效率,在入列之前就应该判断是否NULL.所以这里将来可省略
				if(p_wpq->widget->response_fun!=NULL)
				{
					(*(p_wpq->widget->response_fun))(p_wpq->param);
				}
			}
		}
	}
}


/**************************************************************
** 函数名:GuiWidgetReponse
** 功能:控件的响应
** 注意事项:为了把控件的响应写规范,使用这个函数统一管理,函数只需要两个参数:控件,响应类型 
**  		暂时为直接运行,以后根据情况改进
***************************************************************/
void GuiWidgetReponse(DX_WIDGET* the_widget,u8 param)
{
//	if(the_widget!=NULL)//这个判断暂时可省略
	{
		if(the_widget->response_fun!=NULL)
		{
			(*(the_widget->response_fun))(param);
		}
	}
}
