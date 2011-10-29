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
** ������:KeyToGui
** ����:���̶�GUI��Ӱ��
** ע������:���̶Կؼ���Ӱ��,ֻ���ڵ�ǰWIN�Ľ���ؼ�
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
** ������:GuiSysTick20ms
** ����:GUI��ϵͳʱ�ӵ��ú���
** ע������:��Ҫ�Ǳ�����,��ʱ��Ϊ20MSһ��.���й�����˸��500MSһ�� 
** 			20MS���ڴ��������㷨 (Ԥ����,Ҳ������GUIϵͳ����Ӧ���ൽһ��1MS�ĺ�����)
***************************************************************/
void GuiSysTick20ms(void)
{
	static u8 cnt_500ms=0;
	static bool flash;

	//20MSҪ������
	//...����WIDGET_PARAM_ACTIVE
	if(pNow_Window!=NULL)
	{
		GuiWidgetReponse((DX_WIDGET*)pNow_Window,WIDGET_PARAM_ACTIVE);
	}

	//500MSҪ��������
	if(++cnt_500ms==25)
	{
		cnt_500ms=0;
		if(flash==TRUE)
			flash=FALSE;
		else
			flash=TRUE;
		WidgetFlash(flash);
		//EDIT�Ĺ��ˢ��-ֻ���������Եķǹ�����Ľ���EDIT�й��,��Ҫ����˸���Ƶĺ���
		EditCursorFlash(flash);
		
	}
}


/**************************************************************
 
		GUI����Ӧ����,���ڴ���ؼ�����Ӧ����*_*��ʱ��û�õ���,��Ԥ��������й���
 
***************************************************************/
//�����õĻ���
static WIDGET_PARAM_Q WPQ[WIDGET_PARAM_Q_SIZE];
//����ָ��
static u32 WPQ_Head_Ptr=0;
static u32 WPQ_Tail_Ptr=0;

/**************************************************************
** ������:WPQPost
** ���� :������Ӧ����������
** ע������:�ɹ�����TRUE
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
** ������:WPQPend
** ����:��ȡ�����е�WPQ,��FIFO��ʽ��ȡ
** ע������:û����Ϣ,�򷵻�NULL
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
** ������:ManageWPQ**ûͶ��ʹ��
** ����:�������
** ע������:������GUIϵͳʱ������� 
**  		����˼��Ϊ,ÿ�ΰ����е���Ӧ������,���������µ���Ӧ,�������´��ڴ���,�����ĺô���,��ֹ����Ӧ��
**  		�������Ӧ��ʹϵͳ��ѭ��
***************************************************************/
void ManageWPQ(void)
{
	u32 wpq_no;//����Ҫ�������Ӧ�ĸ���
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
		//ֻ�����ε���Ӧ
		for(i=0;i<wpq_no;i++)
		{
			//������Ӧ
			p_wpq=WPQPend();
			if(p_wpq!=NULL)
			{
				//��ȫ�ж�,ʵ����Ϊ�˼ӿ�Ч��,������֮ǰ��Ӧ���ж��Ƿ�NULL.�������ｫ����ʡ��
				if(p_wpq->widget->response_fun!=NULL)
				{
					(*(p_wpq->widget->response_fun))(p_wpq->param);
				}
			}
		}
	}
}


/**************************************************************
** ������:GuiWidgetReponse
** ����:�ؼ�����Ӧ
** ע������:Ϊ�˰ѿؼ�����Ӧд�淶,ʹ���������ͳһ����,����ֻ��Ҫ��������:�ؼ�,��Ӧ���� 
**  		��ʱΪֱ������,�Ժ��������Ľ�
***************************************************************/
void GuiWidgetReponse(DX_WIDGET* the_widget,u8 param)
{
//	if(the_widget!=NULL)//����ж���ʱ��ʡ��
	{
		if(the_widget->response_fun!=NULL)
		{
			(*(the_widget->response_fun))(param);
		}
	}
}
