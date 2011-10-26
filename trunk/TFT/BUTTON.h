#ifndef BUTTON_H
#define BUTTON_H

/***********************
**结构体名:BC_BUTTON
**说明:按键控件
***********************/
typedef struct
{
	DX_WIDGET widget;			//基本特性
	u8* p_self_property;		//自我属性*
	char* str;					//字符
//	void (*button_on)(u8);		//响应函数
	//以下为BMP按键才需要设置的属性
	COLOR* bmp_normal;			//平时的图标
	COLOR* bmp_focus;			//焦点时的图标
	COLOR* bmp_push;			//按下后的图标
}const DX_BUTTON;

//BUTTON基础属性-占用8-9-10位
#define BASE_PRO_BUTTON_TYPE_MASK	0x700
#define BASE_PRO_BUTTON_TYPE_STRING	(0<<8)		//默认
#define BASE_PRO_BUTTON_TYPE_BMP	(1<<8)


//BUTTON风格-占用16,17位
#define BASE_PRO_BUTTON_STYLE_MASK	0x30000		//16,17位用于定义风格,共4种风格
#define BASE_PRO_BUTTON_STYLE_3D	0			//普通3D风格(默认)
#define BASE_PRO_BUTTON_STYLE_2D	(1<<16)		//2D平面风格(方框+数字)
#define BASE_PRO_BUTTON_STYLE_NULL	(2<<16)		//纯色风格(底色为WINDOW本身颜色)

//BUTTON自我属性


#define SELF_PRO_PUSH_DOWN			0x01		//SELF属性第一位代表按下的状态,1为按下,0为弹起


/**************************************************************
	控件配色预定义
***************************************************************/

//按键颜色
#define COLOR_BUTTON_STRING			BLACK	//字符色
#define COLOR_BUTTON_BG_UNFOCUS		GRAY1	//没选中的背景色--默认的背景色


extern MESSAGE_WIDGET KeyToChangeButton(DX_BUTTON*,GUIKEY);
extern void ReDrawButton(DX_BUTTON* the_button);
extern void GuiButtonDraw(DX_BUTTON *the_button);

#endif
