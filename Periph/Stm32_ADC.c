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
#include "usart.h"
#include "Stm32_ADC.h"
#include "TFTdriver.h"
#include "DX_gui.h"


extern DX_EDIT MAIN_WIN_ADC;
extern DX_EDIT MAIN_WIN_ADC1;
extern DX_EDIT MAIN_WIN_VBAT;
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
	
//    RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //ADC时钟12M
	/* ADC1 configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立模式 
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //连续多通道模式
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换不受外界决定 
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐 
  	ADC_InitStructure.ADC_NbrOfChannel = 3;  //扫描通道数  //设置转换序列长度为2
  	ADC_Init(ADC1, &ADC_InitStructure); ////调用函数，初始化ADC1
  	
  	/* ADC1 regular channel9 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);
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
		
	//PC0,1,2 作为模拟通道10,11,12输入引脚                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //PA4作为模拟通道4输入引脚                         
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
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
    DMA_InitStructure.DMA_BufferSize = 3;//定义DMA缓冲区大小1
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//当前外设寄存器地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//当前存储器地址变
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//定义外设数据宽度16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //定义存储器数据宽度16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA通道操作模式位环形缓冲模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道优先级高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//禁止DMA通道存储器到存储器传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化DMA通道1
    
    DMA_ITConfig(DMA1_Channel1 , DMA_IT_TC , ENABLE);
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
unsigned char uu = 0;
void ADC_Test(void)
{
	int tem[3];
//	char *c = value;

//	AD_value=ADC_GetConversionValue(ADC1);
	tem[0] = (int)ADC_ValueTab[0]*250/4096;
    tem[1] = (int)ADC_ValueTab[1]*250/4096;
    tem[2] = (int)ADC_ValueTab[2]*250/4096;
	IntToChar(tem[0]);
    IntToChar(tem[1]);
//	TFTShowString24(50,250,c,BLUE,0);
    SetEditTmp(&MAIN_WIN_ADC,tem[0]);
    SetEditTmp(&MAIN_WIN_ADC1,tem[1]);
    SetEditTmp(&MAIN_WIN_VBAT,tem[2]);
	#ifdef DEBUG
//    printf("ADC: %.3f \r\n",(float)ADC_ValueTab[1]*336/409600);
      printf("袖带压: %.2f V 脉搏: %.2f V 电池: %.2f V\r\n",(float)tem[0]/100,(float)tem[1]/100,(float)tem[2]/100);
//        printf("%d",tem[1]);
    #endif
}
void Draw_xy(void)
{
    TFTHLine(0,280,240,BLUE);
    TFTVLine(10,20,280,BLUE);      
}
void Draw_wave(void)
{
//    u16 tem[3];
    u16 Sum_Value0,Sum_Value1;
//    u16 Sum_Value2;
    u8 Value[6];
    u8 n;
    
    n = 0;
    Sum_Value0 = 0;
    Sum_Value1 = 0;
//    tem[0] = ADC_ValueTab[0];
//    tem[1] = ADC_ValueTab[1];
//    tem[2] = ADC_ValueTab[2];
    while(n<10)
    {
        if(ADC_DMA_TRSMIT_OK==1)
        {
            ADC_DMA_TRSMIT_OK = 0;
            Sum_Value0 += ADC_ValueTab[0];
//            Sum_Value2 += ADC_ValueTab[0];
            Sum_Value1 += ADC_ValueTab[1];
            n++;
        }
    }
    Sum_Value0 = Sum_Value0/10;
    Sum_Value1 = Sum_Value1/10;
//    if(Sum_Value1>1000)
//    {
//        Sum_Value0 = Sum_Value1;
//    }
//    else Sum_Value0 = 900;
    if(change_wait==1 && Sum_Value0>1280)
    {
        duty_change = 1;
        change_wait = 0;
    }
    if(change_wait==3 && Sum_Value0>3000)
    {
        duty_change = 3;
        change_wait = 0;
    }
    if(change_wait==2 && Sum_Value0>2045)
    {
        change_wait = 0;
        duty_change = 2;
    }
    Value[0] = 0xaa;
    Value[1] = 0x55;
    Value[2] = Sum_Value0&0x00ff;
    Value[3] = (Sum_Value0>>8)&0x00ff;
    Value[4] = Sum_Value1&0x00ff;
    Value[5] = (Sum_Value1>>8)&0x00ff;
    USART3_Puts(Value,6);
}