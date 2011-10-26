/***************************************************
**HAL.c
**��Ҫ����оƬӲ�����ڲ���Χ���ⲿ��Χ�ĳ�ʼ��������INIT����
**��MAIN�е��ã�ʹMAIN�����о�����Ӳ�����޹�
***************************************************/

#include "stm32f10x.h"
#include "hal.h"

//�����ڲ�Ӳ��ģ������ú���
extern void GPIO_Configuration(void);			//GPIO
extern void RCC_Configuration(void);			//RCC
extern void USART_Configuration(void);			//����
extern void NVIC_Configuration(void);			//NVIC
extern void FLASH_SPI_Config(void);				//FLASH SPI
extern void I2C_Configuration(void);			//I2C��ʼ��

extern void Ch375_Configuration(void);			//USB��-CH375
extern void Tim1_Configuration(void);			//TIM1
extern void ADC_Configuration(void);			//ADC
extern void Lcds_Config(void);					//LCDs
extern void SD_Config(void);						//SD
extern void TFT_Config(void);					//TFT
extern void Touch_Config(void);					//TOUCH
extern void TurnToQtouch(void);					//QTOUCH
extern void CAN_Configuration(void);			//CAN
extern void ENC28j60_Config(void);				//����
extern void USB_Configurage(void);				//USB��
extern void Key_Config(void);					//���м���

/*******************************
**������:ChipHalInit()
**����:Ƭ��Ӳ����ʼ��
*******************************/
void  ChipHalInit(void)
{
	//��ʼ��ʱ��Դ
	RCC_Configuration();
	
	
	//��ʼ��GPIO
	GPIO_Configuration();
	
	//��ʼ������
	USART_Configuration();
	
	//��ʼ��NVIC
	NVIC_Configuration();
	
	
	//��ʼ��SST25VF080
	FLASH_SPI_Config();
	
	//��ʼ��I2C
	I2C_Configuration();

	//��ʼ��CH375
	//Ch375_Configuration();

	//��ʼ��tim1��PWM
	//Tim1_Configuration();
	
	//��ʼ��ADC
	ADC_Configuration();
	
	//��ʼ��CAN
	CAN_Configuration();
	
//	USB_Configurage();
}


/*********************************
**������:ChipOutHalInit()
**����:Ƭ��Ӳ����ʼ��
*********************************/
void  ChipOutHalInit(void)
{
	//��ʼ��SD
	SD_Config();
	
	//��ʼ��TFT
	TFT_Config();
	
	//��ʼ��������
	Touch_Config();
	
	//��ʼ��QTOUCH����
//	TurnToQtouch();
	
	//��ʼ��LCD
	Lcds_Config();

	ENC28j60_Config();

	Key_Config();
}

//����SPI�ӿڵ�״̬,���㸴��
u8 Spi1_Mode=0;
u8 Spi2_Mode=0;


