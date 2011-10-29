#ifndef HAL_H
#define HAL_H

/**************************************************************
后备寄存器的地址分配
***************************************************************/
//用于时间
#define BKP_TIME_SET		BKP_DR1	//RTC设置与否标志
#define BKP_TIME_YEAR		BKP_DR2
#define BKP_TIME_MONTH		BKP_DR3
#define BKP_TIME_DAY		BKP_DR4
#define BKP_TIME_DATE		BKP_DR5
//用于触摸屏
#define BKP_TOUCH_SET			BKP_DR6	//触摸屏校正与否标志
#define BKP_TOUCH_X_BEGIN		BKP_DR7
#define BKP_TOUCH_LENGTH		BKP_DR8
#define BKP_TOUCH_Y_BEGIN		BKP_DR9
#define BKP_TOUCH_HEIGHT		BKP_DR10


#endif

