/********************************************************
**ģ��:CHECK.c
**����:CHECKBOX��صĴ���
**����:���Ӱײ�
**���¼�¼:
********************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
#include "window.h"

/**************************************************************
**������:__SetOperationalWidget
**����:��ȡCHECK�ĸ�����������,����ֵ��ȫ�ֱ�����,ʹ��֮��Ĳ�����������CHECK
**ע������:�����������Ч�ʺͼ��ٴ��볤��֮��,ע�⵽����CHECK����֮ǰ,������ô˺�����ȡ��Ϣ 
***************************************************************/
static BPRO __base_property_check_type;
static BPRO __base_property_check_style;
static u8 __font_height;//�ָ�
static COLOR __color_string;

static void __SetOperationalWidget(DX_CHECK* the_check)
{
	__base_property_check_type=the_check->widget.base_property&BASE_PRO_CHECK_TYPE_MASK;
	__base_property_check_style=the_check->widget.base_property&BASE_PRO_CHECK_STYLE_MASK;
	
	if((the_check->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)//16�ָ�
	{
		__font_height=16;
	}
	else if((the_check->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)//24�ָ�
	{
		__font_height=24;
	}
	else if((the_check->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)//24�ָ� 
	{
		__font_height=32;
	}
	
	//ȷ������ɫ
	if ((*(the_check->widget.p_ext_property))&EXT_PRO_FLASH)			//��˸
	{
		BG_COLOR=COLOR_FLASH;
	}
	else if((*(the_check->widget.p_ext_property))&EXT_PRO_FOCUS)		//����
	{
		BG_COLOR=COLOR_FOCUS;
	}
	else if((*(the_check->widget.p_ext_property))&EXT_PRO_DISABLE)		//��Ч
	{
		BG_COLOR=COLOR_DISABLE;
	}
	else
	{
		if(__base_property_check_type==BASE_PRO_CHECK_TYPE_HOOK)		//��ģʽ,��ɫ������SYSɫ
		{
			BG_COLOR=COLOR_SYS_FILL_RECT;
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_3D)	//3D���
		{    
			BG_COLOR=the_check->color2d;
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_2D)	//2D��2dɫ
		{	
			BG_COLOR=the_check->color2d;
		}
		else
			BG_COLOR=the_check->widget.far_win->widget.color;			//NULL���ô��ڵ�ɫ
	}

	//��ȡ����ɫ
	__color_string=the_check->widget.color;					//CHECK��ɫ,3D������
}

/**************************************************************
** ������:__DebugChecking
** ����:����ģʽ�¼��Check�Ƿ����,�������һ��while(1)����,���ھ�̬������ʱ������û���������
** ע������:����DEBUG_GUI�궨���������Ƿ����
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
** ������:__DrawFrame
** ����:�����
** ע������:����ǰҪ�Ȼ�ȡ����
***************************************************************/
static void __DrawFrame(DX_CHECK *the_check)
{
	LOC_X length;
	LOC_Y height;

	///���������,����Ҫ����
	if((*(the_check->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}
	if(__base_property_check_type==BASE_PRO_CHECK_TYPE_HOOK)//��ģʽ(���ģʽҲ��2D,3D,NULL֮��)
	{
		//����ģʽ�Ŀ�-����ʽ,���С������߶ȶ�
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
	//BMOģʽû�п�
	//..
	else if(__base_property_check_type==BASE_PRO_CHECK_TYPE_STRING)//�ַ�ģʽ
	{
		//����
		if(__base_property_check_style==BASE_PRO_CHECK_STYLE_2D)	//2D
		{
			TFTRect(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height,BLACK);
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_3D)//3D
		{
			__Draw3DFrameB(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height);
		}
		else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_NULL)//�޿�-�޿���һ��������ʾ
		{
		}
	}
}

/**************************************************************
** ������:__Draw
** ����:�ػ�CHECK
** ע������:ֻ���ݱ�־�ػ�,���ı��־λ����Ϣλ.����ǰҪ�Ȼ�ȡ����
***************************************************************/
static void __Draw(DX_CHECK *the_check)
{
	COLOR *bmp;
	char *str;
	LOC_X x_str;
	LOC_Y y_str;
	///���������,����Ҫ�ø����ڱ���ɫ���
	if((*(the_check->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		if(__base_property_check_type==BASE_PRO_CHECK_TYPE_HOOK)//��ģʽ
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

	if(__base_property_check_type==BASE_PRO_CHECK_TYPE_HOOK)//������-���С���������
	{
		//����ģʽ�Ŀ�-����ʽ,����С������߶ȶ�
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
		
		//����
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
	else if(__base_property_check_type==BASE_PRO_CHECK_TYPE_BMP)//BMP����
	{
		if((*(the_check->p_tmp))==TRUE)
			bmp=((COLOR**)(the_check->obj))[0];
		else
			bmp=((COLOR**)(the_check->obj))[1];
		TFTShowBmp(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height,bmp);
	}
	else
	{
			//���
			if(__base_property_check_style==BASE_PRO_CHECK_STYLE_2D)	//2D
			{
				TFTRectFill(the_check->widget.x+2,the_check->widget.y+2,the_check->widget.length-4,the_check->widget.height-4,BG_COLOR);
			}
			else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_3D)//3D
			{
				TFTRectFill(the_check->widget.x+2,the_check->widget.y+2,the_check->widget.length-4,the_check->widget.height-4,BG_COLOR);
			}
			else if(__base_property_check_style==BASE_PRO_CHECK_STYLE_NULL)//�޿�-�޿���һ��������ʾ
			{
				TFTRectFill(the_check->widget.x,the_check->widget.y,the_check->widget.length,the_check->widget.height,BG_COLOR);
			}
	
			if((*(the_check->p_tmp))==TRUE)
				str=((char**)(the_check->obj))[0];
			else
				str=((char**)(the_check->obj))[1];
			
			x_str=GetStringLocXFromFRAME(the_check->widget.x,the_check->widget.length,__font_height/2,str);
			y_str=GetMiddleFromLine(the_check->widget.y,the_check->widget.y+the_check->widget.height,__font_height);
	
			//д��
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
** ������:ReDrawCheck
** ����:�ػ�ؼ�
** ע������:����һЩˢ�µ��ػ�,���ı�ؼ������κα�־
***************************************************************/
void ReDrawCheck(DX_CHECK *the_check)
{
	//0.��ȡ����
	__SetOperationalWidget(the_check);
	//1.�����
	__DrawFrame(the_check);
	//2.�ػ�����
	__Draw(the_check);

}

/**************************************************************
** ������:GuiCheckDraw
** ����:��ʾCHECK�ؼ�
** ע������:�˺�����������WINDOW���ֵ�ʱ����ʾ�ؼ�,�����ػ���
***************************************************************/
void GuiCheckDraw(DX_CHECK *the_check)
{
	//0.��ȡ����
	__SetOperationalWidget(the_check);
	//1.�������ж�
	__DebugChecking(the_check);
	//2.HIDE���Դ���
	if ((*(the_check->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}
	//3.�ڲ�����
	// ..
	
	//4.�����
	__DrawFrame(the_check);
	
	//5.�ػ�
	__Draw(the_check);
}

/**************************************************************
** ������:SetCheckTmp/GetCheckTmp
** ����:����/��ȡCheck����ʾֵ
** ע������:
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
** ������:UpdateCheck()
** ����:tmpֵ��dataֵ���໥ת��
** ע������:TRUE��data->tmp,��ˢ��;FALSE tmp=>data 
** 		���dataָ��ΪNULL,��������,���tmpΪNULL,������̫����
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
** ������:KeyToChangeCheck
** ����:������CHECK�ĸı�
** ע������: *_*���������������ػ�ؼ�,�ɿ��ǲ��������ػ�
** 			ע��KeyToChangeXXϵ��,����Ӧ�Է��������Ӧ.���������Ӧ��ϵͳ����
***************************************************************/
MESSAGE_WIDGET KeyToChangeCheck(DX_CHECK* the_check,GUIKEY key)
{
	MESSAGE_WIDGET msg_widget=MESSAGE_NULL;
	bool reflash=FALSE;

	//DISABLE�ؼ�������
	//if ((*(the_check->widget.p_ext_property))&EXT_PRO_DISABLE)
	//	return MESSAGE_NULL;

	//�س�
	if (key==KEY_TO_GUI_ENTER)
	{
		msg_widget=MESSAGE_WIDGET_ENTER;
		reflash=FALSE;
	}
	//�༭
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

	//�ػ�
	if(reflash==TRUE)
	{
		//��ȡ��������
		__SetOperationalWidget(the_check);
		//�ػ�
		__Draw(the_check);
	}

	return msg_widget;
}

