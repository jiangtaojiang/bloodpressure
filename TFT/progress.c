/**************************************************************
** ģ����:progress
** ����:������ģ��-������ģ��Ϊֻ��,�����ڳ�ʼ����ʱ����Զ�DISALBE������,ʹ�ý��㲻�������ƶ�
** ���¼�¼:2009-11-25 ������ģ��,���ǵ�ʵ��һЩ��Ŀ����,����API������EDIT�Ĺ���,������ʾģ���е�����.  
**  
***************************************************************/

#include "stm32f10x.h"
#include "GuiDef.h"
#include "widget.h"
#include "window.h"

/**************************************************************
**������:__SetOperationalWidget
**����:��ȡPROGRESS�ĸ�����������,����ֵ��ȫ�ֱ�����,ʹ��֮��Ĳ�����������PROGRESS
**ע������:�����������Ч�ʺͼ��ٴ��볤��֮��,ע�⵽����progress����֮ǰ,������ô˺�����ȡ��Ϣ 
***************************************************************/
static BPRO __base_property_progress_type;
static BPRO __base_property_progress_dir;
static BPRO __base_property_progress_begin;
static BPRO __base_property_progress_style;	

static u8 __font_height;//�����ָ�*_*�ٵ�Ľ�
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
** ������:__DebugChecking
** ����:����ģʽ�¼��progress�Ƿ����,�������һ��while(1)����,���ھ�̬������ʱ������û���������
** ע������:����DEBUG_GUI�궨���������Ƿ����
***************************************************************/
#ifdef DEBUG_GUI

static void __DebugChecking(DX_PROGRESS *the_progress)
{
	if(the_progress->p_tmp==NULL)
	{
		gui_asssert_failed(0);
		while(1);
	}
	//����ǰ��ɫ����һ��
	if(the_progress->bg_color==the_progress->fg_color)
	{
		gui_asssert_failed(0);
		while(1);
	}
	//���ֵ����Сֵ֮���ϵҪ����
	if(the_progress->max_data<the_progress->min_data)
	{
		gui_asssert_failed(0);
		while(1);
	}
	//TMPֵ���ܴ������ֵ
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
** ������:__DrawFrame
** ����:�����
** ע������:����ǰҪ�Ȼ�ȡ����
***************************************************************/
static void __DrawFrame(DX_PROGRESS *the_progress)
{
	if(__base_property_progress_style==BASE_PRO_PROGRESS_STYLE_3D)//
	{
		//3D����
		__Draw3DFrameA(the_progress->widget.x,the_progress->widget.y,the_progress->widget.length,the_progress->widget.height);
	}
	else if(__base_property_progress_style==BASE_PRO_PROGRESS_STYLE_2D)
	{
		//2D
		TFTRect(the_progress->widget.x,the_progress->widget.y,the_progress->widget.length,the_progress->widget.height,BLACK);
	}
}

/**************************************************************
** ������:__Draw
** ����:�ػ�
** ע������:ֻ���ݱ�־�ػ�,���ı��־λ����Ϣλ.����ǰҪ�Ȼ�ȡ����
***************************************************************/
static void __Draw(DX_PROGRESS *the_progress)
{
	u8 percent;
	char str[15];//װ���ַ���
	u8 str_len;
	LOC_X x_str,x_progress;
	LOC_Y y_str;
	u32 tmp1,tmp2;

	///���������,����Ҫ�ø����ڱ���ɫ���
	if((*(the_progress->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		TFTRectFill(the_progress->widget.x,the_progress->widget.y,the_progress->widget.length,the_progress->widget.height,(the_progress->widget.far_win)->widget.color);
		return;
	}

	tmp1=(*(the_progress->p_tmp))-the_progress->min_data;
	tmp2=the_progress->max_data-the_progress->min_data;

	//��ȡҪ��ʾ���ַ���
	if(__base_property_progress_type==BASE_PRO_PROGRESS_TYPE_PERCENT)//�ٷֱ�
	{
		percent=(tmp1*100)/tmp2;
		str_len=NumToString(percent,str);
		str[str_len]='%';
		str[++str_len]='\0';
	}
	else if(__base_property_progress_type==BASE_PRO_PROGRESS_TYPE_NUM)//��ֵ
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
	//������ʾ���������
	y_str=GetStringLocYFromFRAME(the_progress->widget.y,the_progress->widget.height,__font_height);
	x_str=GetMiddleFromLine(the_progress->widget.x,the_progress->widget.x+the_progress->widget.length,str_len*__font_height/2);

	//�������������
	x_progress=tmp1*(the_progress->widget.length-4)/tmp2;

	//�����������ȴ�
	if(x_progress)
	{
		TFTRectFill(the_progress->widget.x+2,the_progress->widget.y+2,x_progress,the_progress->widget.height-4,the_progress->fg_color);
	}
	//���հ״�
	TFTRectFill(the_progress->widget.x+2+x_progress,the_progress->widget.y+2,the_progress->widget.length-4-x_progress,the_progress->widget.height-4,the_progress->bg_color);
	//��ʾ�ַ�
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
** ������:ReDrawProgress
** ����:�ػ������
** ע������:����progress���ɽ��㻯,�����ڻ���ʱ��ϵͳ�Զ���DISABLE����
***************************************************************/
void ReDrawProgress(DX_PROGRESS *the_progress)
{
	//���⴦��DISABLE
	*(the_progress->widget.p_ext_property)|=EXT_PRO_DISABLE;
	//0.��ȡ����
	__SetOperationalWidget(the_progress);
	//1.�����
	__DrawFrame(the_progress);
	//2.�ػ�����
	__Draw(the_progress);
}

/**************************************************************
** ������:GuiProgressDraw
** ����:��PROGRESS
** ע������:����progress���ɽ��㻯,�����ڻ���ʱ��ϵͳ�Զ���DISABLE����
***************************************************************/
void GuiProgressDraw(DX_PROGRESS *the_progress)
{
	//���⴦��DISABLE
	*(the_progress->widget.p_ext_property)|=EXT_PRO_DISABLE;
	//0.��ȡ����
	__SetOperationalWidget(the_progress);
	//1.�������ж�
	__DebugChecking(the_progress);
	//2.HIDE���Դ���
	if ((*(the_progress->widget.p_ext_property))&EXT_PRO_HIDE)
	{
		return;
	}
	// 3.�ڲ�����
	// ..
	//4.�����
	__DrawFrame(the_progress);
	//5.�ػ�����
	__Draw(the_progress);
}

/**************************************************************
** ������:SetProgressTmp/GetProgressTmp
** ����:������ʱֵ
** ע������:
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




