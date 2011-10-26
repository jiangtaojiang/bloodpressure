/**************************************************************
** ģ���� :window.c
** ���� :���ڹ���
** ���¼�¼ :2009-11-19 ������WINDOW��������ؼ�����.�пؼ����е�һϵ������
***************************************************************/
#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
#include "window.h"

/**************************************************************
	ƴ�Ӵ���Ĺ���Ϊһ����Ծ��pNow_Window�ͷǻ�Ծ��Z_Axis,ע������֮��û�н���
***************************************************************/
DX_WINDOW *pNow_Window=NULL;		//ָ��ǰ�����,Ϊ�����Ч��,����������GUI�ڲ�ȫ�ֵ���,��һ�㲻Ӧ���û�����
DX_WINDOW *Z_Axis[MAX_Z];				//αZ��,DX-GUI��֧�ֲ������,��֧��ƴ�Ӵ���,Z_Axis����ǰ����������Ч�ķǻ����
static bool __widget_ok=FALSE;

/**************************************************************
** ������:GuiWindowDraw/Add
** ����:������
** ע������:�����ڵ�ͬʱҲ�ǻ��ؼ�,������һ��WINDOW�����������Ӧ���� 
**  		10.2.25:�ڴ����HIDE��Ӧ֮ǰ����pNow_Window,ʹ��������,���оɴ���HIDE������ʱ��,��ǰ��Ч�����Ѿ�ָ�����µĴ���
**  				���ڴ���HIDE������,���Ի�֪��һ���´�����ʲô.
**  
**  		GuiWindowDraw�����Ĵ�����Ϊ�µ�pNow_Window,֮ǰ��pNow_Window�����HIDE����
** 			GuiWindowAdd ���䲻֮ͬ����,��������һ��ƴ�Ӵ���,���Զ�λ��Z��,��Z���ϵĴ��嶼�Ƿǻ����
***************************************************************/
void GuiWindowDraw(DX_WINDOW* the_window)
{
	DX_WINDOW* old_window;
	u8 i;
	bool focus_exit=FALSE;

	if(the_window!=NULL)
	{
		old_window=pNow_Window;
		pNow_Window=the_window;				//ˢ�µ�ǰ����ָ��
	}
	else
		return;

	//��һ�����ڵ���С������hide:ע������ظ�����GuiWindowDraw��ͬһ������,HIDE��������Ч
	__widget_ok=FALSE;
	if((old_window!=NULL)&&(old_window!=pNow_Window))
	{
		GuiWidgetReponse((DX_WIDGET*)old_window,WIDGET_PARAM_HIDE);
	}

	
	pFocus_Widget=NULL;					//��ʼ������ؼ�ָ��
	Focus_Cursor_State=FOCUS_CURSOR_ALL;//��ʼ�����
	BG_COLOR=the_window->widget.color;		//��ʼ������ɫ


	//������
	if(!(the_window->widget.base_property&BASE_PRO_NOT_FILL))
	{
		TFTRectFill(the_window->widget.x,the_window->widget.y,the_window->widget.length,the_window->widget.height,BG_COLOR);
	}

	//����show����
	GuiWidgetReponse((DX_WIDGET*)the_window,WIDGET_PARAM_SHOW);
		
	/**************************************************************
	�Զ��޸�������� 
	0.�����ǰ pFocus_Widget
	1.ȷ���׸������ 
	2.���µĽ��㸴λһ����չ���Ե� EXT_PRO_FOCUSλ 
	3.�������ֻ�ڵ�һ�λ�WINDOW��ʱ����� (�޸���,WINDOW����չ����λEXT_PRO_FOCUSҪ��λ)
	4.���û�ҵ�����,����Ϊ��һ�����ý���Ŀؼ��ý��� 
	5.�������û�н���,����pFocus_WidgetΪNULL 
	***************************************************************/
	pFocus_Widget=NULL;
	//֮ǰû�Զ���ʼ������
	if(!((*(the_window->widget.p_ext_property))&EXT_PRO_FOCUS))
	{
		for(i=0;i<the_window->max_widget;i++)
		{
			//ȷ������ı��
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
				//�޸��ཹ������(ʵ�����ǰ����µĿؼ�����λһ��ext_property����λEXT_PRO_FOCUS)
				*(the_window->p_child[i]->p_ext_property)&=~EXT_PRO_FOCUS;
			}
		}
		else//û�г�ʼ������,����ΪѰ���׸����ý���Ŀؼ�
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

		//�޸����,�Ժ������޸���
		*(the_window->widget.p_ext_property)|=EXT_PRO_FOCUS;
	}
	else //֮ǰ�Ѿ���ʼ���˽���
	{
		i=*(the_window->p_focu_no);
		pFocus_Widget=the_window->p_child[i];
	}
	//���ؼ�
	for(i=0;i<the_window->max_widget;i++)
	{
		GuiWidgetDraw(the_window->p_child[i]);
	}
	__widget_ok=TRUE;
}
void GuiWindowAdd(DX_WINDOW* the_window)
{
	u8 i;

	//1.��������ϵ�ǰ���Ѿ���Ծ�Ĵ���
	if(pNow_Window==the_window)
		return;

	//2.���ﲻ��鴰���Ƿ����ص���,ʵ��ʹ�����û��Լ�ע��
	//..

	//3.����Z��
	for(i=0;i<MAX_Z;i++)
	{
		if(Z_Axis[i]!=NULL)
		{
			//����Ҫ�ظ�����
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
	
	if(i==MAX_Z)//Z���������ټ���
	{
		return;
	}

	BG_COLOR=the_window->widget.color;		//��ʼ������ɫ


	//������
	if(!(the_window->widget.base_property&BASE_PRO_NOT_FILL))
	{
		TFTRectFill(the_window->widget.x,the_window->widget.y,the_window->widget.length,the_window->widget.height,BG_COLOR);
	}
	
	__widget_ok=FALSE;
	//����show����
	GuiWidgetReponse((DX_WIDGET*)the_window,WIDGET_PARAM_SHOW);
		
	/**************************************************************
	ƴ�Ӵ����в������н���ؼ�,������Զ��޸���,�޸���Ĵ���,��EXT_PRO_FOCUS�ᱻ����
	***************************************************************/
	for(i=0;i<the_window->max_widget;i++)
	{
		*(the_window->p_child[i]->p_ext_property)&=~EXT_PRO_FOCUS;	
	}
	*(the_window->widget.p_ext_property)&=~EXT_PRO_FOCUS;
	
	//���ؼ�
	for(i=0;i<the_window->max_widget;i++)
	{
		GuiWidgetDraw(the_window->p_child[i]);
	}
	__widget_ok=TRUE;
}

void GuiWindowDec(DX_WINDOW* the_window)
{
	u8 i,j;

	//����������ǰ���Ѿ���Ծ�Ĵ���
	if(pNow_Window==the_window)
	{
		pNow_Window=NULL;
	}
	//��Z�����޳�
	else
	{
		for(i=0;i<MAX_Z;i++)
		{
			if(Z_Axis[i]==the_window)
			{
				break;
			}
		}
		
		if(i==MAX_Z)//����Z�ᶼû��,��û�п������Ĵ���
		{
			return;
		}
		//����Z��
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
	BG_COLOR=the_window->widget.color;		//��ʼ������ɫ

	//���ɫ,��ʱ��䴰��������ɫ,�ƺ�û��Ҫ���,֮����ʵ��Ӧ�������޸�*_*
	//TFTRectFill(the_window->widget.x,the_window->widget.y,the_window->widget.length,the_window->widget.height,BG_COLOR);

	//����hide����
	GuiWidgetReponse((DX_WIDGET*)the_window,WIDGET_PARAM_HIDE);
}
/**************************************************************
** ������:GetNowWindow
** ����:��ȡ��ǰ����
** ע������:���ص�ǰ�����ָ��.�����GUI�ײ���,��ֱ������pNow_Window
***************************************************************/

DX_WINDOW* GetNowWin(void)
{
	return pNow_Window;
}


/**************************************************************
** ������:IsWindowAvailable
** ����:�жϴ����Ƿ����
** ע������:��ν����,������pNow_Window������Z����
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
** ������:IsWidgetAvailable
** ����:�жϿؼ��Ƿ����
** ע������:��ν����,���������WIN���ò�������沢�ҷ�HIDE
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
** ������ :UpdateWindow
** ���� :tmpֵ��dataֵ���໥ת��
** ע������:TRUE��data->tmp,��ˢ��;FALSE tmp=>data 
***************************************************************/
void UpdateWindowData(DX_WINDOW* the_window,bool dir)
{
	u8 i;
	u8 widget_type;
	
	for(i=0;i<the_window->max_widget;i++)
	{
		widget_type=the_window->p_child[i]->base_property&BASE_PRO_TYPE_MASK; //��ȡ�ؼ�����
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
			//��������ʱ����
			//case BASE_PRO_PROGRESS
			default:
				break;

		}
	}
}