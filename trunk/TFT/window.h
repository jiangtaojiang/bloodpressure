#ifndef WINDOW_H
#define WINDOW_H

//typedef u8 EPROW;
typedef u8 FNO;
/*****************************************
**�ṹ��:DX_WINDOW
**˵��:���ڽṹ��
*****************************************/
typedef struct _dx_window
{
	DX_WIDGET widget;					//��������
	DX_WIDGET*const* p_child;			//�ؼ���(ע���鱾����CODE,����Ԫ��Ҳ��CODE)
	u8 max_widget;						//�ؼ���
	FNO* p_focu_no;						//�����,����Ҫ�û���ʼ��,ֻ��Ҫ����һ���������
}const DX_WINDOW;


extern DX_WINDOW *pNow_Window;		//ָ��ǰ�����

extern void GuiWindowDraw(DX_WINDOW* the_window);
extern void GuiWindowAdd(DX_WINDOW* the_window);
extern void GuiWindowDec(DX_WINDOW* the_window);
extern bool IsWindowAvailable(DX_WINDOW* the_window);
extern DX_WINDOW* GetNowWin(void);
extern void UpdateWindowData(DX_WINDOW* ,bool );
#endif

