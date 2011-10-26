#ifndef CHECK_H
#define CHECK_H

/*************************
**结构体名:DX_EDIT
**说明:编辑框控件 
**注意事项:*_*考虑不需要p_tmp值,此值用self_pro的一个位表示,这样可省点空间
**			p_data则保留, 
*************************/
typedef struct
{
	DX_WIDGET widget;			//基本属性
	u8* p_self_property;		//自我属性
	bool * p_tmp;				//临时值* 
	bool * p_data;				//实际值*
	void*const* obj;			//项目组,一般是BMP风格,STRING风格用
	COLOR color2d;				// 第二号的色
}const DX_CHECK;

/**************************************************************
CHECK BASE属性项
***************************************************************/
//CHECK的基础属性-占用基础属性部分 8,9,10位(8种状态)
#define BASE_PRO_CHECK_TYPE_MASK	0x700		//8,9,10位用于定义CHECK的类型,共8钟类型
#define BASE_PRO_CHECK_TYPE_HOOK	0			//0是普通的勾勾模式,大小由字体大小决定
#define BASE_PRO_CHECK_TYPE_BMP		(1<<8)		//1是BMP模式
#define BASE_PRO_CHECK_TYPE_STRING	(2<<8)		//2是字符模式
#define BASE_PRO_CHECK_TYPE_BUTTON	(3<<8)		//3是按键模式(按下和弹起代表两个状态)

//CHECK风格-占用16,17位*_*特别说明,暂时CHECK风格只有在STRING模式的时候有效(09.11.19)
#define BASE_PRO_CHECK_STYLE_MASK	0x30000		//16,17位用于定义风格,共4种风格
#define BASE_PRO_CHECK_STYLE_3D		0			//普通3D风格
#define BASE_PRO_CHECK_STYLE_2D		(1<<16)		//2D平面风格(方框),默认
#define BASE_PRO_CHECK_STYLE_NULL	(2<<16)		//纯色风格(底色为WINDOW本身颜色)


extern void ReDrawCheck(DX_CHECK*);
extern void GuiCheckDraw(DX_CHECK *the_check);
extern void SetCheckTmp(DX_CHECK*,bool);
extern bool GetCheckTmp(DX_CHECK*);
extern void UpdateCheck(DX_CHECK*,bool);
extern MESSAGE_WIDGET KeyToChangeCheck(DX_CHECK* the_check,GUIKEY key);

#endif
