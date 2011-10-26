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
* Description    : adc����
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void Adc_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	/* ADC1 configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //����ģʽ 
  	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //������ͨ��ģʽ
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //����ת��
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ת������������ 
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //�Ҷ��� 
  	ADC_InitStructure.ADC_NbrOfChannel = 2;  //ɨ��ͨ����  //����ת�����г���Ϊ2
  	ADC_Init(ADC1, &ADC_InitStructure); ////���ú�������ʼ��ADC1
  	
  	/* ADC1 regular channel9 configuration */ 
        ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
   
  	//ͨ��X,����ʱ��Ϊ1.5����,1�������ͨ����1��
        /* Enable ADC1 DMA [ʹ��ADC1 DMA]*/
        ADC_DMACmd(ADC1, ENABLE);
  	/* Enable ADC1 */
  	ADC_Cmd(ADC1, ENABLE);
/******������ADC�Զ�У׼����������ִ��һ�Σ���֤����*******/ 
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
		
	//PB1 ��Ϊģ��ͨ��9��������                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //PA4��Ϊģ��ͨ��4��������                         
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
    DMA_InitTypeDef DMA_InitStructure;//����DMA��ʼ���ṹ��
    DMA_DeInit(DMA1_Channel1);//��λDMAͨ��1
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS; //���� DMAͨ���������ַ=ADC1_DR_Address
    DMA_InitStructure.DMA_MemoryBaseAddr =(u32)ADC_ValueTab; //����DMAͨ���洢����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//ָ������ΪԴ��ַ
    DMA_InitStructure.DMA_BufferSize = 2;//����DMA��������С1
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//��ǰ����Ĵ�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//��ǰ�洢����ַ��
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�����������ݿ��16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //����洢�����ݿ��16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMAͨ������ģʽλ���λ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ�����ȼ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��ֹDMAͨ���洢�����洢������
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��DMAͨ��1
    
//    DMA_ITConfig(DMA1_Channel1 , DMA_IT_TC , ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��DMAͨ��1
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