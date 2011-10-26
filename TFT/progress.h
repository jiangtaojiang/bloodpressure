#ifndef PROGRESS_H
#define PROGRESS_H

/*************************
**结构体名:DX_PROGRESS
**说明:编辑框控件
*************************/
typedef struct
{
	DX_WIDGET widget;			//基本属性
	u8* p_self_property;		//自我属性
	s32* p_tmp;					//临时值*
	s32* p_data;				//实际值*
	s32 max_data;				//最大值
	s32 min_data;				//最小值
	COLOR bg_color;				// 底色
	COLOR fg_color;				// 面色
}const DX_PROGRESS;

/**************************************************************
	PROGRESS BASE属性项
***************************************************************/
//PROGRESS的基础属性-占用基础属性部分 8,9,位(4种状态)
#define BASE_PRO_PROGRESS_TYPE_MASK		0x300		//8,9,10位用于定义PROGRESS的类型,
#define BASE_PRO_PROGRESS_TYPE_PERCENT	0			//0是百分比
#define BASE_PRO_PROGRESS_TYPE_NUM		(1<<8)		//1代表实际数值

//PROGRESS的方向属性-占用基础属性部分的第10位 (两种状态)
#define BASE_PRO_PROGRESS_DIR_MASK		0x400
#define BASE_PRO_PROGRESS_DIR_H			(0<<10)		//横
#define BASE_PRO_PROGRESS_DIR_V			(1<<10)		//竖

//PROGRESS的起始属性-占用基础属性部分的第11位(两种状态)
#define BASE_PRO_PROGRESS_BEGIN_MASK	0x800
#define BASE_PRO_PROGRESS_BEGIN_LD		(0<<11)		//从左或下开始
#define BASE_PRO_PROGRESS_BEGIN_RU		(1<<11)		//从右或上开始

//PROGRESS的风格
#define BASE_PRO_PROGRESS_STYLE_MASK	0x30000		//16,17位用于定义风格,共4种风格
#define BASE_PRO_PROGRESS_STYLE_3D		0			//普通3D风格(默认)
#define BASE_PRO_PROGRESS_STYLE_2D		(1<<16)		//2D平面风格


/**************************************************************
	颜色定义
***************************************************************/
//#define COLOR_PROGRESS_STRING	BLACK	//进度条的字符颜色



/**************************************************************
	外部调用函数
***************************************************************/
extern void ReDrawProgress(DX_PROGRESS *the_progress);
extern void GuiProgressDraw(DX_PROGRESS *the_progress);
extern void SetProgressTmp(DX_PROGRESS *the_progress,s32 tmp);
extern s32 GetProgressTmp(DX_PROGRESS *the_progress);
extern void UpdateProgress(DX_PROGRESS *the_progress,bool dir);
#endif
