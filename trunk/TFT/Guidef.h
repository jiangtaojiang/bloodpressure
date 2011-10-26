#ifndef GUIDEF_H
#define GUIDEF_H

#include "color.h"
#include "TFTdriver.h"
#include "key.h"

#ifndef NULL
#define NULL	0
#endif


#define GUIKEY	u8		//������
#define MAX_Z	5		//��������ƴ�Ӵ�����


/**************************************************************
	�ؼ���Ϣ
***************************************************************/
//�ؼ�����
#define MESSAGE_WIDGET	u8

#define MESSAGE_NULL					0		//����Ϣ
#define MESSAGE_WIDGET_FOCUS_LOST		1		//ʧȥ����
#define MESSAGE_WIDGET_FOCUS_GET		2		//��ý���
#define MESSAGE_WIDGET_EDIT				3		//��ֵ���ı�
#define MESSAGE_WIDGET_ENTER			4		//�ؼ�������ENTER
#define MESSAGE_WIDGET_TOUCH_DOWN		5		//�ؼ�������
#define MESSAGE_WIDGET_TOUCH_UP			6		//�ؼ��Ӵ���״̬���ſ�


/**************************************************************
	ϵͳ��ɫ
***************************************************************/
#define COLOR_EDIT_CURSOR	YELLOW

/**************************************************************
	���Թ��� �Ժ��õ���
***************************************************************/
#define DEBUG_GUI 1

#ifdef DEBUG_GUI

#define gui_asssert_failed(x) 	/*��������ʱ����Ϊ��,���û��Լ����ٴ�������,����__FILE__ __LINE__�ؼ���ͨ�������������λ��*/

#endif

#ifdef GuiData_c

#else
extern void UsrInterface(void);
#endif

#endif

