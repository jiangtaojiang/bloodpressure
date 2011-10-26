/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_conf.h"
#include "main.h"
//#include "usb_istr.h"
//#include "usb_pwr.h"
//#include "stm32_eval_sdio_sd.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}
/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
//  USB_Istr();
}
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern volatile u16 Timer1,Timer2;
extern volatile bool Ms_20;
extern volatile bool Ms_1;	//1MS标志
void SysTick_Handler(void)
{
    static u16 cnt=0;
	if(Timer1)
		Timer1--;
	
	if(Timer2)
		Timer2--;
	
	//20MS标志
	if(++cnt==20)
	{
		cnt=0;
		Ms_20=TRUE;
	}
	Ms_1=TRUE;
}
/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
    /* Process All SDIO Interrupt Sources */
//    SD_ProcessIRQSrc();
}
/**
  * @brief :定时器中断函数，中断周期为1毫秒，可以产生系统需要的超时应用
	* @param :None
	* @retval:None
	*/
void TIM2_IRQHandler (void)
{
    if( TIM2->SR & TIM_FLAG_Update )
    {
        TIM2->SR = ~ TIM_FLAG_Update;	 //clear updata flag
        
        ms_count++;
        if(recont_flag == 1)
        {
            if(ms_count == 10)
            {
                ms_count = 0;
                ms100++;
            }
        }
    }
    
}
void RTC_IRQHandler(void)
{
	if(RTC_GetFlagStatus(RTC_FLAG_SEC) != RESET)
	{	
        RTC_ClearFlag(RTC_FLAG_SEC);
	}
	rtc_sec_flag = 1;
	if(RTC_GetFlagStatus(RTC_FLAG_ALR) != RESET)
	{	
        RTC_ClearFlag(RTC_FLAG_ALR);
	}
}
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
    if(USART1->SR & USART_IT_RXNE)
    {
//        unsigned char rxdata;
//		rxdata = USART1->DR;
        USART1->SR &= ~USART_IT_RXNE;
    }
    if(USART1->SR & USART_IT_TXE)
	{
	    USART1->SR &= ~USART_IT_TXE;
	}
}
/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)  //接蓝牙
{				  
  if(USART2->SR & USART_IT_RXNE)
  {
//    unsigned char rcv_data;
//    rcv_data = USART2->DR;
    
//    buff_in(&xtcp_buf, &rcv_data, 1);
    USART2->SR &= ~USART_IT_RXNE;		
  }
  
  if(USART2->SR & USART_IT_TXE)
  {
    USART2->SR &= ~USART_IT_TXE;
  }
}
void USART3_IRQHandler(void)  
{
	if(USART3->SR & USART_IT_RXNE) 
	{	

	}

	if(USART3->SR & USART_IT_TXE)
	{	
		USART3->SR &= ~USART_IT_TXE;
	}

}
void PVD_IRQHandler(void)
{
    
}
void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC7))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL7);
    }
}
void DMA1_Channel1_IRQHandler(void) 
{ 
    if(DMA_GetITStatus(DMA1_IT_TC1)) 
    { 
        ADC_DMA_TRSMIT_OK = 1; 
        DMA_ClearITPendingBit(DMA1_IT_GL1); 
    } 
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
