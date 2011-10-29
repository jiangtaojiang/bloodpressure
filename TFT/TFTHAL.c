#define TFTHAL_c
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include "main.h"
#include "TFTHal.h"
#include "RCC_config.h"


/*******************************************************************************
* Function Name  : LCD_GPIOConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable FSMC clocks */
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  	
  	/* 
    PD.14----(D0) 
    PD.15----(D1)
    PD.00----(D2)
    PD.01----(D3) 
    PD.04----(NOE) 
    PD.05----(NWE) 
    PD.07----(NE1)   
    PD.08----(D13)
    PD0.9----(D14)
    PD,10----(D15)
    as alternate function push pull */    
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_7 |
                                 GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_14 | GPIO_Pin_15;
                                
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  	
  	 /* Set 
    PE.07----(D4) 
    PE.08----(D5) 
    PE.09----(D6) 
    PE.10----(D7) 
    PE.11----(D8) 
    PE.12----(D9)
    PE.13----(D10) 
    PE.14----(D11)
    PE.15----(D12)
    as alternate function push pull */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
  	
  	/* Set PD.11(A16 (RS)) as alternate function push pull */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  	/*set pb.0 rst*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*set pc.6 back led*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  p;
  	
  	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 1 ----------------------------------------*/
  	/* FSMC_Bank1_NORSRAM4 configuration */
  	p.FSMC_AddressSetupTime = 2;
  	p.FSMC_AddressHoldTime = 0;
  	p.FSMC_DataSetupTime = 5;
  	p.FSMC_BusTurnAroundDuration = 0;
  	p.FSMC_CLKDivision = 0;
  	p.FSMC_DataLatency = 0;
  	p.FSMC_AccessMode = FSMC_AccessMode_B;
  	
  	/* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
    
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

  	/* BANK 4 (of NOR/SRAM Bank 1~4) is enabled */
  	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
/*******************************************************************************
* Function Name  : delay_1
* Description    : delay_1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void delay_1()
{
  vu32 i=0xffff;
  while(i--)
    asm("nop");
}
/*******************************************************************************
* Function Name  : wr_cmd
* Description    : write cmd
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void wr_cmd(u16 cmd)
{
	LCD_Command = cmd;
}
/*******************************************************************************
* Function Name  : wr_data
* Description    : write date
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void wr_data(u16 dat)
{
	LCD_Data = dat;
}
/*******************************************************************************
* Function Name  : LCD_WR_REG
* Description    : LCD_WR_REG 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_REG(u16 cmd,u16 dat)
{
    wr_cmd(cmd);
//    delay_1();
    wr_data(dat);
}
/*******************************************************************************
* Function Name  : CheckController
* Description    : CheckController 
* Input          : None
* Output         : None
* Return         : Controller ID ILI9331
*******************************************************************************/
u16 CheckController(void)
{
    u16 tem;
    LCD_WR_REG(0x0000,0x0001);  //start oscillation
    tem = LCD_Data;
    return tem;
}
/*******************************************************************************
* Function Name  : lcd_initialize
* Description    : lcd_initialize 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void lcd_initialize(void)
{
//    u16 i;
    LCD_GPIOConfig();
    LCD_FSMCConfig();
  
    RSTTFT_H;
	SysTickDelay(10);
	RSTTFT_L;
	SysTickDelay(10);
	RSTTFT_H;
	SysTickDelay(10);
      
//    i = CheckController();
//    Write_Log("");
//    printf("Controller: %4x",i);
#ifdef ILI9331	/*0x9331ÏµÁÐµÄÆÁ*/
	
	LCD_WR_REG(0x00E7, 0x1014);
    LCD_WR_REG(0x0001, 0x0100); // set SS and SM bit
    LCD_WR_REG(0x0002, 0x0200); // set 1 line inversion
    LCD_WR_REG(0x0003,(1<<12)|(3<<4)|(0<<3));//65K    
    //LCD_WR_REG(0x0003, 0x1030); // set GRAM write direction and BGR=1.
    LCD_WR_REG(0x0008, 0x0202); // set the back porch and front porch
    LCD_WR_REG(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_WR_REG(0x000A, 0x0000); // FMARK function
    LCD_WR_REG(0x000C, 0x0000); // RGB interface setting
    LCD_WR_REG(0x000D, 0x0000); // Frame marker Position
    LCD_WR_REG(0x000F, 0x0000); // RGB interface polarity
    //*************Power On sequence ****************//
    LCD_WR_REG(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WR_REG(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_WR_REG(0x0012, 0x0000); // VREG1OUT voltage
    LCD_WR_REG(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
    SysTickDelay(200); // Dis-charge capacitor power voltage
    LCD_WR_REG(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WR_REG(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
    SysTickDelay(50); // Delay 50ms
    LCD_WR_REG(0x0012, 0x000C); // Internal reference voltage= Vci;
    SysTickDelay(50); // Delay 50ms
    LCD_WR_REG(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
    LCD_WR_REG(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
    LCD_WR_REG(0x002B, 0x000B); // Set Frame Rate
    SysTickDelay(50); // Delay 50ms
    LCD_WR_REG(0x0020, 0x0000); // GRAM horizontal Address
    LCD_WR_REG(0x0021, 0x013f); // GRAM Vertical Address
    // ----------- Adjust the Gamma Curve ----------//
    LCD_WR_REG(0x0030, 0x0000);
    LCD_WR_REG(0x0031, 0x0106);
    LCD_WR_REG(0x0032, 0x0000);
    LCD_WR_REG(0x0035, 0x0204);
    LCD_WR_REG(0x0036, 0x160A);
    LCD_WR_REG(0x0037, 0x0707);
    LCD_WR_REG(0x0038, 0x0106);
    LCD_WR_REG(0x0039, 0x0707);
    LCD_WR_REG(0x003C, 0x0402);
    LCD_WR_REG(0x003D, 0x0C0F);
    //------------------ Set GRAM area ---------------//
    LCD_WR_REG(0x0050, 0x0000); // Horizontal GRAM Start Address
    LCD_WR_REG(0x0051, 0x00EF); // Horizontal GRAM End Address
    LCD_WR_REG(0x0052, 0x0000); // Vertical GRAM Start Address
    LCD_WR_REG(0x0053, 0x013F); // Vertical GRAM Start Address
    LCD_WR_REG(0x0060, 0x2700); // Gate Scan Line
    LCD_WR_REG(0x0061, 0x0001); // NDL,VLE, REV 
    LCD_WR_REG(0x006A, 0x0000); // set scrolling line
    //-------------- Partial Display Control ---------//
    LCD_WR_REG(0x0080, 0x0000);
    LCD_WR_REG(0x0081, 0x0000);
    LCD_WR_REG(0x0082, 0x0000);
    LCD_WR_REG(0x0083, 0x0000);
    LCD_WR_REG(0x0084, 0x0000);
    LCD_WR_REG(0x0085, 0x0000);
    //-------------- Panel Control -------------------//
    LCD_WR_REG(0x0090, 0x0010);
    LCD_WR_REG(0x0092, 0x0600);
    LCD_WR_REG(0x0007, 0x0133); // 262K color and display ON
#endif 


}




