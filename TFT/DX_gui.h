/**************************************************************
ģ��:tiny BC GUI:΢�͵İײ˰�GUI
����:��֧�ֲ������,����ֻ��Ҫ��������Ĵ��ؼ���GUI,֧�ִ������ͼ����ƶ� ,����Ϳؼ��ı���ɫΪϵͳԤ����.�ɽ��к��޸�
***************************************************************/

#ifndef BC_GUI_H
#define BC_GUI_H

#include "GuiDef.h"
/**************************************************************
	��Դ���
***************************************************************/
#include "widget.h"
#include "button.h"
#include "edit.h"
#include "combo.h"
#include "check.h"
#include "window.h"

//��ʱû�õ�
#define WIDGET_PARAM_Q_SIZE	100	/*����ؼ���Ӧ���е������Ӧ����,��������Ҫ�����û�һ������ӵ�е����ؼ���*/


/*************************************************************
	�������
*************************************************************/
#include "dx_touch.h"



extern void GuiInit(void);
extern void KeyToGui(GUIKEY key);
extern void GuiSysTick20ms(void);

extern u8 TouchAdj(void);
extern bool NeedTouchAdj(void);

extern void ManageWPQ(void);					//��Ӧ���еĹ���
#endif
