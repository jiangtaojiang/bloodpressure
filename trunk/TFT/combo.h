#ifndef COMBO_H
#define COMBO_H

/*************************
**结构体名:DX_COMBO
**说明:combo控件 
**		显示形式为字符段,用U8定义值,所以只能有0-255个项目,*_*项目均为const
** 
*************************/
typedef struct
{
	DX_WIDGET widget;			//基本属性
	u8* p_self_property;		//自我属性--可能没必要
	u8* p_tmp;					//临时值*
	u8* p_data;					//实际值*
	void*const* obj;			//项目数组(项目均为const)
	u8  max_data;				//项目数
	COLOR color2d;				//2D色

}const DX_COMBO;


//COMBO基础属性 占用基础属性8,9,10位(8种状态)
#define BASE_PRO_COMBO_TYPE_MASK 			0x700
#define BASE_PRO_COMBO_TYPE_STRING			(1<<8)		//字符串型
#define BASE_PRO_COMBO_TYPE_BMP				(2<<8)		//BMP型

//COMBO的风格-占用11,12位
#define BASE_PRO_COMBO_STYLE_MASK	0x1800		//11,12位用于定义风格,共4种风格
#define BASE_PRO_COMBO_STYLE_3D		0			//普通3D风格(默认)
#define BASE_PRO_COMBO_STYLE_2D		(1<<11)		//2D平面风格(方框+内容)
#define BASE_PRO_COMBO_STYLE_NULL	(2<<11)		//无外框风格



//COMBO颜色
//#define COLOR_COMBO_STRING			BLACK	//字颜色
//#define COLOR_COMBO_BG_UNFOCUS_3D	WHITE	//没选中的背景色 3D
//#define COLOR_COMBO_BG_UNFOCUS_2D	GREEN_5	//没选中的背景色 2D
//#define COLOR_COMBO_BG_FOCUS		RED		//焦点背景色
//#define COLOR_COMBO_BG_FLASH		RED		//闪烁背景色

extern void ReDrawCombo(DX_COMBO* the_combo);
extern void GuiComboDraw(DX_COMBO *the_combo);
extern void SetComboTmp(DX_COMBO* the_combo,u8 tmp);
extern u8 GetComboTmp(DX_COMBO* the_combo);
extern void UpdateCombo(DX_COMBO* the_combo,bool dir);
extern MESSAGE_WIDGET KeyToChangeCombo(DX_COMBO* the_combo,GUIKEY key);
#endif
