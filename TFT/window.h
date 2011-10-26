#ifndef WINDOW_H
#define WINDOW_H

//typedef u8 EPROW;
typedef u8 FNO;
/*****************************************
**结构体:DX_WINDOW
**说明:窗口结构体
*****************************************/
typedef struct _dx_window
{
	DX_WIDGET widget;					//基本属性
	DX_WIDGET*const* p_child;			//控件组(注意组本身是CODE,组内元素也是CODE)
	u8 max_widget;						//控件数
	FNO* p_focu_no;						//焦点号,不需要用户初始化,只需要分配一个变量则可
}const DX_WINDOW;


extern DX_WINDOW *pNow_Window;		//指向当前活动窗口

extern void GuiWindowDraw(DX_WINDOW* the_window);
extern void GuiWindowAdd(DX_WINDOW* the_window);
extern void GuiWindowDec(DX_WINDOW* the_window);
extern bool IsWindowAvailable(DX_WINDOW* the_window);
extern DX_WINDOW* GetNowWin(void);
extern void UpdateWindowData(DX_WINDOW* ,bool );
#endif

