/***************************************************
**HAL.c
**主要用于芯片硬件的内部外围和外部外围的初始化，两大INIT函数
**在MAIN中调用，使MAIN函数中尽量与硬件库无关
***************************************************/

#include "stm32f10x.h"
#include "hal.h"

//各个内部硬件模块的配置函数
extern void GPIO_Configuration(void);			//GPIO
extern void RCC_Configuration(void);			//RCC
extern void USART_Configuration(void);			//串口
extern void NVIC_Configuration(void);			//NVIC
extern void FLASH_SPI_Config(void);				//FLASH SPI
extern void I2C_Configuration(void);			//I2C初始化

extern void Ch375_Configuration(void);			//USB主-CH375
extern void Tim1_Configuration(void);			//TIM1
extern void ADC_Configuration(void);			//ADC
extern void Lcds_Config(void);					//LCDs
extern void SD_Config(void);						//SD
extern void TFT_Config(void);					//TFT
extern void Touch_Config(void);					//TOUCH
extern void TurnToQtouch(void);					//QTOUCH
extern void CAN_Configuration(void);			//CAN
extern void ENC28j60_Config(void);				//网卡
extern void USB_Configurage(void);				//USB从
extern void Key_Config(void);					//行列键盘

/*******************************
**函数名:ChipHalInit()
**功能:片内硬件初始化
*******************************/
void  ChipHalInit(void)
{
	//初始化时钟源
	RCC_Configuration();
	
	
	//初始化GPIO
	GPIO_Configuration();
	
	//初始化串口
	USART_Configuration();
	
	//初始化NVIC
	NVIC_Configuration();
	
	
	//初始化SST25VF080
	FLASH_SPI_Config();
	
	//初始化I2C
	I2C_Configuration();

	//初始化CH375
	//Ch375_Configuration();

	//初始化tim1的PWM
	//Tim1_Configuration();
	
	//初始化ADC
	ADC_Configuration();
	
	//初始化CAN
	CAN_Configuration();
	
//	USB_Configurage();
}


/*********************************
**函数名:ChipOutHalInit()
**功能:片外硬件初始化
*********************************/
void  ChipOutHalInit(void)
{
	//初始化SD
	SD_Config();
	
	//初始化TFT
	TFT_Config();
	
	//初始化触摸屏
	Touch_Config();
	
	//初始化QTOUCH按键
//	TurnToQtouch();
	
	//初始化LCD
	Lcds_Config();

	ENC28j60_Config();

	Key_Config();
}

//定义SPI接口的状态,方便复用
u8 Spi1_Mode=0;
u8 Spi2_Mode=0;


