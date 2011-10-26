#ifndef _EDIT_H
#define _EDIT_H

/*************************
**结构体名:DX_EDIT
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

}const DX_EDIT;

/**************************************************************
EDIT BASE属性项
***************************************************************/
//EDIT的基础属性-占用基础属性部分 8,9,10位(8种状态)
#define BASE_PRO_EDIT_TYPE_MASK		0x700		//8,9,10位用于定义EDIT的类型,共8钟类型
#define BASE_PRO_EDIT_TYPE_0P		0			//0是无小数点的普通EDIT(默认)
#define BASE_PRO_EDIT_TYPE_1P		(1<<8)		//1代表1位小数EDIT
#define BASE_PRO_EDIT_TYPE_2P		(2<<8)		//2代表2位小数EDIT
#define BASE_PRO_EDIT_TYPE_PW		(3<<8)		//3代表PASSWORD形式的EDIT
#define BASE_PRO_EDIT_TYPE_O8		(4<<8)		//4代表8进制整数模式


//EDIT的风格-占用16,17位
#define BASE_PRO_EDIT_STYLE_MASK	0x30000		//16,17位用于定义风格,共4种风格
#define BASE_PRO_EDIT_STYLE_3D		0			//普通3D风格(默认)
#define BASE_PRO_EDIT_STYLE_2D		(1<<16)		//2D平面风格(方框+数字)
#define BASE_PRO_EDIT_STYLE_NULL	(2<<16)		//无风格(底色为WINDOW本身颜色) NULL风格图形不支持焦点闪烁.
#define BASE_PRO_EDIT_STYLE_GLASS	(3<<16)		//玻璃风格(忽略底色) 玻璃风格不支持焦点和闪烁*_*暂时还不可用,真的要有玻璃风格,程序会比较繁~


//EDIT的位长,占用24,25,26,27 代表长度填充可以1-16位
#define BASE_PRO_EDIT_BCD_MASK		0xf000000
#define BASE_PRO_EDIT_BCD_NULL		(0<<24)			//默认为自动长度
#define BASE_PRO_EDIT_BCD_1			(1<<24)			//1位-似乎无意义,但必须保留,因为获取位长的宏根用单纯的移位操作判断位长
#define BASE_PRO_EDIT_BCD_2			(2<<24)
#define BASE_PRO_EDIT_BCD_3			(3<<24)
#define BASE_PRO_EDIT_BCD_4			(4<<24)
#define BASE_PRO_EDIT_BCD_5			(5<<24)
#define BASE_PRO_EDIT_BCD_6			(6<<24)
#define BASE_PRO_EDIT_BCD_7			(7<<24)
#define BASE_PRO_EDIT_BCD_8			(8<<24)
/*
#define BASE_PRO_EDIT_BCD_9			(9<<24)
#define BASE_PRO_EDIT_BCD_10		(10<<24)
#define BASE_PRO_EDIT_BCD_11		(11<<24)
#define BASE_PRO_EDIT_BCD_12		(12<<24)
#define BASE_PRO_EDIT_BCD_13		(13<<24)
#define BASE_PRO_EDIT_BCD_14		(14<<24)
#define BASE_PRO_EDIT_BCD_15		(15<<24) 
*/ 
//获取位长的宏
#define GET_EDIT_BCD_LEN(the_edit)	((the_edit->widget.base_property&BASE_PRO_EDIT_BCD_MASK)>>24)

//SELF属性0,1位
#define SELF_PRO_EDIT_STATE_MASK	0x03		//1,2位用于装载EDIT的状态
//1.若EDIT为普通类型,则状态位暂时无用
//...
//2.若EDIT为带小数位
#define SELF_PRO_EDIT_STATE_NULL	0			//0代表没出现小数,这里必须定义这个类型为0
#define SELF_PRO_EDIT_STATE_0P		1			//1代表出现XXX.的状态
#define SELF_PRO_EDIT_STATE_1P		2			//2代表出现XXX.X的状态
#define SELF_PRO_EDIT_STATE_2P		3			//3代表出现XXX.XX状态
//3.若EDIT为PASSWORD,1,2,3,4位用于表示密码位数
#define SELF_PRO_EDIT_PASS_NUM_MASK	0x0f
#define SELF_PRO_EDIT_PASS_NUM0		0
#define SELF_PRO_EDIT_PASS_NUM1		1
#define SELF_PRO_EDIT_PASS_NUM2		2
#define SELF_PRO_EDIT_PASS_NUM3		3
#define SELF_PRO_EDIT_PASS_NUM4		4
#define SELF_PRO_EDIT_PASS_NUM5		5
#define SELF_PRO_EDIT_PASS_NUM6		6
#define SELF_PRO_EDIT_PASS_NUM7		7
#define SELF_PRO_EDIT_PASS_NUM8		8







extern void ReDrawEdit(DX_EDIT* the_edit);
extern void GuiEditDraw(DX_EDIT* the_edit);
extern MESSAGE_WIDGET KeyToChangeEdit(DX_EDIT*,GUIKEY);
extern void SetEditTmp(DX_EDIT* the_edit,s32 tmp);
extern s32 GetEditTmp(DX_EDIT* the_edit);
extern void UpdateEdit(DX_EDIT* the_edit,bool dir);
extern void EditCursorFlash(bool flash);
extern bool EditPassMatch(DX_EDIT* the_edit,u32 num);
#endif
