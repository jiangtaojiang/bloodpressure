/**************************************************************
本模块用于定义TFT的一些特定宏,如颜色,坐标,和底层应用函数
***************************************************************/
#ifndef TFTHAL_H
#define TFTHAL_H


/**************************************************************
虚拟总线状态,只有读和写两种
***************************************************************/

#define SET_BUS_OUT		GPIOE->CRL=0x33333333	/*推挽输出50MHZ速度?*/
#define SET_BUS_IN		GPIOE->CRL=0x44444444	/*浮空输入*/

#define ST7781			1

//TFT显示镜像定义
#define TFT_TYPE	1		//1是一号,2是二号屏

//触摸屏定义
#define TOUCH_TYPE	2		//1是旧版,2是新版
#ifdef TFTHAL_c
void wr_cmd(u16 cmd);
void wr_data(u16 dat);
void lcd_initialize(void);
void SetBusWrite(void);
void SetBusRead(void);

#else
extern void TFT_Config(void);
extern void wr_cmd(u16 cmd);
extern void wr_data(u16 dat);
extern void SetBusWrite(void);
extern void SetBusRead(void);
extern void lcd_initialize(void);


#endif

#endif