/**************************************************************
**ģ����:EDIT.C
**����:EDITģ��
**���¼�¼:֧��BCD��ʽ,����,С����1,2λ,����,����С��1,2λ  ע��ʵ���ϼ�¼���ݵķ�ʽ��������, 
** 			Ҳ������ʾС��1λģʽ��EDIT,��ʾ1,1.2,�ֱ��Ӧʵ��ֵ10,12
** 			EDIT�����пؼ�����ӵ�һ��,֮�󻹻�����Ľ�һЩBUG.
***************************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
#include "window.h"

/**************************************************************
**������:__SetOperationalWidget(09.8.5)
**����:��ȡEDIT�ĸ�����������,����ֵ��ȫ�ֱ�����,ʹ��֮��Ĳ�����������EDIT
**ע������:�����������Ч�ʺͼ��ٴ��볤��֮��,ע�⵽����EDIT����֮ǰ,������ô˺�����ȡ��Ϣ 
***************************************************************/
static BPRO __base_property_edit_type;	//normal,1p,2p,pw
static BPRO __base_property_edit_style;	//3d,2d,null
static SPRO __self_property_edit_state;	//null,0p,1p,2p

//bcd_len 								//BCD����,���û��������,���ֵΪ0
static u8 __font_height;				//�ָ�

static COLOR __color_string;

static void __SetOperationalWidget(DX_EDIT* the_edit)
{
	__base_property_edit_type=the_edit->widget.base_property&BASE_PRO_EDIT_TYPE_MASK;	  	//normal,1p,2p,pw
	__base_property_edit_style=the_edit->widget.base_property&BASE_PRO_EDIT_STYLE_MASK;		//3d 2d null
	__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;		//null,0p,1p,2p
	
	if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)
	{
		__font_height=16;
	}
	else if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)
	{
		__font_height=24;
	}
	else if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT32)
		__font_height=32;

	//��ȡ����ɫ
	if((*(the_edit->widget.p_ext_property))&EXT_PRO_FLASH)		//&&(��˸��־��Ч)	//��˸̬
	{
		BG_COLOR=COLOR_FLASH;
	}
	else if((*(the_edit->widget.p_ext_property))&EXT_PRO_FOCUS)	//����
	{
		BG_COLOR=COLOR_FOCUS;
	}
	else if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)	//��Ч
	{
		BG_COLOR=COLOR_DISABLE;
	}
	else									//��̬
	{
		if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_3D)			//3D������Լ��涨�ĵ�ɫ-SYSɫ
		    BG_COLOR=COLOR_SYS_FILL_RECT;
		else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_2D)
		{	//2D��ɫʹ��color_2d
			//BG_COLOR=the_edit->color2d;
		}
		else
			BG_COLOR=the_edit->widget.far_win->widget.color;			//NULL���ô��ڵ�ɫ
	}

	//����������ɫ
	if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_3D)
		__color_string=BLACK;
	else 
		__color_string=the_edit->widget.color;				
	
}

/**************************************************************
** ������:__DebugChecking
** ����:����ģʽ�¼��EDIT�Ƿ����,�������һ��while(1)����,���ھ�̬������ʱ������û���������
** ע������:����DEBUG_GUI�궨���������Ƿ����
***************************************************************/
#ifdef DEBUG_GUI

static void __DebugChecking(DX_EDIT *the_edit)
{
	if(the_edit->p_tmp==NULL)
	{
		gui_asssert_failed(0);
		while(1);
	}
	if((the_edit->widget.x<MAX_X)&&(the_edit->widget.y<MAX_Y))
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
** ������:__TFTShowCursor
** ����:��ʾһ�����
** ע������:TRUEΪ��ʾ,FLASE����ʾ��ɫ
***************************************************************/
void __TFTShowCursor(LOC_X x,LOC_Y y,u8 len,bool flash)
{
	COLOR color;
	if(flash==TRUE)
		color=__color_string;
	else
		color=BG_COLOR;
	TFTVLine(x,y,len,color);
}

/**************************************************************
** ������:__DrawFrame(09.8.5)
** ����:��EDIT���
** ע������:֮ǰ��Ҫ���ù�__GetEditPro()
***************************************************************/
static void __DrawFrame(DX_EDIT *the_edit)
{
	///���������,����Ҫ����
	if((*(the_edit->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}

	if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_3D)//3D���
	{
		__Draw3DFrameA(the_edit->widget.x,the_edit->widget.y,the_edit->widget.length,the_edit->widget.height) ;
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_2D)//2D���
	{
		TFTRect(the_edit->widget.x,the_edit->widget.y,the_edit->widget.length,the_edit->widget.height,COLOR_FRAME_2D);
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_NULL)//NULL���-
	{

	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_GLASS)//�������*��ʱ�Ƚϸ���,Ԥ��
	{
		//û�п�
	}
}
/**************************************************************
** ������:__FixEditState(09.8.5)
** ����:ʹEDIT�ӱ༭̬ת��Ϊ��ȫ̬
** ����:�ڼ��̰���ENTER,�����뿪,EditDraw��ʱ�����
** ע������:��������ж���Сֵ,����ǰ�����__GetEditPro
**  ����TRUE����������,FLASH���ǲ���Ҫ����(��������ʵ����֮����Ҫ�ػ�)
**  �ǵ�ǰ������ʾ����EDIT�Ż����Focus_Cursor_State
**  ���֮ǰFocus_Cursor_State��ALL״̬,�򲻸���Focus_Cursor_State
**  
**  *_*��������EDIT�򳤶ȵ�����,ʹ����ʾֵ���ᳬ����
***************************************************************/
bool __FixEditState(DX_EDIT* the_edit)
{
	s32 *ptmp;
	s32 tmp;

	//��ǰ����EDIT,��������λ
	if((the_edit->widget.far_win==pNow_Window)&&((*(the_edit->widget.p_ext_property))&EXT_PRO_FOCUS))
	{
		Focus_Cursor_State=FOCUS_CURSOR_ALL;
	}

	ptmp=the_edit->p_tmp;
	
	//1P���Է�1P״̬
	if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)
	{
		if( __self_property_edit_state!=SELF_PRO_EDIT_STATE_1P)
		{
			tmp=(*ptmp)*10;
			//�ж����ֵ
			if(tmp<=(the_edit->max_data))
			{
				//�ж���Сֵ
				if(tmp>=(the_edit->min_data))
				{
					*ptmp=tmp;
				}
				else
					*ptmp=the_edit->min_data;
			}
			else
			{
				*ptmp=the_edit->max_data;
			}

			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_1P;
			__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			return TRUE;
		}
	}
	//2P����
	else if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)
	{
		//1P״̬
		if( __self_property_edit_state==SELF_PRO_EDIT_STATE_1P)
		{
			tmp=(*ptmp)*10;
			if(tmp<=(the_edit->max_data))
			{
				*ptmp=tmp;
			}
			else
			{
				*ptmp=the_edit->max_data;
			}
			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_2P;
			__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			return TRUE;
		}
		//NULL��0P״̬
		else if( __self_property_edit_state!=SELF_PRO_EDIT_STATE_2P)
		{
			tmp=(*ptmp)*100;
			if(tmp<=(the_edit->max_data))
			{
				*ptmp=tmp;
			}
			else
			{
				*ptmp=the_edit->max_data;
			}
			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_2P;
			__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			return TRUE;
		}
	}
	//�Ѿ�����ȫ̬,���жϴ�С�Ƿ����
	//�ж����ֵ
	if(*ptmp<=(the_edit->max_data))
	{
		//�ж���Сֵ
		if(*ptmp<(the_edit->min_data))
		{	
			*ptmp=the_edit->min_data;
			return TRUE;
		}
	}
	else
	{
		*ptmp=the_edit->max_data;
		return FALSE;
	}
	return FALSE;
}
/**************************************************************
** ���� :__Draw
** ����:ˢ��EDIT�ռ��е�����,���ﻹ����ˢ�±���ɫ
** ע������:  
**  		֧�ָ�����ʾ(09.7.10)
**  
***************************************************************/
static void __Draw(DX_EDIT* the_edit)
{
	LOC_X x_str;
	LOC_Y y_str;
	u8 str_length;
	char str_buf[15+10];//�������8λ����+1λС����+'\0',Ԥ�����0,��15λ
	char *str=&str_buf[15];
	u8 i,j;
	
	///���������,����Ҫ�ø����ڱ���ɫ���
	if((*(the_edit->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		TFTRectFill(the_edit->widget.x,the_edit->widget.y,the_edit->widget.length,the_edit->widget.height,(the_edit->widget.far_win)->widget.color);
		return;
	}

	str_length=__font_height/2;

	

	//���ݷ��λ�ַ���ʾ��λ��(��Ҫ��λXλ��) ��ˢ�����ֲ���
	if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_3D)			//3D���
	{	
		x_str=the_edit->widget.x +4;
		TFTRectFill(the_edit->widget.x+2,the_edit->widget.y+2,the_edit->widget.length-4,the_edit->widget.height-4,BG_COLOR);
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_2D)	//2D���
	{	
		x_str=the_edit->widget.x +2;
		//��ʹBCD���,Ҳ����Ѿ�������
		//if((the_edit->widget.base_property&BASE_PRO_EDIT_BCD_MASK)==BASE_PRO_EDIT_BCD_NULL)
			TFTRectFill(the_edit->widget.x+2,the_edit->widget.y+2,the_edit->widget.length-4,the_edit->widget.height-4,BG_COLOR);
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_NULL)	//NULL���-
	{	
		x_str=the_edit->widget.x;
		if((the_edit->widget.base_property&BASE_PRO_EDIT_BCD_MASK)==BASE_PRO_EDIT_BCD_NULL)
			TFTRectFill(the_edit->widget.x,the_edit->widget.y,the_edit->widget.length,the_edit->widget.height,BG_COLOR);
	}
	else if(__base_property_edit_style==BASE_PRO_EDIT_STYLE_GLASS)	//�������--����
	{
		x_str=the_edit->widget.x;
	}
	
	y_str=GetStringLocYFromFRAME(the_edit->widget.y,the_edit->widget.height,str_length*2);
	

	//���������
	if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_PW)
	{
		j=GET_EDIT_BCD_LEN(the_edit);//*_*���Թ鵽__GetEditPro�л��
		//��8����ģʽ
		if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_O8)
		{
			i=NumToString(*(the_edit->p_tmp),str);
			//1λС��
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P)
			{
				if(i==1)	//0.X�ĸ�ʽ��Ȼ�̶�.
				{
					str[3]='\0';
					str[2]=str[0];
					str[1]='.';
					str[0]='0';
					if(j>1)
						j-=2;
					else
						j=0;
				}
				else
				{
					str[i+1]='\0';
					str[i]=str[i-1];
					str[i-1]='.';
					if(j>i)
						j-=i;
					else 
						j=0;
				}
				
			}
			//2λС��
			else if(__self_property_edit_state==SELF_PRO_EDIT_STATE_2P)
			{
				str[i+2]='\0';
				str[i+1]=str[i-1];
				str[i]=str[i-2];
				str[i-2]='.';
				if(i>3)
				{
					if(j>3)
						j-=i;
					else
						j=0;
				}
				else
				{
					if(j>3)
						j-=3;
					else
						j=0;
				}
			}
			//��λС��
			else if(__self_property_edit_state==SELF_PRO_EDIT_STATE_0P)
			{
				str[i+1]='\0';
				str[i]='.';
				j=0;	//*_*��ʱ�Ȳ��������
			}
			//����
			else
			{
				if(j>=i)
					j-=i;
			}
		}
		else//8����
		{
			i=NumToStringO8(*(the_edit->p_tmp),str);
			if(j>=i)
				j-=i;
		}
		
		//����Jֵ(BCD����)��0
		i=0;
		if(j)
		{
			for(;j!=0;j--)
			{
				str_buf[15-j]='0';
				i++;
			}
			str=&(str_buf[15-i]);
		}

		//����ǰҪ�Ӹ���
		if((*(the_edit->p_tmp))<0)
		{
			i++;
			str_buf[15-i]='-';
			str=&(str_buf[15-i]);
		}
		if(__font_height==16)
		{
			
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString16(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString16(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString16(x_str,y_str,str,__color_string,0);
			} 
			 
		}
		if(__font_height==24)
		{
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString24(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString24(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString24(x_str,y_str,str,__color_string,0);
			}
		}
		else if(__font_height==32)   
		{
			/*
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
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
	//����
	else
	{
		//����ģʽ��,����λ������self_pro��
		i=(*(the_edit->p_self_property)&SELF_PRO_EDIT_PASS_NUM_MASK);
		str[i]='\0';
		for(;i>0;i--)
			str[i-1]='*';
		if(__font_height==16)
		{
			
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString16(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString16(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString16(x_str,y_str,str,__color_string,0);
			} 
			
		}
		if(__font_height==24)
		{
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
			{
				TFTShowString24(x_str+1,y_str+1,str,COLOR_STRING_DISABLE1,0);
				TFTShowString24(x_str,y_str,str,COLOR_STRING_DISABLE2,1);
			}
			else
			{
				TFTShowString24(x_str,y_str,str,__color_string,0);
			}
		}
		else if(__font_height==32)
		{
			/*
			if((*(the_edit->widget.p_ext_property))&EXT_PRO_DISABLE)
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
}
/**************************************************************
** ������:ReDrawEdit
** ����:�ػ�EDIT
** ע������:����һЩˢ�µ��ػ�,���ı�ؼ������κα�־
**  			*_*EDIT�Ƚϸ���,��Ҫ����ʵ��ʹ������ٿ��ǸĽ�
***************************************************************/
void ReDrawEdit(DX_EDIT* the_edit)
{
	//0.��ȡ����
	__SetOperationalWidget(the_edit);
	//1.�����
	__DrawFrame(the_edit);
	//2.�ػ�����
	__Draw(the_edit);
}

/**************************************************************
** ������:GuiEditDraw
** ����:��EDIT
** ע������:�˺�����������WINDOW���ֵ�ʱ����ʾ�ؼ�,�����ػ��� 
** 			EDIT�Ǳ༭���ֵĿؼ�,�����ڲ��Ĺ��״̬. 
***************************************************************/
void GuiEditDraw(DX_EDIT* the_edit)
{
	__DebugChecking(the_edit);

	//0.��ȡ����
	__SetOperationalWidget(the_edit);
	//1.�����Է���
	__DebugChecking(the_edit);
	//2.HIDE���Դ���
	if( (*(the_edit->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}

	//3.�ڲ�����
	__FixEditState(the_edit);

	//4.�����
	__DrawFrame(the_edit);
	
	//5.�ػ�
	__Draw(the_edit);
}

/**************************************************************
** ������:SetEditTmp/GetEditTmp
** ����:����EDIT�ؼ�����ʾֵ
** ע������:�˺������ⲿ���ú���,һ�������,EDIT�ؼ����������ⲿֱ�Ӷ���TMPֵ���иĶ�,Ӧ��ͨ���˺������� 
**  		
**  		��GUI�涨,��С�������Ե�EDIT,���˱༭̬,�������һ����С����ʽ��ʾ,Ҳ����1P���Ե�,TMP=1�ʹ�����0.1.TMP=10�ʹ���1.0
**  		10.2.25:����ģʽ��,�ı�ֵ��ı�����λ��
***************************************************************/
void SetEditTmp(DX_EDIT* the_edit,s32 tmp)
{
	u8 len;

	__SetOperationalWidget(the_edit);

	if(the_edit->p_tmp!=NULL)
	{
		*(the_edit->p_tmp)=tmp;
		//����ģʽ
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_PW)
		{
			//�峤��
			(*(the_edit->p_self_property))&=~SELF_PRO_EDIT_PASS_NUM_MASK;
			//���tmpΪ��0,�������³���
			if(tmp)
			{
				len=GetNumLen(tmp);
				(*(the_edit->p_self_property))|=len;
			}
		}
	}
	

	//����ǵ�ǰ��ʾ���ҷ�����,��ˢ������
	if((IsWidgetAvailable((DX_WIDGET*)the_edit)))
	{
		/*����TMPֵ,�����tmp��ȻΪ����̬ʱ���ֵ,����tmp=50,edit��1P����,��ʵ������ʾΪ5.0, 
		  �����������Ϊ����self_property_edit_stateΪ����̬.
		*/ 
		
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)
		{
			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_1P;
			__self_property_edit_state=SELF_PRO_EDIT_STATE_1P;
		}
		else if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)
		{
			*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
			*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_2P;
			__self_property_edit_state=SELF_PRO_EDIT_STATE_2P;
		} 
		
		//2.����STATE-��EDIT��������ȫ̬��ʱ��,����__FixEditState����Ϊ����tmp ֵ��1.0ȴ�����ʾ10.0�����
		// ����������TMP��ʱ���ܵ���__FixEditState
		//__FixEditState(the_edit);
		__Draw(the_edit);
	}
}

s32 GetEditTmp(DX_EDIT* the_edit)
{
	if(the_edit!=NULL)
	{
		return *(the_edit->p_tmp);
	}
	else 
		return 0;
}

/**************************************************************
** ������:UpdateEdit()
** ����:tmpֵ��dataֵ���໥ת��
** ע������:TRUE��data->tmp,��ˢ��;FALSE tmp=>data 
** 		���dataָ��ΪNULL,��������,���tmpΪNULL,������̫����
***************************************************************/
void UpdateEdit(DX_EDIT* the_edit,bool dir)
{
	if(dir==TRUE)
	{
		if(the_edit->p_data!=NULL)
		{
			SetEditTmp(the_edit,*(the_edit->p_data));
		}
	}
	else
	{
		if(the_edit->p_data!=NULL)
		{
			*(the_edit->p_data)=*(the_edit->p_tmp);
		}
	}
}

/**************************************************************
** ������:KeyToChangeEdit
** ����:�����������Ϣ��EDIT��ֵ��Ӱ��
** ע������:
**  		�˺�����ı� Focus_Edit_Cursor_State��ֵ,������ˢ���ػ�ؼ�.*_*,���Կ��ǲ��ڴ˺������ػ�ؼ�.
**  		���ؿؼ���Ϣ,ע�������ֵû�иı�,�򲻷�����Ч��Ϣ(�˹����ݶ�)
**  		ע��KeyToChangeXXϵ��,����Ӧ�Է��������Ӧ.���������Ӧ��ϵͳ����
***************************************************************/
MESSAGE_WIDGET KeyToChangeEdit(DX_EDIT* the_edit,GUIKEY key)
{
	bool reflash=FALSE;
	MESSAGE_WIDGET msg_widget=MESSAGE_NULL;	//�ؼ���Ϣ
	s32* ptmp;
	s32 tmp, tmp2;
	
	__SetOperationalWidget(the_edit);
	ptmp=the_edit->p_tmp;//��ȡTMPָ��
	
	//1.��������,
	// a.������״̬Ϊ��,����ֱֵ����0����ʾΪ����ֵ
	// b.ע���ж��Ƿ�������ֵ,��������������,���ȡ��ֵ����ԭ��
	// c.�����0,����С����־λ
	// d.���������ģʽ,���0Pģʽ����,��Ҫע��ı�self_pro��NUMλ
	if(/*(key>=KEY_TO_GUI_0)&&*/(key<=KEY_TO_GUI_9))
	{
		//8����8,9������ֱ�Ӻ���
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_O8)
		{
			if((key==KEY_TO_GUI_8)||(key==KEY_TO_GUI_9))
			{
				reflash=TRUE;
				goto end_key_num;
			}
		}
		//a.���Ϊ��
		if(Focus_Cursor_State==FOCUS_CURSOR_ALL)
		{
			tmp2=tmp=key;
			Focus_Cursor_State=0;
			reflash=TRUE;
			//c.
			if( (__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)||(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P))
			{
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				//*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_NULL;//�����ʡ��
				if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_PW)//������ģʽ,�����P��λ
				{
					__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				}
				else//����ģʽ,�����NUM��λ
				{
					*(the_edit->p_self_property)=(*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK;
				}
			}
		}
		//1P����,1P״̬
		else if((__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P))
		{
			return MESSAGE_NULL;
		}
		//2P����,2P״̬
		else if((__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_2P))
		{
			return MESSAGE_NULL;
		}
		else//�����Ŀ��������ֵ�״̬(��������)
		{
			if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_PW)
				if((*ptmp==0)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL))
					Focus_Cursor_State--;
			if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_O8)//��8����
			{
				tmp2=tmp=*ptmp*10+key;
			}
			else
			{
				tmp2=tmp=*ptmp*8+key;
			}
		}

		
		//1P���ֻ������,���ֵ�Ƚ�ʱҪ�������ֳ���10.
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)
		{
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL)
			{
				tmp2=tmp*10;
			}
		}
		//2P���ֻ������,�Ƚ�ʱҪ*100,������*10
		else if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)
		{
			//����
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL)
			{
				tmp2=tmp*100;
			}
			//0/1P
			else if((__self_property_edit_state==SELF_PRO_EDIT_STATE_0P)||(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P))
			{
				tmp2=tmp*10;
			}
		}
		

		//���ֵ������--(����ģʽҲ�����ֵ������,������������λ��)
		if(tmp2<=the_edit->max_data)
		{
			*ptmp=tmp;
			Focus_Cursor_State++;
			reflash=TRUE;
			//���������
			if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_PW)
			{
				tmp=GetNumLen(tmp);
				if(((*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK)!=tmp)
					*(the_edit->p_self_property)=tmp;//���������һЩ��������,ֱ�Ӹ�ֵ
				else
				{
					reflash=FALSE;
					goto end_key_num;
				}
			/*
				//NUMֵ��1�Ŀ�����:û�������ֵ,ԭֵ��0���ҷǰ���0
				if((*ptmp!=0)&&(key!=KEY_TO_GUI_0))
				{
					*(the_edit->p_self_property)=((*(the_edit->p_self_property))+1)&SELF_PRO_EDIT_PASS_NUM_MASK;
				}
				//������0��λ��Ϊ0,��λ����1*_*��0Ҳ��*��,���ȡ����,��Ϊ�Ȱ�0,�ٰ�1���������Ū
			//	else if(((*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK)==SELF_PRO_EDIT_PASS_NUM0)
			//	{
			//		*(the_edit->p_self_property)=((*(the_edit->p_self_property))+1)&SELF_PRO_EDIT_PASS_NUM_MASK;
			//	}
				else
				{
					reflash=FALSE;
					goto end_key_num;
				}
			*/
			}
		}
		else
		{
			//��ȡ����ԭ��
			reflash=FALSE;
			goto end_key_num;
		}
		//��������,С����ı仯
		if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_PW)
		{
			//0P��1P
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_0P)
			{
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_1P;
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			}
			//1P������2P�������2P
			else if((__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P))
			{
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_2P;
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			}
		}
	end_key_num:
		msg_widget=MESSAGE_WIDGET_EDIT;
	}
	//2.����С����
	// a.���BASEģʽ��������С��,�����״̬Ϊ��,����ֱֵ����0������һ��С����
	// b.ֻ��BASE������������С������SELF������NULL�ſ��Ի���һ��С����(0P)
	// c.����ģʽ����������С��
	else if(key==KEY_TO_GUI_POINT)
	{
		if((__base_property_edit_type==BASE_PRO_EDIT_TYPE_1P)||(__base_property_edit_type==BASE_PRO_EDIT_TYPE_2P))
		{
			//a.
			if(Focus_Cursor_State==FOCUS_CURSOR_ALL)
			{
				*ptmp=0;
				Focus_Cursor_State=1;
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				//*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_NULL;//�����ʡ��
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				reflash=TRUE;
				msg_widget=MESSAGE_WIDGET_EDIT;
			}
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL)
			{
				//*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;//�������ʡ��
				*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_0P;
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				Focus_Cursor_State++;
				reflash=TRUE;
				msg_widget=MESSAGE_WIDGET_EDIT;
			}
		}
	}
	//3.�����˸�
	// a.����Ѿ���0,�򲻱�,����� FOCUS_CURSOR_ALL.����0
	// b.���û��С������,���˸�
	// b.SELF�������2P,���1P,�����1P,���0P,�����0P,���NULL
	else if(key==KEY_TO_GUI_BACK)
	{
		//����ģʽ
		if(__base_property_edit_type==BASE_PRO_EDIT_TYPE_PW)
		{
			//�����FOCUS_CURSOR_ALL״̬,���0
			if(Focus_Cursor_State==FOCUS_CURSOR_ALL)
			{
				*ptmp=0;
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_PASS_NUM_MASK;
			}
			//����λ����0,�����Լ�1
			else if((*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK)
			{
				(*(the_edit->p_self_property))--;
				*ptmp/=10;
				Focus_Cursor_State--;
			}
			else
			{
				return MESSAGE_NULL;
			}
		}
		else
		{
			//a.ֻ��һ��0
			if((*ptmp==0)&&(__self_property_edit_state==SELF_PRO_EDIT_STATE_NULL))
			{
				return MESSAGE_NULL;
			}
			//FOCUS_CURSOR_ALL��ȫ��
			if(Focus_Cursor_State==FOCUS_CURSOR_ALL)
			{
				*ptmp=0;
				Focus_Cursor_State=1;
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				//*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_NULL;//�����ʡ��
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				msg_widget=MESSAGE_WIDGET_EDIT;
				reflash=TRUE;
			}

			//8����
			if(__base_property_edit_type!=BASE_PRO_EDIT_TYPE_O8)
			{
				tmp=*ptmp/8;
			}
			//c. 0P->NULL(0.->0)
			if(__self_property_edit_state==SELF_PRO_EDIT_STATE_0P)
			{
				*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
				//*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_NULL;//�����ʡ��
				__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
			}
			else
			{	
				//c. 1P->0P(0.1->0.)
				if(__self_property_edit_state==SELF_PRO_EDIT_STATE_1P)
				{
					*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
					*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_0P;
					__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				}
				//c. 2P->1P(0.12->0.1)
				else if(__self_property_edit_state==SELF_PRO_EDIT_STATE_2P)
				{
					*(the_edit->p_self_property)&=~SELF_PRO_EDIT_STATE_MASK;
					*(the_edit->p_self_property)|=SELF_PRO_EDIT_STATE_1P;
					__self_property_edit_state=(*(the_edit->p_self_property))&SELF_PRO_EDIT_STATE_MASK;
				}

				//b.���´�������NULL�����
				tmp=*ptmp/10;
				*ptmp=tmp;
			}
			Focus_Cursor_State--;
			if(Focus_Cursor_State==0)
				Focus_Cursor_State=1;
		}
		reflash=TRUE;
		msg_widget=MESSAGE_WIDGET_EDIT;
	}
	//4.ȷ��,-ȷ�ϼ���Ҫӭ��С�������ʾԭ��.�ο�SetEditTmp
	// a.����С������
	// b.���������,���ж���Сֵ,������Сֵ,����һ����Сֵ,���ĳ���,��ʱ�����ж����ֵ
	// d.���������,...
	else if(key==KEY_TO_GUI_ENTER)
	{
		reflash=__FixEditState(the_edit);
	//	reflash=1;
		msg_widget=MESSAGE_WIDGET_ENTER;
	}

	if(reflash==TRUE)
	{	
		__Draw(the_edit);
	}

	return msg_widget;
}


/**************************************************************
** ������:EditCursorFlash
** ����:EDIT�Ĺ����˸
** ע������:��GuiSysTick20ms�е���,ϵͳ�涨500MS����һ��,����ˢ�¹��
***************************************************************/
void EditCursorFlash(bool flash)
{
	static DX_WIDGET* old_widget=NULL;
	static LOC_Y y0;
	DX_EDIT* the_edit;
	static u8 len,bcd;
	u8 num_len;
	LOC_X x0;
	
	if((pFocus_Widget->base_property&BASE_PRO_TYPE_MASK)!=BASE_PRO_EDIT)
	{
		return;
	}
	//�������µĿؼ�,����»�������
	if(old_widget!=pFocus_Widget)
	{
		old_widget=pFocus_Widget;
		the_edit=(DX_EDIT*)old_widget;
		if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT16)//�ȼ�����������߶�
		{
			len=16;
		}
		else if((the_edit->widget.base_property&BASE_PRO_FONT_MASK)==BASE_PRO_FONT24)
		{
			len=24;
		}
		else
		{
			len=32;
		}
		y0=the_edit->widget.y;
		y0=GetStringLocYFromFRAME(y0,the_edit->widget.height,len);
		bcd=GET_EDIT_BCD_LEN(the_edit);
	}

	if(Focus_Cursor_State!=FOCUS_CURSOR_ALL)
	{
		__SetOperationalWidget((DX_EDIT*)old_widget);
		//��BCD���Ե�EDIT,���λ����Focus_Cursor_State����
		if(!bcd)
		{
			num_len=Focus_Cursor_State;
		}
		//BCD����EDIT,�޹��
		else
		{
			return;
		}
		//���ݸ���������һ�γ���
		if((*(((DX_EDIT*)old_widget)->p_tmp))<0)
		{
			num_len++;
		}
	
		x0=((DX_EDIT*)old_widget)->widget.x+num_len*len/2+4;
		__TFTShowCursor(x0,y0,len,flash);
	}
}

/**************************************************************
** ������:EditPassMatch
** ����:�����ж������Ƿ���ȷ
** ע������:��ȷ����TRUE,ע�����ﻹ������λ�����ж�
***************************************************************/
bool EditPassMatch(DX_EDIT* the_edit,u32 num)
{
	u8 i;
	i=(*(the_edit->p_self_property))&SELF_PRO_EDIT_PASS_NUM_MASK;
	if(i==GetNumLen(*(the_edit->p_tmp)))
	{
		if(num==*(the_edit->p_tmp))
		{
			return TRUE;
		}
	}
	return FALSE;
}
