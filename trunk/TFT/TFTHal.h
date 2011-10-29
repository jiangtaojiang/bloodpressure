/**************************************************************
本模块用于定义TFT的一些特定宏,如颜色,坐标,和底层应用函数
***************************************************************/
#ifndef TFTHAL_H
#define TFTHAL_H

//为了加快速度,这里采用直接操作寄存器的方式  
#define RSTTFT_H	GPIOB->BSRR = GPIO_Pin_5
#define RSTTFT_L	GPIOB->BRR=GPIO_Pin_5
//背光开关
#define BLIGHT_ON		GPIOC->BSRR=GPIO_Pin_6
#define BLIGHT_OFF      GPIOC->BRR=GPIO_Pin_6
//#define ST7781	    1
#define ILI9331      1
#define ILI9328      1

/*A0=0  -- cmd*/
#define LCD_Command  *((u16 * )0x60000000)
/*A0=1 -- data*/
#define LCD_Data  *((u16 * )0x60020000)

#ifdef TFTHAL_c
void wr_cmd(u16 cmd);
void wr_data(u16 dat);
void lcd_initialize(void);

#else
extern void TFT_Config(void);
extern void wr_cmd(u16 cmd);
extern void wr_data(u16 dat);
extern void lcd_initialize(void);


#endif

#endif