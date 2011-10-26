#ifndef HAL_H
#define HAL_H

//硬件定义
/**************************************************************
SPI接口处于的状态的定义方便复用SPI接口
***************************************************************/
#define SPI1_MODE_SST25		1
#define SPI1_MODE_SD		2

#define SPI2_MODE_NET		1
#define SPI2_MODE_TOUCH		2
#define SPI2_MODE_LCD		3



/**************************************************************
LCD的控制器的定义,这里包括了几个不同控制器的初始化代码
***************************************************************/
//R6150V控制器
//#define R6150V		1

//R6150u控制器
//#define R6150U		1

//SSD1297
//#define SSD1297			1

//SSD1289
//#define SSD1289		1

//HX8347
//#define HX8347			1

//ST7781
//#define ST7781			1

//ILI9328
//#define ILI9328			1





/**************************************************************
串行FLASH的地址分配
***************************************************************/
//字库在FLASH中的起始位置
#define HZ_DOT16_START_ADDR		0
#define HZ_DOT24_START_ADDR		0x34e00
//用户FLASH的起始位置
#define USR_FLASH_START_ADDR	0xabd80
//用户FLASH的末端(注意不能等于这个值)
#define USR_FLASH_END_ADDR		0x100000

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


//输出宏定义
#define LED1_ON		GPIOA->BRR = GPIO_Pin_8
#define LED1_OFF	GPIOA->BSRR = GPIO_Pin_8
#define LED1_POLL	GPIOA->ODR ^=GPIO_Pin_8

#define LED2_ON		GPIOD->BRR =GPIO_Pin_7
#define LED2_OFF	GPIOD->BSRR =GPIO_Pin_7

#define LED3_ON		GPIOC->BRR = GPIO_Pin_7
#define LED3_OFF	GPIOC->BSRR = GPIO_Pin_7

#define LED4_ON		GPIOC->BRR = GPIO_Pin_5
#define LED4_OFF	GPIOC->BSRR = GPIO_Pin_5

#define LED5_ON		GPIOB->BRR = GPIO_Pin_9
#define LED5_OFF	GPIOB->BSRR = GPIO_Pin_9

#define LED6_ON		GPIOB->BRR = GPIO_Pin_8
#define LED6_OFF	GPIOB->BSRR = GPIO_Pin_8

#define LED7_ON		GPIOB->BRR = GPIO_Pin_5
#define LED7_OFF	GPIOB->BSRR = GPIO_Pin_5

#define LED8_ON		GPIOB->BRR = GPIO_Pin_0
#define LED8_OFF	GPIOB->BSRR = GPIO_Pin_0



//输入宏定义
#define GET_LEFT()	(!(GPIOD->IDR&GPIO_Pin_3))
#define GET_UP()	(!(GPIOD->IDR&GPIO_Pin_4))
#define GET_RIGHT()	(!(GPIOD->IDR&GPIO_Pin_5))
#define GET_DOWN()	(!(GPIOD->IDR&GPIO_Pin_6))


//背光开关
#define BLIGHT_OFF		GPIOC->BSRR=GPIO_Pin_5
#define BLIGHT_ON      GPIOC->BRR=GPIO_Pin_5




//硬件初始化
extern void  ChipHalInit(void);
extern void  ChipOutHalInit(void);
//SPI接口状态
extern u8 Spi1_Mode;
extern u8 Spi2_Mode;


//系统时钟延迟
extern void SysTickDelay(u16);

//串口
extern void USART1_Putc(u8 c);
extern void USART1_Puts(char * str);
extern u8 WaitUsart1(void);
extern void USART2_Putc(u8 c);
extern void USART2_Puts(char * str);

extern u8 Uart2_Get_Flag;
extern u8 Uart2_Get_Data;


//SST25VF
//SST5VF
extern void SST25ChipErase(void);							//全刷除
extern void SST25SectorErase(u32 addr);						//扇区刷除
extern void SST25Write(u32 addr,u8* p_data,u32 no);			//写
extern void SST25Read(u32 addr,u8* p_data,u32 no);			//读
extern void TestSst(void);
extern void SPI1TurnToSST25(void);
//I2C
extern void I2C_ReadS_24C(u8 addr ,u8* pBuffer,u16 no);		//读
extern void I2C_ByteWrite_24C(u8 addr,u8 dat);				//字写入
extern void I2C_WriteS_24C(u8 addr,u8* pBuffer,  u16 no);	//随即写入
extern void I2C_Test(void);

//RTC
extern bool RTCInit(void);
//ADC 
extern u16 TestAdc(void);

//LCD
extern void SPI2TurnToLcd(void);
extern void LcdInit(void);
extern void TestLCDS(void);

//SD
extern void SPI1TurnToSD(void);						//SD

//TFT


//touch
extern void TestTouch(void);
extern void SPI2TurnToTouch(void);

//QT
extern void TestQTouch(void);

//CAN
extern bool CAN_Polling(void);
extern void CAN_Interrupt(void);
extern void SendCan(void);

//KEY
extern u16 PollingKey(void);

//ch375
extern bool InitCH375(void);
//NET
extern void SPI2TurnToNet(void);
extern void NetInit(void);
extern void NetLoop(void);
#endif

