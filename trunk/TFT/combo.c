/********************************************************
**ģ��:COMBO.c
**����:COMBOBOX��صĴ���
**����:���Ӱײ�
**���¼�¼:�����COMBO�ʹ�ͳPC�ϵ�COMBO��ͬ,�򵥺ܶ�,�����ڶ�ѡ���.��˫ѡ�����Ժ�CHECK���
********************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
//#include "combo.h"
#include "window.h"

/**************************************************************
**������:__SetOperationalWidget
**����:��ȡCOMBO�ĸ�����������,����ֵ��ȫ�ֱ�����,ʹ��֮��Ĳ�����������COMBO
**ע������:�����������Ч�ʺͼ��ٴ��볤��֮��,ע�⵽����COMBO����֮ǰ,������ô˺�����ȡ��Ϣ 
***************************************************************/
static BPRO __base_property_combo_type;
static BPRO __base_property_combo_style;
static u8 __font_height;//�ָ�
static COLOR __color_string;

static void __SetOperationalWidget(DX_COMBO* the_combo)
{
	__base_property_combo_type=the_combo->widget.base_property&BASE_PRO_COMBO_TYPE_MASK;
	__base_property_combo_style=the_combo->widget.base_property&BASE_PRO_COMBO_STYLE_MASK;

	if((the_combo->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)//16�ָ�
	{
		__font_height=16;
	}
	else if((the_combo->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)//24�ָ�
	{
		__font_height=24;
	}
	else if((the_combo->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)//24�ָ� 
	{
		__font_height=32;
	}

	//�Զ��޸�tmpֵ
	if ( (*(the_combo->p_tmp))>the_combo->max_data-1)//��ǰ��Ŀ�Ų��ɳ�max
	{
		*(the_combo->p_tmp)=the_combo->max_data-1;
	}

	//ȷ������ɫ
	if ((*(the_combo->widget.p_ext_property))&EXT_PRO_FLASH)			//��˸
	{
		BG_COLOR=COLOR_FLASH;
	}
	else if((*(the_combo->widget.p_ext_property))&EXT_PRO_FOCUS)		//����
	{
		BG_COLOR=COLOR_FOCUS;
	}
	else if((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)		//��Ч
	{
		BG_COLOR=COLOR_DISABLE;
	}
	else
	{
		if(__base_property_combo_style==BASE_PRO_COMBO_STYLE_3D)			//3D������Լ��涨�ĵ�ɫ-SYSɫ
		    BG_COLOR=COLOR_SYS_FILL_RECT;
		else if(__base_property_combo_style==BASE_PRO_COMBO_STYLE_2D)	//2D��2dɫ
		{	
			//2D��ɫʹ��color_2d
			BG_COLOR=the_combo->color2d;
		}
		else
			BG_COLOR=the_combo->widget.far_win->widget.color;			//NULL���ô��ڵ�ɫ
	}

	//��ȡ����ɫ
	__color_string=the_combo->widget.color;					//COMBO��ɫ,3D������
}

/**************************************************************
** ������:__DebugChecking
** ����:����ģʽ�¼��Combo�Ƿ����,�������һ��while(1)����,���ھ�̬������ʱ������û���������
** ע������:����DEBUG_GUI�궨���������Ƿ����
***************************************************************/
#ifdef DEBUG_GUI

static void __DebugChecking(DX_COMBO *the_combo)
{
	if(the_combo->p_tmp==NULL)
	{
		gui_asssert_failed(0);
		while(1);
	}
	if (the_combo->max_data==0)//max_data����Ϊ0
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
** ������:__DrawFrame
** ����:��COMBO�����
** ע������:
***************************************************************/
static void __DrawFrame(DX_COMBO *the_combo)
{
	//���������,����Ҫ����
	if((*(the_combo->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}

	//��BMP��COMBO�ſ��б���ɫ�����
	if ((__base_property_combo_type)!=BASE_PRO_COMBO_TYPE_BMP)
	{
		//���ݷ�����
		if (__base_property_combo_style==BASE_PRO_COMBO_STYLE_3D)//3D���
		{
			__Draw3DFrameB(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height);
		}
		else if (__base_property_combo_style==BASE_PRO_COMBO_STYLE_2D)//2D���
		{
			TFTRect(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height,COLOR_FRAME_2D);
		}
		else if (__base_property_combo_style==BASE_PRO_COMBO_STYLE_NULL)//NULL���-
		{
			//NULL������û�п�ɫ
		}
	}
}


/**************************************************************
** ������ :__Draw
** ����:ˢ��COMBO�ռ�������
** ע������:
***************************************************************/
static void __Draw(DX_COMBO* the_combo)
{
	LOC_X x_str,y_str;
	u8 str_length;
	char* str;
	COLOR* bmp_buf;
	u32 align;

	//���������,����Ҫ�ø����ڱ���ɫ���
	if((*(the_combo->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		TFTRectFill(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height,(the_combo->widget.far_win)->widget.color);
		return;
	}

	//�ַ�������
	if (__base_property_combo_type==BASE_PRO_COMBO_TYPE_STRING)
	{
		//������
		if(__base_property_combo_style==BASE_PRO_COMBO_STYLE_NULL)
		{
			TFTRectFill(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height,BG_COLOR);
		}
		else//�����2D����3Dģʽ,���������λ
		{
			TFTRectFill(the_combo->widget.x+2,the_combo->widget.y+2,the_combo->widget.length-4,the_combo->widget.height-4,BG_COLOR);
		}
		//��ȡ����߶�
		str_length=__font_height/2;
		
		//��ȡ�ַ���
		str=(((char**)the_combo->obj)[*(the_combo->p_tmp)]);
		//��ȡ�ַ���λ��
		y_str=GetMiddleFromLine(the_combo->widget.y,the_combo->widget.y+the_combo->widget.height+1,__font_height);
		align=the_combo->widget.base_property&BASE_PRO_ALIGN_MASK;
		//Ĭ���м����
		if((align==BASE_PRO_ALIGN_DEFAULT)||(align==BASE_PRO_ALIGN_MIDDLE))
		{
			x_str=GetMiddleFromLine(the_combo->widget.x,the_combo->widget.x+the_combo->widget.length+1,GetStringSize(str)*str_length);
		}
		//�����
		else if(align==BASE_PRO_ALIGN_LEFT)
		{
			x_str=the_combo->widget.x+3;
		}
		//�Ҷ���
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
	//BMP����
	else if(__base_property_combo_type==BASE_PRO_COMBO_TYPE_BMP)
	{
		bmp_buf=(((COLOR**)the_combo->obj)[*(the_combo->p_tmp)]);
		TFTShowBmp(the_combo->widget.x,the_combo->widget.y,the_combo->widget.length,the_combo->widget.height,bmp_buf);
	}
}

/**************************************************************
** ������:ReDrawCombo
** ����:�ػ�COMBO
** ע������:����һЩˢ�µ��ػ�,���ı�ؼ������κα�־
***************************************************************/
void ReDrawCombo(DX_COMBO* the_combo)
{
	//0.��ȡ����
	__SetOperationalWidget(the_combo);
	//1.�����
	__DrawFrame(the_combo);
	//2.�ػ�����
	__Draw(the_combo);
}

/**************************************************************
** ������:GuiComboDraw
** ����:��COMBO�ؼ�
** ע������:�˺�����������WINDOW���ֵ�ʱ����ʾ�ؼ�,�����ػ���
**  
***************************************************************/
void GuiComboDraw(DX_COMBO *the_combo)
{

	//0.��ȡ����
	__SetOperationalWidget(the_combo);
	//1.�������ж�
	__DebugChecking(the_combo);
	//2.HIDE���Դ���
	if ((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)
	{
		return;
	}
	//3.�ڲ���־����
	// ...
	
	//4.�����
	__DrawFrame(the_combo);

	//5.�ػ�����
	__Draw(the_combo);
}


/**************************************************************
** ������:SetComboTmp/GetComboTmp
** ����:����COMBO����ʾֵ
** ע������:COMBO��ֵ��U8���͵�,set�����������ж�ֵ�Ƿ��б仯,�б仯�Ÿ���.
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
** ������:UpdateCombo()
** ����:tmpֵ��dataֵ���໥ת��
** ע������:TRUE��data->tmp,��ˢ��;FALSE tmp=>data 
** 		���dataָ��ΪNULL,��������,���tmpΪNULL,������̫����
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
** ������:KeyToChangeCombo
** ����:COMBO�԰����ķ�Ӧ
** ע������:ע��KeyToChangeXXϵ��,����Ӧ�Է��������Ӧ.���������Ӧ��ϵͳ����
***************************************************************/
MESSAGE_WIDGET KeyToChangeCombo(DX_COMBO* the_combo,GUIKEY key)
{
	MESSAGE_WIDGET msg_widget=MESSAGE_NULL;
	bool reflash=FALSE;

	if ((*(the_combo->widget.p_ext_property))&EXT_PRO_DISABLE)
		return MESSAGE_NULL;

	//�س�
	if (key==KEY_TO_GUI_ENTER)
	{
		msg_widget=MESSAGE_WIDGET_ENTER;
		reflash=FALSE;
	}
	//�༭
	else if((KEY_TO_GUI_0<key)&&(KEY_TO_GUI_9))
	{
		if(++(*(the_combo->p_tmp))>=the_combo->max_data)
		{
			*(the_combo->p_tmp)=0;
		}
		msg_widget=MESSAGE_WIDGET_EDIT;
		reflash=TRUE;
	}

	//�ػ�
	if(reflash==TRUE)
	{
		//��ȡ��������
		__SetOperationalWidget(the_combo);
		//�ػ�
		__Draw(the_combo);
	}

	return msg_widget;
}