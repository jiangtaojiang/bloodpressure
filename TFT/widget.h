#ifndef WIDGET_H
#define WIDGET_H

typedef u32 BPRO;
typedef u8 EPRO;
typedef u8 SPRO;

/************************
**�ṹ����:DX_WIDGET
**˵��:�ؼ���������,ÿ���ؼ���ӵ�еĹ�ͬ����
*************************/
typedef struct _dx_widget
{
	LOC_X x;
	LOC_Y y;
	u16 length;
	u16 height;							//�ؼ��Ĵ�С��λ��
	BPRO base_property;					//��������
	EPRO* p_ext_property;				//��չ����*
	const struct _dx_window *far_win; 	//�����ĸ�����(���ڴ���,���һ��ΪNULL)
	void (*response_fun)(u8);			//��Ӧ����(10.1.11)
	COLOR color;						//��ɫ(���ڿؼ�,���������ɫ,����WINDOW,�Ǵ������ɫ
}const DX_WIDGET;



/**************************************************************
	��������ؼ�ͷ�ļ�
***************************************************************/
#include "BUTTON.h"
#include "EDIT.h"
#include "COMBO.h"
#include "CHECK.h"
#include "PROGRESS.h"

/**************************************************************
**�ṹ����:WIDGET_PARAM_Q
**˵��:��Ӧ����Ϣ����Ԫ��.�����ؼ�ָ�����Ӧ����
***************************************************************/
typedef struct
{
	DX_WIDGET *widget;
	u8 param;
}WIDGET_PARAM_Q;

/**************************************************************
	�ؼ��������Ա�
***************************************************************/

//BASE�ؼ�����,���ڿؼ�������-λ0,1,2,3
#define BASE_PRO_TYPE_MASK		0x0f		/*�ؼ����Ե�4λ����������ؼ�������,��֧��8�ֿؼ�,����:*/
#define BASE_PRO_WINDOW			0			//����Ҳ���ڿؼ�֮һ 09-11-19
#define BASE_PRO_BUTTON			1
#define BASE_PRO_EDIT			2
#define BASE_PRO_CHECK			3
#define BASE_PRO_STRINGEDIT		4
#define BASE_PRO_COMBO			5			/**/
#define BASE_PRO_PROGRESS		6			//������
/*
#define BASE_PRO_RADIO			2
#define BASE_PRO_STATIC			6
#define BASE_PRO_STATICA			7
#define BASE_PRO_PROGRAMBAR		8
#define BASE_PRO_MESSAGEBOX		9
*/

//BASE�ؼ����Ʊ�־-λ4,5 
#define BASE_PRO_FORBID_FOCUS	(1<<4)		//��ֹ����
#define BASE_PRO_FORBID_TOUCH	(1<<5)		//��ֹ����

//BASE�ؼ�����-λ6,7
#define BASE_PRO_FONT_MASK		0xc0
#define BASE_PRO_FONT16			0
#define BASE_PRO_FONT24			(1<<6)
#define BASE_PRO_FONT32			(2<<6)

//BASE�ؼ����� -λ18,19
#define BASE_PRO_ALIGN_MASK		(0x03<<18)
#define BASE_PRO_ALIGN_DEFAULT	(0<<18)		//Ĭ�Ϸ�ʽ����
#define BASE_PRO_ALIGN_LEFT		(1<<18)		//��
#define BASE_PRO_ALIGN_RIGHT	(2<<18)		//��
#define BASE_PRO_ALIGN_MIDDLE	(3<<18)		//�м�

/*
//BASE�ؼ���ɫ����-λ20,21,22
#define BASE_PRO_COLOR_STYLE_MASK		(0x07<<20)
#define BASE_PRO_COLOR_STYLE_DEFAULT	0			//Ĭ��:��WINDOW 3Dɫ
#define BASE_PRO_COLOR_STYLE_1			(1<<20)		//����:BLUE_BT_4;����:WHITE;3D����:BLUE_BT_7;����:RED
#define BASE_PRO_COLOR_STYLE_2			(2<<20)
#define BASE_PRO_COLOR_STYLE_3	 		(3<<20)  
#define BASE_PRO_COLOR_STYLE_4	 		(4<<20)  
#define BASE_PRO_COLOR_STYLE_5	 		(5<<20)  
#define BASE_PRO_COLOR_STYLE_6	 		(6<<20)  
#define BASE_PRO_COLOR_STYLE_7	 		(7<<20)  
*/
/**************************************************************
	����������Ա�
***************************************************************/
//BASE WIN������
#define BASE_PRO_NOT_FILL		(1<<4)		//����Ҫ���ɫ


/**************************************************************
	�ؼ���չ���Ա�
***************************************************************/
//EXT�ؼ�����
#define EXT_PRO_FOCUS			0x01		//����
#define EXT_PRO_DISABLE			0x02		//������,���ڿ�������ǻ�ɫ�ؼ�,�����õĿؼ����ܴ���Ҳ���ܻ�ȡ����
#define EXT_PRO_FLASH_EN		0x04		//��˸EN
#define EXT_PRO_HIDE			0x08		//����
#define EXT_PRO_FLASH			0x10		//��˸λ
#define EXT_PRO_FLOCK           0x20        //����
//���
#define FOCUS_CURSOR_ALL	0xff		//���״̬��,�����ָ��һ���µ�EDITʱ,��������״̬


/**************************************************************
	��Ӧ�����Ĳ���
***************************************************************/
#define WIDGET_PARAM_ENTER		0	/*����ȷ��*/
#define WIDGET_PARAM_EDIT		1	/*�û���д*/
#define WIDGET_PARAM_SET_TMP	2	/*����SET_TMP*/
#define WIDGET_PARAM_FOCUS_LOST	3	/*�����뿪*/
#define WIDGET_PARAM_FOCUS_GET	4	/*��ȡ����*/
#define WIDGET_PARAM_SHOW		5	/*��ʾ,���������*/
#define WIDGET_PARAM_HIDE		6	/*����,���������*/
#define WIDGET_PARAM_ACTIVE		7	/*��Ч,��Ҫ���ڴ���,���ؼ�,�����ǽ���״̬.��Ч̬Ϊ��ʱ���õ�������*/

#define WIDGET_PARAM_TDOWN		10	/*��������*/
#define WIDGET_PARAM_TUP		11	/*�����ſ�*/





extern DX_WIDGET* pFocus_Widget;	//ָ��ǰ����ؼ���ָ��
extern u8 Focus_Cursor_State;		//��ǰ����ؼ��Ĺ��״̬��ֵ���Ժ����������˸������Ч��


extern LOC_X GetStringLocXFromFRAME(LOC_X x0,LOC_X length,u8 str_length,char*str);
extern LOC_Y GetStringLocYFromFRAME(LOC_Y y0,LOC_Y height,u8 str_height);
extern u32 GetMiddleFromLine(u32,u32,u32 length);
extern u32 GetStringSize(char*);

extern void __Draw3DFrameA(LOC_X,LOC_Y,LOC_X,LOC_Y);//��
extern void __Draw3DFrameB(LOC_X,LOC_Y,LOC_X,LOC_Y);//͹
extern void __Draw3DFrameC(LOC_X,LOC_Y,LOC_X,LOC_Y);//��2 
extern u8 GetNumLen(s32 num);
extern u8 NumToString(s32 num,char* str);
extern u8 NumToStringO8(s32 num,char* str);
extern bool IsWidgetAvailable(DX_WIDGET* the_widget);
extern void ReDrawWidget(DX_WIDGET *the_widget);
extern void GuiWidgetDraw(DX_WIDGET *the_widget);
extern void UpdateWidget(DX_WIDGET* the_widget);
extern void SetWidgetFlash(DX_WIDGET* the_widget,bool);
extern void SetWidgetFLock(DX_WIDGET* the_widget,bool );
extern void SetWidgetHide(DX_WIDGET* the_widget,bool);
extern void SetWidgetDisable(DX_WIDGET* the_widget,bool);
extern DX_WIDGET* SetWidgetFocus(DX_WIDGET* the_widget);
extern void SetNextFocus(DX_WIDGET* the_widget);
extern void KeyToChangeWidget(DX_WIDGET* the_widget,GUIKEY key);
extern void GuiWidgetReponse(DX_WIDGET* the_widget,u8 param);		//�ؼ���Ӧ
extern void WidgetFlash(bool flash);

//����������,��װ��SAMPOO�ĺ���

extern void GUIRectFill2DA(LOC_X x,LOC_Y y,LOC_X length,LOC_Y height,COLOR color);
extern void GUIRectFill2DB(LOC_X x,LOC_Y y,LOC_X length,LOC_Y height,COLOR color);
//extern void GUIRectFill3D(LOC_X x,LOC_Y y,LOC_X length,LOC_Y height,COLOR color);
//extern void GUIFramFillA3D(LOC_X x,LOC_Y y,LOC_X length,LOC_Y height,COLOR color1,COLOR color2);
extern void GUIHLine3D(LOC_X x,LOC_Y y,LOC_X len,COLOR color);
extern void GUIVLine3D(LOC_X x,LOC_Y y,LOC_Y len,COLOR color);
#endif
