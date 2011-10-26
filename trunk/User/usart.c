/******************************************************************************
* file Name     : usart.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#define  usart_c
#include "stm32f10x_conf.h"
#include "main.h"
#include "usart.h"
//#include "UserAPP.h"
/*******************************************************************************
* Function Name  : Usart_Config
* Description    : usart配置
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void Usart_Config(USART_TypeDef *uart, uint32_t bps)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	
	USART_InitStructure.USART_BaudRate = bps;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	
	USART_ClockInit(uart, &USART_ClockInitStructure);
	USART_Init(uart, &USART_InitStructure);

	USART_ClearITPendingBit(uart, USART_IT_RXNE);
}
/*******************************************************************************
* Function Name  : Usart_Init
* Description    : usart IO配置，初始化。
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void Usart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//enable clock of usart1			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  //enable clock of usart2
 	 /* Enable DMA1 clock */
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
#ifdef DEBUG1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);  //enable clock of usart3
	
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_10;//Usart3_Tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	// Configure USART1 USART3 Rx as input floating 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//Usart3_Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	Usart_Config(USART3,115200);
	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);	
#endif

	/* Configure USART1 USART2 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_9 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	/* Configure USART1 USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	Usart_Config(USART1,115200);
	Usart_Config(USART2,115200);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	/* Enable the USARTx */
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
    //DMA 工作方式
    
}
/*******************************************************************************
* Function Name  : DMA_Configuration
* Description    : DMA_Configuration
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
/*
void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //usart2_TX，DMA设置：
    //设置DMA源：内存地址&串口数据寄存器地址
    //方向：内存-->外设  DMA_DIR_PeripheralSRC和DMA_DIR_PeripheralDST
    //每次传输位：8bit
    //传输大小DMA_BufferSize=SENDBUFF_SIZE
    //地址自增模式：外设地址不增，内存地址自增1
    //DMA模式：一次传输，非循环
    //优先级：中
    DMA_DeInit(DMA1_Channel7);//usart2_TX,通道7
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Base;//DMA的源
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)sendbuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//方向：
    DMA_InitStructure.DMA_BufferSize = BLOCK_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
      
    //DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE); //配置传输完成中断
}*/
/*******************************************************************************
* Function Name  : DMA_Configuration
* Description    : DMA_Configuration
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void DMA_TxConfig(BYTE *BufferSRC, uint32_t BufferSize)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //usart2_TX，DMA设置：
    //设置DMA源：内存地址&串口数据寄存器地址
    //方向：内存-->外设  DMA_DIR_PeripheralSRC和DMA_DIR_PeripheralDST
    //每次传输位：8bit
    //传输大小DMA_BufferSize=SENDBUFF_SIZE
    //地址自增模式：外设地址不增，内存地址自增1
    //DMA模式：一次传输，非循环
    //优先级：中
    DMA_DeInit(DMA1_Channel7);//usart2_TX,通道7
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Base;//DMA的源
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)BufferSRC;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//方向：
    DMA_InitStructure.DMA_BufferSize = BufferSize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
      
    //DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE); //配置传输完成中断
}
/*******************************************************************************
* Function Name  : USART2_Puts
* Description    : USART2_Puts
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void USART2_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }

}