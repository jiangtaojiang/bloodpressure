/********************************************************
**ģ��:BUTTON.c
**����:������صĴ���
**����:���Ӱײ�
**���¼�¼:
********************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "TFTDriver.h"
#include "widget.h"
#include "window.h"

/**************************************************************
**������:__SetOperationalWidget
**����:��ȡButton�ĸ�����������,����ֵ��ȫ�ֱ�����,ʹ��֮��Ĳ�����������BUTTON
**ע������:�����������Ч�ʺͼ��ٴ��볤��֮��,ע�⵽����Button����֮ǰ,������ô˺�����ȡ��Ϣ 
***************************************************************/
static BPRO __base_property_button_type;
//static BPRO __base_property_button_style;//��ʱ����
static u8 __font_height;//�ָ�

static void __SetOperationalWidget(DX_BUTTON* the_button)
{
	__base_property_button_type=the_button->widget.base_property&BASE_PRO_BUTTON_TYPE_MASK;
//	__base_property_button_style=the_button->widget.base_property&BASE_PRO_BUTTON_STYLE_MASK;

	if((the_button->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)//16�ָ�
	{
		__font_height=16;
	}
	else if((the_button->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)//24�ָ�
	{
		__font_height=24;
	}
	else if((the_button->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)//32�ָ� 
	{
		__font_height=32;
	}
	
	//BUTTON�ķ��Ƚϵ�һ,���Բ���Ҫ�������ر�������ɫֵ
}

/**************************************************************
** ������:__DebugChecking
** ����:����ģʽ�¼��Check�Ƿ����,�������һ��while(1)����,���ھ�̬������ʱ������û���������
** ע������:����DEBUG_GUI�궨���������Ƿ����
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
** ������:__DrawFrame
** ����:��BUTTON�����
** ע������:��ʱBUTTON�Ļ�����__Draw�����,��ΪBUTTONĳЩ�仯,��Ҳ���иı�
***************************************************************/
static void __DrawFrame(DX_BUTTON* the_button)
{
	//��������ε�,����Ҫ����
	if((*(the_button->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}
}
/**************************************************************
** ������:__Draw
** ����:�ػ水��
** ע������:
***************************************************************/
static void __Draw(DX_BUTTON *the_button)
{
	u8 str_length;
	LOC_X x_str,y_str;
	//���������,����Ҫ�ø����ڱ���ɫ���
	if((*(the_button->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		TFTRectFill(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height,(the_button->widget.far_win)->widget.color);
		return;
	}
	//BMPģʽ
	if(__base_property_button_type==BASE_PRO_BUTTON_TYPE_BMP)
	{
		if ((*(the_button->widget.p_ext_property))&EXT_PRO_FOCUS) //����(�ְ����뵯��)
		{
			//����
			if ((*(the_button->p_self_property))&SELF_PRO_PUSH_DOWN)
			{	
				if(the_button->bmp_push!=NULL)
				{
					TFTShowBmp(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height,the_button->bmp_push);
				}
			}
			//����
			else
			{	
				//���㰴��BMP
				if(the_button->bmp_focus!=NULL)
				{
					TFTShowBmp(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height,the_button->bmp_focus);
				}
				//���㰴��BMPΪ����̬�ķ�ɫ
				else
				{
					//TFTShowBmp(x0,y0,the_button->widget.length,the_button->widget.height,the_button->bmp_normal);//*_*��ҪҪ���з�ɫ����
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
	//�ַ�����
	else if(__base_property_button_type==BASE_PRO_BUTTON_TYPE_STRING)
	{

		//�ַ����̲���̫С,�������岻����ʾ.���ﲻ���ж�,�û��Լ�ע��
		//������ɫ-
		if ((*(the_button->widget.p_ext_property))&EXT_PRO_FLASH)//&&(��˸��־��Ч)	//��˸̬
		{
			TFTRectFill(the_button->widget.x+1,the_button->widget.y+1,the_button->widget.length-2,the_button->widget.height-2,COLOR_FLASH);
			BG_COLOR=COLOR_FLASH;
			goto frame_draw;
		}
		else if ((*(the_button->widget.p_ext_property))&EXT_PRO_FOCUS) //����(�ְ����뵯��)
		{
			TFTRectFill(the_button->widget.x+2,the_button->widget.y+2,the_button->widget.length-4,the_button->widget.height-4,COLOR_FOCUS);
			BG_COLOR=COLOR_FOCUS;
		}
		else if ((*(the_button->widget.p_ext_property))&EXT_PRO_DISABLE)	 //disable
		{
			TFTRectFill(the_button->widget.x+2,the_button->widget.y+2,the_button->widget.length-4,the_button->widget.height-4,COLOR_DISABLE);
			BG_COLOR=COLOR_BUTTON_BG_UNFOCUS;
		}
		else									//��̬
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
		
		//���ڿ�
		if ((*(the_button->p_self_property))&SELF_PRO_PUSH_DOWN)	 //����״̬
		{
			//���п�
			__Draw3DFrameC(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height);
			x_str++;
			y_str++;
		}
		else									//����״̬
		{
			//͹��
			__Draw3DFrameB(the_button->widget.x,the_button->widget.y,the_button->widget.length,the_button->widget.height);
		}

		//д��
		if(__font_height==16)
		{
			
			if ((*(the_button->widget.p_ext_property))&EXT_PRO_DISABLE)	 //disable,����Ҫд͸����,Ϊ���ص���DISABLE��Ч��
			{
				TFTShowString16(x_str+1,y_str+1,the_button->str,COLOR_STRING_DISABLE1,0);
				TFTShowString16(x_str,y_str,the_button->str,COLOR_STRING_DISABLE2,1);
			}
			else
			 	TFTShowString16(x_str,y_str,the_button->str,the_button->widget.color,0);
			
		}
		if (__font_height==24)
		{
			if ((*(the_button->widget.p_ext_property))&EXT_PRO_DISABLE)	 //disable,����Ҫд͸����,Ϊ���ص���DISABLE��Ч��
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
			if ((*(the_button->widget.p_ext_property))&EXT_PRO_DISABLE)//��Ч��BUTTON
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
** ������:ReDrawButton
** ����:�ػ�BUTTON
** ע������:����һЩˢ�µ��ػ�,���ı�ؼ������κα�־
***************************************************************/
void ReDrawButton(DX_BUTTON* the_button)
{
	//0.��ȡ����
	__SetOperationalWidget(the_button);
	//1.�����
	__DrawFrame(the_button);
	//2.�ػ�����
	__Draw(the_button);
}

/**************************************************************
**������:ButtonDraw
**����:������
**ע������:�˺�����������WINDOW���ֵ�ʱ����ʾ�ؼ�,�����ػ���
***************************************************************/
void GuiButtonDraw(DX_BUTTON *the_button)
{   
	//0.��ȡ����
	__SetOperationalWidget(the_button);

	//1.�������ж�
	__DebugChecking(the_button);

	//2.������Ч������ؼ�
	if ((*(the_button->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}

	//3.�ڲ�����
	//...

	//4.�����
	__DrawFrame(the_button);

	//5.�ػ�
	__Draw(the_button);
}

/**************************************************************
** ������:UpdateButton
** ����:����һ��Button
** ע������:���¿ؼ�һ�����ػ�һ�� *_*�˺�����ʱ�ƺ�û��Ҫ
***************************************************************/
void UpdateButton(DX_BUTTON *the_button)
{
	//��ȡ����
	__SetOperationalWidget(the_button);
	//�����
	//..
	//������
	__Draw(the_button);
}

/**************************************************************
** ������:KeyToChangeButton
** ����:������BUTTON��Ӱ��
** ע������:һ����˵,ֻ�лس�����BUTTON������ 
** 			ע��KeyToChangeXXϵ��,����Ӧ�Է��������Ӧ.���������Ӧ��ϵͳ����
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
