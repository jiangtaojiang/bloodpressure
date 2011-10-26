/**************************************************************
ģ����:widget.c 
����:�ؼ���ͨ�ù��ܺ���
���¼�¼:2009-11-18 ���뻭ͨ��3D����,����EDIT,CHECK����,�ð���3D��,������͹��
***************************************************************/


#include "stm32f10x.h"

#include "GUIDef.h"
#include "WIDGET.h"
#include "WINDOW.h"
#include "mem.h"

DX_WIDGET* pFocus_Widget;	//ָ��ǰ����ռ��ָ��
u8 Focus_Cursor_State;		//��ǰ����״̬�ĸ�����¼����
/**************************************************************
** ������:GetStringXFromFRAME/GetStringYFromFRAME
** ����:���ݱ߿��С���ַ�����,��ȡ�ַ�Ӧ�е����
** ע������:��ȡX��Ҫ�ı���ָ��Ϳ��,��ȡY��Ҫ�ı��ĸ߶�,һ����16��24 
** 			*_*���������һЩ��������,ʹ������ĺ������Լ�,��Ҫ��EDIT����
***************************************************************/
//str_length��һ���ַ��Ŀ��,ע����һ���ַ�,�����������ַ�.
LOC_X GetStringLocXFromFRAME(LOC_X x0,LOC_X length,u8 str_length,char*str)
{
	u8 i=0;
	while(i<30)//���������ַ�����ݶ�Ϊ30
	{
		if(*str++==0)
			break;
		i++;
	}
	return(x0+(length+1-i*str_length)/2);
}

LOC_Y GetStringLocYFromFRAME(LOC_Y y0,LOC_Y height,u8 str_height)
{
	return(y0+(height-str_height+2)/2);//����+2��Ϊ�ֿ������~
}

//09.11.26���ӵļ������㸨������,��ȡ�߶����߶��м����ʼλ��
u32 GetMiddleFromLine(u32 xy0,u32 xy1,u32 length)
{
	return(xy0+(xy1-xy0-length)/2);
}

//������Ķ��뺯��,����XY��
//��ȡ�ַ����ֽ���,����'��',����2 'A',����1
u32 GetStringSize(char* str)
{
	u8 i=0;
	while(i<30)//���������ַ�����ݶ�Ϊ30
	{
		if(*str++==0)
			break;
		i++;
	}
	return i;
}




/**************************************************************
** ������:__Draw3DFrameA/B
** ����:��3D��,����ͨ�õ�WINDOW 3D��
** ע������:A�ǰ���,B��͹��
***************************************************************/
void __Draw3DFrameA(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height)
{
	LOC_X x1;
	LOC_X y1;

	x1=x0+length-1;
	y1=y0+height-1;
	//���Ͽ�
	TFTHLine(x0,y0,length-1,COLOR_FRAME_A_OU);
	TFTVLine(x0,y0+1,height-2,COLOR_FRAME_A_OU);
	//���¿�
	TFTHLine(x0,y1,length,COLOR_FRAME_A_OD);
	TFTVLine(x1,y0,height-1,COLOR_FRAME_A_OU);
	//�ڿ���
	TFTHLine(x0+1,y0+1,length-3,COLOR_FRAME_A_IU);
	TFTVLine(x0+1,y0+2,height-4,COLOR_FRAME_A_IU);
	//�ڿ���
	TFTHLine(x0+1,y1-1,length-2,COLOR_FRAME_A_ID);
	TFTVLine(x1-1,y0+1,height-3,COLOR_FRAME_A_IU);
}
//͹��
void __Draw3DFrameB(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height)
{
	LOC_X x1;
	LOC_X y1;

	x1=x0+length-1;
	y1=y0+height-1;
	//���
	TFTRect(x0,y0,length,height,COLOR_FRAME_B);

	//���п���
	TFTHLine(x0+1,y0+1,length-3,COLOR_FRAME_B_U);
	TFTVLine(x0+1,y0+2,height-4,COLOR_FRAME_B_U);
	
	//���п���
	TFTHLine(x0+1,y1-1,length-3,COLOR_FRAME_B_D);
	TFTVLine(x1-1,y0+1,height-2,COLOR_FRAME_B_D);

	//���ڿ�
	TFTHLine(x0+2,y1-2,length-4,COLOR_FRAME_B_I);
	TFTVLine(x1-2,y0+2,height-5,COLOR_FRAME_B_I);
}

//����2--�������º�İ���*_*��������ǰ�����,��������ɲ�����,������պ��ٸ�
void __Draw3DFrameC(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height)
{
	TFTRect(x0,y0,length,height,COLOR_FRAME_B);
	TFTRect(x0+1,y0+1,length-2,height-2,COLOR_FRAME_B_I);
}



/**************************************************************
** ������:__PointInRect__
** ����:���һ��POINT�Ƿ��ھ�����
** ע������:Ϊ�˼ӿ�Ч��,���εı�﷽ʽ����x0,y0,length,height. 
** 			*_*���ڱ����ϲ����ھ����� 
***************************************************************/
bool __PointInRect__(LOC_X x,LOC_Y y,LOC_X x0,LOC_Y y0,u16 length,u16 height)
{
	if((x>x0)&&(x<(x0+length))&&(y>y0)&&(y<(y0+height)))
	{
		return TRUE;
	}
	else 
		return FALSE;
}
/**************************************************************
** ������:__PointInRect
** ����:���һ��POINT�Ƿ��ھ�����
** ע������:��__PointInRect__���,����ʹ����x0,y0,x1,y1���ж�
** 			*_*���ڱ����ϲ����ھ����� 
***************************************************************/
bool __PointInRect(LOC_X x,LOC_Y y,LOC_X x0,LOC_Y y0,LOC_X x1,LOC_Y y1)
{
	if((x>x0)&&(x<x1)&&(y>y0)&&(y<y1))
	{
		return TRUE;
	}
	else 
		return FALSE;
}

/**************************************************************
** ������:__MakeSearchRectL/R/U/D
** ����:����������ĸ�����(������ĸ���),����һ���µ����ž���,��Ҫ���ڷ�����ƶ��ؼ����Զ���������
** ע������:���ŵ����������,�򷵻�FALSE
***************************************************************/
#define SEARCH_WINC_AREA		25		//���ÿ������
#define SEARCH_DINC_AREA		50		//���ÿ������
static bool __MakeSearchRectL(LOC_X* px0,LOC_Y* py0,LOC_X* px1,LOC_Y* py1)
{
	bool w0_state=TRUE,w1_state=TRUE,d_state=TRUE;
	
	//�������
	if(*px0==MIN_X)
		d_state=FALSE;
	else if(*px0>SEARCH_DINC_AREA)
		*px0-=SEARCH_DINC_AREA;
	else
		*px0=MIN_X;

	//�������
	if(*py0==MIN_Y)
		w0_state=FALSE;
	else if(*py0>SEARCH_WINC_AREA)
		*py0-=SEARCH_WINC_AREA;
	else
		*py0=MIN_Y;

	if(*py1==MAX_Y)
		w1_state=FALSE;
	else if(*py1<MAX_Y-SEARCH_WINC_AREA)
		*py1+=SEARCH_WINC_AREA;
	else
		*py1=MAX_Y;

	if((w0_state!=FALSE)||(w1_state!=FALSE)||(d_state!=FALSE))
	{
		return TRUE;
	}
	return FALSE;
}

static bool __MakeSearchRectR(LOC_X* px0,LOC_Y* py0,LOC_X* px1,LOC_Y* py1)
{
	bool w0_state=TRUE,w1_state=TRUE,d_state=TRUE;
	
	//�������
	if(*px1==MAX_X)
		d_state=FALSE;
	else if(*px1<MAX_X-SEARCH_DINC_AREA)
		*px1+=SEARCH_DINC_AREA;
	else
		*px1=MAX_X;

	//�������
	if(*py0==MIN_Y)
		w0_state=FALSE;
	else if(*py0>SEARCH_WINC_AREA)
		*py0-=SEARCH_WINC_AREA;
	else
		*py0=MIN_Y;

	if(*py1==MAX_Y)
		w1_state=FALSE;
	else if(*py1<MAX_Y-SEARCH_WINC_AREA)
		*py1+=SEARCH_WINC_AREA;
	else
		*py1=MAX_Y;

	if((w0_state!=FALSE)||(w1_state!=FALSE)||(d_state!=FALSE))
	{
		return TRUE;
	}
	return FALSE;
}
static bool __MakeSearchRectU(LOC_X* px0,LOC_Y* py0,LOC_X* px1,LOC_Y* py1)
{
	bool w0_state=TRUE,w1_state=TRUE,d_state=TRUE;
	
	//�������
	if(*py0==MIN_Y)
		d_state=FALSE;
	else if(*py0>SEARCH_DINC_AREA)
		*py0-=SEARCH_DINC_AREA;
	else
		*py0=MIN_Y;

	//�������
	if(*px0==MIN_X)
		w0_state=FALSE;
	else if(*px0>SEARCH_WINC_AREA)
		*px0-=SEARCH_WINC_AREA;
	else
		*px0=MIN_X;

	if(*px1==MAX_X)
		w1_state=FALSE;
	else if(*px1<MAX_X-SEARCH_WINC_AREA)
		*px1+=SEARCH_WINC_AREA;
	else
		*px1=MAX_X;

	if((w0_state!=FALSE)||(w1_state!=FALSE)||(d_state!=FALSE))
	{
		return TRUE;
	}
	return FALSE;
}
static bool __MakeSearchRectD(LOC_X* px0,LOC_Y* py0,LOC_X* px1,LOC_Y* py1)
{
	bool w0_state=TRUE,w1_state=TRUE,d_state=TRUE;
	
	//�������
	if(*py1==MAX_Y)
		d_state=FALSE;
	else if(*py1<MAX_Y-SEARCH_DINC_AREA)
		*py1-=SEARCH_DINC_AREA;
	else
		*py1=MAX_Y;

	//�������
	if(*px0==MIN_X)
		w0_state=FALSE;
	else if(*px0>SEARCH_WINC_AREA)
		*px0-=SEARCH_WINC_AREA;
	else
		*px0=MIN_X;

	if(*px1==MAX_X)
		w1_state=FALSE;
	else if(*px1<MAX_X-SEARCH_WINC_AREA)
		*px1+=SEARCH_WINC_AREA;
	else
		*px1=MAX_X;

	if((w0_state!=FALSE)||(w1_state!=FALSE)||(d_state!=FALSE))
	{
		return TRUE;
	}
	return FALSE;
}
/**************************************************************
** ������:FeelingDistanceCnt 
** ���� :��ȡ������֮��ĸ����Ӿ�����
** ע������: ��ʱ�ܼ򵥵�X,Y����,��Ϊ����Ӿ�������ݺ��й�ϵ,��ʱ�������Ҫ�ֿ���������
***************************************************************/
u32 FeelingDistanceCnt(LOC_X x0,LOC_Y y0,LOC_X x1,LOC_Y y1)
{
	u32 feel_distance;
	if(x0>x1)
	{
		feel_distance=x0-x1;
	}
	else
	{
		feel_distance=x1-x0;
	}

	if(y0>y1)
	{
		feel_distance+=y0-y1;
	}
	else
	{
		feel_distance+=y1-y0;
	}
	return feel_distance;
}

/**************************************************************
** ������:GetNumLen
** ����:��ȡ���ֵ�λ��
** ע������:��ȡ����10���Ƶ�λ,��СҲ�᷵��1,��Ϊ0Ҳռ��1λ
			����Ǹ���,����ת��Ϊ����������(09.7.10)
***************************************************************/
u8 GetNumLen(s32 num)
{
	if(num<0)//ת��Ϊ����
	{
		num=0-num;
	}

	if(num>9999999)
		return 8;
	if(num>999999)
		return 7;
	if(num>99999)
		return 6;
	if(num>9999)
		return 5;
	if(num>999)
		return 4;
	if(num>99)
		return 3;
	if(num>9)
		return 2;
	return 1;
}
/**************************************************************
** ������:NumToString/O8
** ����:��ʮ�������ֱ任���ַ���
** ע������:��ȻNUM��32λ����,������ϵͳ�涨�����ֳ��Ȳ��ܳ���8λ,���һ���Ӧ�ö��ԱȽ��㹻 
**  		�����ַ������� ,���صĳ��Ȳ���'\0',���������0,���ַ�������ҲΪ1,
**  		֧�ָ�����ʾ(09,7.10)
**  		O8��׺��,����8��ֹ��ʾ��ʽ,���ص�Ҳ��8������ʾʱ����ֳ�
***************************************************************/
u8 NumToString(s32 num,char* str)
{
	u8 i;
	u8 j;
	u8 num_data[8]={0,0,0,0,0,0,0,0};
	
	if(num<0)//ת��Ϊ����
	{
		num=0-num;
	}
	
	while(num>99999999)
	{
		num-=100000000;		//��֤���ֲ�����8λʮ����
	}

	//2.���ַֽ�-Ҳ�����ó�������,�ĸ��ٶȿ��д���֤
	while(num>9999999)
	{
		num_data[7]++;num-=10000000;
	}
	while(num>999999)
	{
		num_data[6]++;num-=1000000;
	}
	while(num>99999)
	{
		num_data[5]++;num-=100000;
	}
	while(num>9999)
	{
		num_data[4]++;num-=10000;
	}
	while(num>999)
	{
		num_data[3]++;num-=1000;
	}
	while(num>99)
	{
		num_data[2]++;num-=100;
	}
	while(num>9)
	{
		num_data[1]++;num-=10;
	}
	while(num>0)
	{
		num_data[0]++;num-=1;
	}

	i=8;
	j=0;
	do
	{
		i--;
		if((num_data[i])||(j!=0))
		{
			*str++='0'+num_data[i];
			if(j==0)
			{
				j=i+1;
			}
		}
	}while(i!=0);
	//���ڼӵĸĶ�,��ʹNUMΪ0,ҲҪ���ַ���0����
	if(j==0)
	{
		*str++='0';
		j=1;
	}
	*str='\0';
	return j;
}
u8 NumToStringO8(s32 num,char* str)
{
	u8 i;
	u8 j;
	u8 num_data[8]={0,0,0,0,0,0,0,0};
	
	if(num<0)//ת��Ϊ����
	{
		num=0-num;
	}
	
	while(num>077777777)
	{
		num-=0100000000;		
	}

	//2.���ַֽ�-Ҳ�����ó�������,�ĸ��ٶȿ��д���֤
	while(num>07777777)
	{
		num_data[7]++;num-=010000000;
	}
	while(num>0777777)
	{
		num_data[6]++;num-=01000000;
	}
	while(num>077777)
	{
		num_data[5]++;num-=0100000;
	}
	while(num>07777)
	{
		num_data[4]++;num-=010000;
	}
	while(num>0777)
	{
		num_data[3]++;num-=01000;
	}
	while(num>077)
	{
		num_data[2]++;num-=0100;
	}
	while(num>7)
	{
		num_data[1]++;num-=010;
	}
	while(num>0)
	{
		num_data[0]++;num-=1;
	}

	i=8;
	j=0;
	do
	{
		i--;
		if((num_data[i])||(j!=0))
		{
			*str++='0'+num_data[i];
			if(j==0)
			{
				j=i+1;
			}
		}
	}while(i!=0);
	//���ڼӵĸĶ�,��ʹNUMΪ0,ҲҪ���ַ���0����
	if(j==0)
	{
		*str++='0';
		j=1;
	}
	*str='\0';
	return j;
}


/**************************************************************
** ������:__AutoMoveWidget
** ����:�Զ����㲢�ƶ��ؼ�����
** ע������: �˺�����KeyToChangeWidget����, 
**  		����˺����������µĽ���,�򷵻��µĽ���ָ�� ,���򷵻�NULL
**  		Ϊ�����Ч��,����ķ����ڴ��װ��������Կ�����static�������Զ��ж��Ƿ���ˢ�¶�ѡ���Ը���,��ʱΪÿ�ζ�����
**  		10.2.19:�������ž����ƶ����㻹������,�Ľ�Ϊ�����ž������ҵ��Ŀؼ���
**  		��һ���Ա�,�ҵ������Ŀؼ�
**  		*_*���иĽ����Ż���������
***************************************************************/
static DX_WIDGET* __AutoMoveWidget(DX_WIDGET* the_widget,GUIKEY key)
{
	u8 widget_no,i,self,next_no;
	LOC_X* widget_x;
	LOC_Y* widget_y;			//���ĵ��
	u32 tmp_distance,min_distance=0xffffffff;	//�Ӿ�����
	
	LOC_X search_x0,search_x1;	//�����ľ���
	LOC_Y search_y0,search_y1;
	bool (*make_search_rect)(LOC_X*,LOC_Y*,LOC_X*,LOC_Y*);
	DX_WINDOW* the_window;
//	DX_WIDGET* tmp_widget;
	
	
	the_window=the_widget->far_win;
	widget_no=the_window->max_widget;
	//������ڵĿؼ�������һ��,��ֱ�ӷ���
	if(widget_no<=1)
	{
		return NULL;
	}
	//����ؼ�����,��ֱ�ӷ���
	if(*(the_widget->p_ext_property)&EXT_PRO_FLOCK)
	{
		return NULL;
	}
	//���ݱ��������еĿؼ���,�������껺������
	widget_x=BCNew(widget_no*sizeof(LOC_X));
	if(widget_x==NULL)
	{
		return NULL;
	}
	widget_y=BCNew(widget_no*sizeof(LOC_Y));
	if(widget_y==NULL)
	{
		BCDelete(widget_x);
		return NULL;
	}
	 
	//��ȡ�����ؼ�����������,��ʹĳЩ�ؼ�Ϊֻ��
	for(i=0;i<widget_no;i++)
	{
		widget_x[i]=the_window->p_child[i]->x+the_window->p_child[i]->length/2;
		widget_y[i]=the_window->p_child[i]->y+the_window->p_child[i]->height/2;
	}
	//��ȡ���ؼ����
	self=*(the_window->p_focu_no);
	   
	//���������������-����������ʵΪһ����
	search_x0=widget_x[self];
	search_x1=widget_x[self];
	search_y0=widget_y[self];
	search_y1=widget_y[self];

	//ѡȡ�������ɺ���
	if(key==KEY_TO_GUI_LEFT)	
		make_search_rect=__MakeSearchRectL;	//������
	else if(key==KEY_TO_GUI_RIGHT)
		make_search_rect=__MakeSearchRectR;	//������
	else if(key==KEY_TO_GUI_UP)
		make_search_rect=__MakeSearchRectU;	//������
	else if(key==KEY_TO_GUI_DOWN)
		make_search_rect=__MakeSearchRectD;	//������

	while( (*make_search_rect)(&search_x0,&search_y0,&search_x1,&search_y1) !=FALSE)//һֱ����������������
	{
		//�����ؼ�
		for(i=0;i<widget_no;i++)
		{
			//�Լ�������
			if(i==self)
				continue;
			//��ֹ����Ŀؼ�,������
			if( ((the_widget->far_win->p_child[i]->base_property&BASE_PRO_FORBID_FOCUS)))
				continue;
			//��Ч�ؼ�������
			if((*(the_widget->far_win->p_child[i]->p_ext_property))&EXT_PRO_DISABLE)
				continue;
			//���οؼ�������
			if((*(the_widget->far_win->p_child[i]->p_ext_property))&EXT_PRO_HIDE)
				continue;
			
			//�жϵ�ǰ���ǲ���������������
			if(__PointInRect(widget_x[i],widget_y[i],search_x0,search_y0,search_x1,search_y1)==TRUE)
			{
				//���������������,���������Ӿ�����
				tmp_distance=FeelingDistanceCnt(widget_x[self],widget_y[self],widget_x[i],widget_y[i]);
				if(tmp_distance<min_distance)
				{
					min_distance=tmp_distance;
					next_no=i;
				}
			}
		}
		//�ҵ��������
		if(min_distance!=0xffffffff)
		{
			//�ͷ��ڴ�
			BCDelete(widget_x);
			BCDelete(widget_y);
			//�����ƶ����Ŀؼ�
			return the_widget->far_win->p_child[next_no];
		}
	}


	//������һ��,�������������
	//�ͷ��ڴ�
	BCDelete(widget_x);
	BCDelete(widget_y);
	return NULL;
}

/**************************************************************
** ������:ReDrawWidget
** ���� :�ػ�ؼ�
** ע������:���������ػ�,����һЩ�ؼ���ˢ�µ�ʱ�����,���ı��־λ
***************************************************************/
void ReDrawWidget(DX_WIDGET *the_widget)
{
	u8 widget_type;
	widget_type=the_widget->base_property&BASE_PRO_TYPE_MASK; //��ȡ�ؼ�����
	switch(widget_type)
	{
		case BASE_PRO_BUTTON:
			ReDrawButton( (DX_BUTTON*)the_widget);
			break;
			
		case BASE_PRO_EDIT:
			ReDrawEdit( (DX_EDIT*)the_widget);
			break;

		case BASE_PRO_COMBO:
			ReDrawCombo( (DX_COMBO*)the_widget);
			break;
		case BASE_PRO_CHECK:
			ReDrawCheck( (DX_CHECK*)the_widget);
			break;
		case BASE_PRO_PROGRESS:
			ReDrawProgress( (DX_PROGRESS*)the_widget);
			break;
	}
}

/***************************
**������:GuiWidgetDraw
**����:���ֿؼ������������ؼ�
**ע������:�������һ�������WINDOW���ֵ�ʱ��Ż���������������ؼ�,����������жԾ�̬���õĿؼ����ݽ���һ���̶ȵļ�� 
**			�����Զ�������һЩ��̬����ʱ�����������Ĵ���:��while(1)����ѭ��,ʹ���û����ʼ�ĵ����о����ҵ���̬����ʱ������� 
***************************/
void GuiWidgetDraw(DX_WIDGET *the_widget)
{
	u8 widget_type;
	
	widget_type=the_widget->base_property&BASE_PRO_TYPE_MASK; //��ȡ�ؼ�����
	//*_*���ӿؼ�SHOW����
	GuiWidgetReponse(the_widget,WIDGET_PARAM_SHOW);
	switch(widget_type)
	{
		//case BASE_PRO_WINDOW:
		//..WINDOW����ʡ�Ե�,��WindowDraw��������
		//break;

		case BASE_PRO_BUTTON:
			//�����ڸ���ʾ������ʱ��,���������ڰ���״̬
			*(((DX_BUTTON*)the_widget)->p_self_property)&=~SELF_PRO_PUSH_DOWN;
			GuiButtonDraw( (DX_BUTTON*)the_widget);
			break;
			
		case BASE_PRO_EDIT:
			GuiEditDraw( (DX_EDIT*)the_widget);
			break;

		case BASE_PRO_COMBO:
			GuiComboDraw( (DX_COMBO*)the_widget);
			break;
		case BASE_PRO_CHECK:
			GuiCheckDraw( (DX_CHECK*)the_widget);
			break;
		case BASE_PRO_PROGRESS:
			GuiProgressDraw( (DX_PROGRESS*)the_widget);
			break;
/*		case OBJ_STATIC:
		//	GuiStaticDraw( (DX_STATIC*)the_obj,op);
			break;
		case OBJ_STATICA:
		//	GuiStaticADraw( (DX_STATICA*)the_obj,op);
			break;
		case OBJ_PROGRAMBAR:
		//	GuiProgramBarDraw((DX_PROGRAMBAR*)the_obj);
			break;			
*/		default:
			break;
	}
}

/**************************************************************
** ������:UpdateWidget
** ����:ˢ�¿ؼ�
** ע������:��ʱû��,
***************************************************************/
void UpdateWidget(DX_WIDGET* the_widget)
{
	
}

/**************************************************************
** ������:SetWidgetFlash
** ����:���ÿؼ���˸����
** ע������:�����ı�FLASH_EN��־λ,��˸�Ĺ�����GUI TICK�н��� 
** 			ע��FLASHλ���ֳ����Զ��ı��,��Ϊֻ�ܸı�ENλ 
***************************************************************/
void SetWidgetFlash(DX_WIDGET* the_widget,bool State)
{
	if(the_widget!=NULL)
	{
		if(State==TRUE)
		{
			*(the_widget->p_ext_property)|=EXT_PRO_FLASH_EN;
		}
		else
		{
			*(the_widget->p_ext_property)&=~EXT_PRO_FLASH_EN;

		}
	}
}

/**************************************************************
** ������:SetWidgetFLock 
** ���� :���ÿؼ�����
** ע������: 
***************************************************************/
void SetWidgetFLock(DX_WIDGET* the_widget,bool State)
{
	if(the_widget!=NULL)
	{
		if(State==TRUE)
		{
			*(the_widget->p_ext_property)|=EXT_PRO_FLOCK;
		}
		else
		{
			*(the_widget->p_ext_property)&=~EXT_PRO_FLOCK;
		}
	}
}

/**************************************************************
** ������:SetWidgetHide
** ����:���ÿؼ����ع���
** ע������:���˸ı�EXT������,����Ҫˢ�¿ؼ�-���ݿؼ���ǰ�Ƿ������ˢ�� 
** 			����ؼ�����������,��Ҫ��֤��ǽ� 
***************************************************************/
void SetWidgetHide(DX_WIDGET* the_widget,bool State)
{
	if(the_widget!=NULL)
	{
		if(State==TRUE)
		{
			//֮ǰ�����ص�-�������
			if(!(*(the_widget->p_ext_property)&EXT_PRO_HIDE))
			{
				if(IsWidgetAvailable(the_widget))
				{
					ReDrawWidget(the_widget);
				}
				*(the_widget->p_ext_property)|=EXT_PRO_HIDE;//**�ر�ע��,������IsWidgetAvailable������λHIDE,����ReDrawWidget�Ͳ���Ч
				SetNextFocus(the_widget);					//��֤��ǽ�
			}
		}
		else
		{
			//֮ǰ�����ص�
			if(*(the_widget->p_ext_property)&EXT_PRO_HIDE)
			{
				*(the_widget->p_ext_property)&=~EXT_PRO_HIDE;
				if(IsWidgetAvailable(the_widget))
				{
					ReDrawWidget(the_widget);
				}
			}
		}
	}
}

/**************************************************************
** ������:SetWidgetDisable
** ����:���ÿؼ�������
** ע������:���˸ı�EXT������,��Ҫˢ�¿ؼ�-���ݿؼ���ǰ�Ƿ���� 
** 			����ؼ������ò�����,�豣֤��ǽ� 
***************************************************************/
void SetWidgetDisable(DX_WIDGET* the_widget,bool State)
{
	if(State==TRUE)
	{
		//֮ǰ��DISABEL
		if(!(*(the_widget->p_ext_property)&EXT_PRO_DISABLE))
		{
			*(the_widget->p_ext_property)|=EXT_PRO_DISABLE;
			if(IsWidgetAvailable(the_widget))
			{
				ReDrawWidget(the_widget);
			}
			SetNextFocus(the_widget);					//��֤��ǽ�
		}
	}
	else
	{
		//֮ǰ��DISABLE
		if(*(the_widget->p_ext_property)&EXT_PRO_DISABLE)
		{
			*(the_widget->p_ext_property)&=~EXT_PRO_DISABLE;
			if(IsWidgetAvailable(the_widget))
			{
				ReDrawWidget(the_widget);
			}
		}
	}
}

/**************************************************************
** ������:__SetWidgetFocus
** ����:���ý���
** ע������:�ɹ��򷵻ؾɽ��� ���򷵻�NULL.�˺���û�е��û����㹦��,�����ı䴰�ھ���Ϳؼ������״̬
***************************************************************/
static DX_WIDGET* __SetWidgetFocus(DX_WIDGET* the_widget)
{
	u8 i;
	DX_WINDOW* the_window;
	DX_WIDGET* old_widget;
	if((*(the_widget->p_ext_property))&EXT_PRO_FOCUS)//�Ѿ��ǽ�����,��������
	{
		return NULL;
	}
	if((*(the_widget->p_ext_property))&EXT_PRO_HIDE)//HIDE����,��������
	{
		return NULL;
	}
	if((*(the_widget->p_ext_property))&EXT_PRO_DISABLE)//DISABLE����,��������
	{
		return NULL;
	}
	if(the_widget->base_property&BASE_PRO_FORBID_FOCUS)//BASE_PRO_FORBID_FOCUS����,��������
	{
		return NULL;
	}

	//����Ҫ�ý���Ŀؼ�,�����������,����������������
	the_window=the_widget->far_win;

	for(i=0;i<the_window->max_widget;i++)
	{
		if(the_window->p_child[i]==the_widget)						
		{
			//����ɽ���
			old_widget=the_window->p_child[*(the_window->p_focu_no)];
			//�����µĽ����
			*(the_window->p_focu_no)=i;		
			goto true_widget;
		}
	}
	return(NULL);

true_widget:
	//ȥ���ɽ���
	*(old_widget->p_ext_property)&=~EXT_PRO_FOCUS;

	//�����½���
	*(the_widget->p_ext_property)|=EXT_PRO_FOCUS;
	//����pFocus
	if(the_window==pNow_Window)
	{
		pFocus_Widget=the_widget;
	}
	return old_widget;
}

/************************************************************** 
** ������:SetNextFocus
** ����:ϵͳ����ĸı佹��,
** ע������:��ʱ�涨,ϵͳ�ı佹��,�������κ���Ӧ����. 
**  		����������ϵͳ�ı�,���Բ��۵�ǰ������ʲô״̬,���㶼Ӧ���ƶ�����һ���ؼ�.
** 			Ŀ���Ǿ�������ʹ�� �ò����ؼ��ǽ�
************************************************************/
void SetNextFocus(DX_WIDGET* the_widget)
{
	u32 i;
	u8 self;
	DX_WINDOW* the_window;
	//�����ؼ��ǽ���,����������������
	if((!(*(the_widget->p_ext_property))&EXT_PRO_FOCUS))
	{
		return;
	}
	the_window=the_widget->far_win;
	self=*(the_window->p_focu_no);
	for(i=0;i<the_window->max_widget;i++)
	{
		if(i==self)
			continue;
		if(__SetWidgetFocus(the_window->p_child[i])!=NULL)
			return;
	}
}
/**************************************************************
**������:KeyToChangeWidget
**����:�����ļ��Կؼ���Ӱ��
**ע������:�˺�����KeyToGui����,����ǰ��ȷ��the_widget�ǿ� ����Ϊ��ǰ����.
** 			���������ı�Focus_Cursor_State
** 			�ı��ͬʱ�������Ӧ����
***************************************************************/
void KeyToChangeWidget(DX_WIDGET* the_widget,GUIKEY key)
{
	DX_WIDGET* new_widget;
	DX_WIDGET* old_widget;
	MESSAGE_WIDGET msg_widget;

	//��ǰ�ؼ�Ϊ������,����Ҫ����
	if((the_widget->base_property)&BASE_PRO_FORBID_FOCUS)
	{
		return ;
	}

	//������������пؼ�����ͬһ����ʽ
	if((key>=KEY_TO_GUI_LEFT)&&(key<=KEY_TO_GUI_DOWN))		//�����
	{
		new_widget=__AutoMoveWidget(the_widget,key);
		if(new_widget!=NULL)	//�ƶ����µĽ���
		{
			//�����µĽ���
			old_widget=__SetWidgetFocus(new_widget);
			if(old_widget!=NULL)
			{
				//����ʧȥ������Ӧ����FOCUS_LOST
				//..
				GuiWidgetReponse((DX_WIDGET*)old_widget,WIDGET_PARAM_FOCUS_LOST);
				//���ƾɿؼ�
				ReDrawWidget(old_widget);
				
				//�����EDIT��ı�Focus_Cursor_State״̬*_*�������е���ǿ,�ȶ�����
				if((new_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_EDIT)
				{
					Focus_Cursor_State=FOCUS_CURSOR_ALL;
				}
				//���л�ȡ������Ӧ����FOCUS_GET
				//..
				GuiWidgetReponse((DX_WIDGET*)new_widget,WIDGET_PARAM_FOCUS_GET);
				//�����¿ؼ�
				ReDrawWidget(new_widget);
			}
		}
		return;
	}

	
	//�������������ʲô�ؼ�,�ֱ���������*_*����д�ò�̫�������,�ȶ�����
	if((the_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_EDIT)//��EDIT��Ч
	{
		msg_widget=KeyToChangeEdit((DX_EDIT*)the_widget,key);
		if(msg_widget==MESSAGE_WIDGET_EDIT)//�༭
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_EDIT);
		}
		else if(msg_widget==MESSAGE_WIDGET_ENTER)//�س�
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_ENTER);
		}
	}
	else if((the_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_BUTTON)//��BUTTON
	{
		msg_widget=KeyToChangeButton((DX_BUTTON*)the_widget,key);
		if(msg_widget==MESSAGE_WIDGET_ENTER)//�س�
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_ENTER);
		}
	}
	else if((the_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_CHECK)//��CHECK
	{
		msg_widget=KeyToChangeCheck((DX_CHECK*)the_widget,key);
		if(msg_widget==MESSAGE_WIDGET_EDIT)//�༭
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_EDIT);
		}
		else if(msg_widget==MESSAGE_WIDGET_ENTER)//�س�
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_ENTER);
		}
	}
	//��combo
	else if((the_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_COMBO)//��COMBO
	{
		msg_widget=KeyToChangeCombo((DX_COMBO*)the_widget,key);
		if(msg_widget==MESSAGE_WIDGET_EDIT)//�༭
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_EDIT);
		}
		else if(msg_widget==MESSAGE_WIDGET_ENTER)//�س�
		{
			GuiWidgetReponse(the_widget,WIDGET_PARAM_ENTER);
		}
	}
}


/**************************************************************
** ������ :SetWidgetFocus
** ���� :��Ϊǿ�и�������
** ע������: �ɹ��򷵻ؾɵĽ���,ʧ�ܷ���NULL,�˺�������__SetWidgetFocus����
***************************************************************/
DX_WIDGET* SetWidgetFocus(DX_WIDGET* new_widget)
{
	DX_WIDGET* old_widget;

	old_widget=__SetWidgetFocus(new_widget);
	if(old_widget!=NULL)
	{
		//����ʧȥ������Ӧ����FOCUS_LOST
		GuiWidgetReponse((DX_WIDGET*)old_widget,WIDGET_PARAM_FOCUS_LOST);
		//���ƾɿؼ�
		GuiWidgetDraw(old_widget);
		
		//�����EDIT��ı�Focus_Cursor_State״̬*_*�������е���ǿ,�ȶ�����
		if((new_widget->base_property&BASE_PRO_TYPE_MASK)==BASE_PRO_EDIT)
		{
			Focus_Cursor_State=FOCUS_CURSOR_ALL;
		}
		//���л�ȡ������Ӧ����FOCUS_GET
		//..
		GuiWidgetReponse((DX_WIDGET*)new_widget,WIDGET_PARAM_FOCUS_GET);
		//�����¿ؼ�
		GuiWidgetDraw(new_widget);
	}
	return old_widget;
}


/*********************************************************************
** ������:WidgetFlash
** ����:����ؼ�����˸
** ע������:
**********************************************************************/
void WidgetFlash(bool flash)
{
	u32 i;
	DX_WIDGET* pFlash_Widget;

	//�������ڿؼ���ˢ��*_*����֮��Ҫ���Ǽ���Z���ϵ�ƴ�Ӵ���
	if(pNow_Window!=NULL)
	{
		//��˸�ؼ���ˢ��
		for(i=0;i<pNow_Window->max_widget;i++)
		{
			if(*(pNow_Window->p_child[i]->p_ext_property)&EXT_PRO_FLASH_EN)
			{
				pFlash_Widget=pNow_Window->p_child[i];
				if(flash==TRUE)
				{
					if(!(*(pFlash_Widget->p_ext_property)&EXT_PRO_FLASH))
					{
						*(pFlash_Widget->p_ext_property)|=EXT_PRO_FLASH;
						//ˢ��
						ReDrawWidget(pFlash_Widget);
					}
				}
				else
				{
					if(*(pFlash_Widget->p_ext_property)&EXT_PRO_FLASH)
					{
						un_flash:
						*(pFlash_Widget->p_ext_property)&=~EXT_PRO_FLASH;
						//ˢ��
						ReDrawWidget(pFlash_Widget);
					}
				}
			}
			//���пؼ�û��ENλȴ��FLASHλʱ,������Ϊȡ����ENλ��(������FALSHλ��λ),���������FLASHλ��ˢ��
			else if(*(pNow_Window->p_child[i]->p_ext_property)&EXT_PRO_FLASH)
			{
				pFlash_Widget=pNow_Window->p_child[i];
				goto un_flash;
			}
			
		}
	}
}

