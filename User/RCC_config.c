/******************************************************************************
* file Name     : RCC_config.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#define  RCC_config_c
#include "stm32f10x_conf.h"
#include "main.h"
#include "RCC_config.h"
#include "usart.h"
#include "RTC.h"
#include "hal.h"
#include "25f.h"
#include "Stm32_ADC.h"
#include "Stm_Pwm.h"
#include "TFTHal.h"
/*******************************************************************************
* Function Name  : system_init
* Description    : system_init.ϵͳ��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void system_init(void)
{
    GPIO_Clock_config();//IOʱ��ʹ��
    Usart_Init();       //���ڳ�ʼ��
    GPIO_Configuration();//IO������
    NVIC_Configuration();//�ڲ��ж�
    EXTI_Configuration();//�ⲿ�����жϣ�
    Tim_Init();          //��ʱ��
    RTC_Config();        //RTC����
    Systick_Configuration();	
	FLASH_SPI_Config(); //��ʼ��SST25VF080
    Adc_Init();     //ADC init	
	TFT_Config();   //��ʼ��TFT
    PWM_Init();
//    Lcd_Configuration(); //lcd io����
//    PVD_config();
//    Adc_Init();
//    LCD_Init();
    //DMA_Configuration();
}
/*******************************************************************************
* Function Name  : RCC_Periph
* Description    : RCC_Periph.����ʱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Clock_config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1  | RCC_APB2Periph_AFIO,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}
/*******************************************************************************
* Function Name  : Systick config
* Description    : systick time: 1ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Systick_Configuration(void)
{	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 	
		while (1);
	}
}
/********************************************
**������:SysTickDelay
**����:ʹ��ϵͳʱ�ӵ�Ӳ�ӳ�
**ע������:һ���,��Ҫ���ж��е��ñ�����,����������������.�������������ȫ���ж�,��Ҫʹ�ô˺���
********************************************/
volatile u16 Timer1,Timer2;
void SysTickDelay(u16 dly_ms)
{
	Timer1=dly_ms;
	while(Timer1);
}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : config output/input pin.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	/*Configure LED pins:PB.8 PB.9 PA.11 PA.12,PB.5:SD_PWR*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    
    /* USB_DISCONNECT used as USB pull-up */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* PB0,5,8,9��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* PC5,7���*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*PD7,���*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/*PA8,���*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	//��©���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/* PD3,4,5,6��������*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50Mʱ���ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : PVD_config
* Description    : PVD_config
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_config(void)
{
    PWR_PVDLevelConfig(PWR_PVDLevel_2V5); //���õ͵����ѹ
	PWR_PVDCmd(ENABLE);
}
/*******************************************************************************
* Function Name  : EXTI_Configuration
* Description    : config EXTI Interrupt.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	EXTI_DeInit();
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line16;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//;EXTI_Trigger_Rising_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); // Configure EXTI Line16 to generate an interrupt
}
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  /*�ж�����˵����������Ӧʱ�����жϣ�����1�жϣ���Ѫ����ͨѶ��,���ȼ������£�
                  ��ռ���ȼ�  ��Ӧ���ȼ�
	1������1            0         0
	2������2            0         1
	3��TIM2�ж�         1         0
	4������3			1		  1	
	5����ѹ�ж�         1         2
 
  ��ռ���ȼ��ߵĿ����жϵ���ռ���ȼ����ȴ��� */
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* Set the Vector Table base location at 0x2000 */ 
  	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);

	/* Enable the TIM2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//0->1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //1->0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#ifdef DEBUG
	/* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//1->0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	/*Enable the PVD Interrupt*/
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); // Enable the PVD Interrupt 
    
    /*RTC*/
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    //USB_Interrupts_Config
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //SDIO_Interrupts_Config
    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //DMA
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;//for ADC1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    //DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE); //���ô�������ж�
}
/*******************************************************************************
* Function Name  : Tim_Init
* Description    : config Tim.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Tim_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 
    //TIM2 init
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //enable clock of TIM2
        
    /* Time Base configuration */
    //APB1��Ƶϵ��Ϊ1ʱ����Ƶ�ʲ��䣬����ϵ��x2
    TIM_TimeBaseStructure.TIM_Prescaler = 72;//72 Ԥ��Ƶϵ��
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000; //APB1 = 36M  reload rate: 1ms 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2,TIM_FLAG_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}