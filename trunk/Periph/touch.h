#ifndef TOUCH_H
#define TOUCH_H


//TOUCH-各个标志位，为了节省内存，使用位方式
#define TOUCH_F_NULL			0
#define TOUCH_F_PUSH 			1				//触摸有效
#define TOUCH_F_BCK_TIME 		(1<<1)			//触摸开始计算消抖此标志还没代表触摸有效
#define TOUCH_F_FILTERING		(1<<2)			//触摸开始滤波
#define TOUCH_F_RELEASE			(1<<3)			//触摸释放*单独这个标志出现是没有意义的.它必须和PUSH,HOLD同时携带,才能表示释放的状态
//#define TOUCH_F_BPK_TIME		(1<<4)			//连触标志-忽略
#define TOUCH_F_WAIT_HOLD		(1<<5)			//等待保持
#define TOUCH_F_HOLD			(1<<6)			//保持标志

//TOUCH 滤波计数器
#define MAX_TOUCH_FILTER_NO			10				//滤波10次

//消抖的计数值
#define TOUCH_CNT_FLITER_TIME		50				//最初的消抖时间,到达此时间则进入滤波状态
#define TOUCH_CNT_WAIT_HOLD_TIME	5				//WAIT_HOLD滤波后的坐标,还要经过这个安全时间段,才能正式作为HOLD的坐标
#define TOUCH_CNT_HOLD_TIME			5				//第一次触摸有效后,到开始HOLD滤波的时间

extern void ManageTouch(void);					//触摸管理函数-1MS调用一次
extern bool GetTouchPush(u16* x,u16* y);
extern bool GetTouchHold(u16* x,u16* y);
extern bool GetTouchRelease(u16* x,u16* y);
extern void ClearTouchFlag(void);

#endif
