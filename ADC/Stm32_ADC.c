/******************************************************************************
* file Name     : Stm32_Adc.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#define  Stm32_ADC_c
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include "main.h"
#include "Stm32_ADC.h"
#include "TFTdriver.h"
#include "DX_gui.h"


extern DX_EDIT MAIN_WIN_ADC;
extern DX_EDIT MAIN_WIN_ADC1;
/*******************************************************************************
* Function Name  : adc_Config
* Description    : adc配置
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void Adc_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	/* ADC1 configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立模式 
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //连续多通道模式
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换不受外界决定 
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐 
  	ADC_InitStructure.ADC_NbrOfChannel = 2;  //扫描通道数  //设置转换序列长度为2
  	ADC_Init(ADC1, &ADC_InitStructure); ////调用函数，初始化ADC1
  	
  	/* ADC1 regular channel9 configuration */ 
        ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
   
  	//通道X,采样时间为1.5周期,1代表规则通道第1个
        /* Enable ADC1 DMA [使能ADC1 DMA]*/
        ADC_DMACmd(ADC1, ENABLE);
  	/* Enable ADC1 */
  	ADC_Cmd(ADC1, ENABLE);
/******下面是ADC自动校准，开机后需执行一次，保证精度*******/ 
  	/* Enable ADC1 reset calibaration register */   
  	ADC_ResetCalibration(ADC1);
  	/* Check the end of ADC1 reset calibration register */
  	while(ADC_GetResetCalibrationStatus(ADC1));

  	/* Start ADC1 calibaration */
  	ADC_StartCalibration(ADC1);
  	/* Check the end of ADC1 calibration */
  	while(ADC_GetCalibrationStatus(ADC1));
/*****************************************************/ 	
  	/* Start ADC1 Software Conversion */ 
  	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
/*******************************************************************************
* Function Name  : Adc_GPIOConfig
* Description    : Adc_GPIOConfig
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void Adc_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		
	//PB1 作为模拟通道9输入引脚                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //PA4作为模拟通道4输入引脚                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : Adc_DMA_Config
* Description    : Adc_DMA_Config
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void Adc_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;//定义DMA初始化结构体
    DMA_DeInit(DMA1_Channel1);//复位DMA通道1
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS; //定义 DMA通道外设基地址=ADC1_DR_Address
    DMA_InitStructure.DMA_MemoryBaseAddr =(u32)ADC_ValueTab; //定义DMA通道存储器地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//指定外设为源地址
    DMA_InitStructure.DMA_BufferSize = 2;//定义DMA缓冲区大小1
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//当前外设寄存器地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//当前存储器地址变
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//定义外设数据宽度16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //定义存储器数据宽度16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA通道操作模式位环形缓冲模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道优先级高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//禁止DMA通道存储器到存储器传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化DMA通道1
    
//    DMA_ITConfig(DMA1_Channel1 , DMA_IT_TC , ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE); //使能DMA通道1
}

/*******************************************************************************
* Function Name  : Adc_Init
* Description    : Adc_Init
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void Adc_Init(void)
{
    Adc_DMA_Config();
	Adc_GPIOConfig();
	Adc_Config();
    
}
/*******************************************************************************
* Function Name  : IntToChar
* Description    : IntToChar
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
//u8 value[4];
void IntToChar(int tem)
{
	value[0] = (tem/100)+0x30;
//	value[1] = DPoint;
	value[1] = (tem%100/10)+0x30;
	value[2] = (tem%100%10)+0x30;
//	value[4] = DV;
}
/*******************************************************************************
* Function Name  : ADC_Test
* Description    : ADC_Test
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/

void ADC_Test(void)
{
	int tem[2];
//	char *c = value;

//	AD_value=ADC_GetConversionValue(ADC1);
	tem[0] = (int)ADC_ValueTab[0]*331/4096;
    tem[1] = (int)ADC_ValueTab[1]*331/4096;
	IntToChar(tem[0]);
    IntToChar(tem[1]);
//	TFTShowString24(50,250,c,BLUE,0);
    SetEditTmp(&MAIN_WIN_ADC,tem[0]);
    SetEditTmp(&MAIN_WIN_ADC1,tem[1]);
	#ifdef DEBUG
//    printf("ADC: %.3f \r\n",(float)ADC_ValueTab[1]*336/409600);
    printf("ADC: %d \r\n",ADC_ValueTab[0]);
    printf("ADC: %d \r\n",ADC_ValueTab[1]);
    #endif
}