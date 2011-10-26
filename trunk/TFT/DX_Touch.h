#ifndef DX_TOUCH_H
#define DX_TOUCH_H
#define TOUCH_ADJ_TOLERANCE		200	//触摸校正允许的AD公差

//四个校正点的中心坐标,注意不能设置为0.这里采用一般的方式,即屏长宽的1/10
#define TADJ_XB		(MAX_X/10)
#define TADJ_YB		(MAX_Y/10)
#define TADJ_XE		(MAX_X-TADJ_XB)
#define TADJ_YE		(MAX_Y-TADJ_YB)

//触摸校正函数返回值
#define TADJ_RTN_OK				0		//成功获取四个校正点
#define TADJ_RTN_FALSE			1		//获取校正点失败
#define TADJ_RTN_CONTINUE		2		//获取还在进行中

extern bool GuiTranTouchXY(u16 touch_x,u16 touch_y,LOC_X* x,LOC_Y* y);

//需要外围根据硬件编写的3个函数
extern bool GetTouchPush(u16* x,u16* y);
extern bool GetTouchHold(u16* x,u16* y);
extern bool GetTouchRelease(u16* x,u16* y);


#endif
