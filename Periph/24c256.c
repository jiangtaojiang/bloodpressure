/******************************************************************************
* file Name     : 24c256.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chenshuiqing                                                                           
* Updata        : chenshuiqing   
* brief         : 
*******************************************************************************/
#define AT24c256_c
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "24c256.h" 
#include "usart.h"
#include "common.h" 

void iic_init(void) 
{	
    GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* Configure I2C1 pins: SCL and SDA */ 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
} 

static void I2C_delay(void) 
{   
    u8 i=20; //这里可以优化速度 ，经测试最低到5还能写入 
	while(i)  
	{	
        i--;
    }  
} 

bool I2C_Start(void) 
{ 	
    SDA_H; 	SCL_H; 	I2C_delay(); 
	if(!SDA_read)return FALSE; //SDA线为低电平则总线忙,退出 
	SDA_L; 	I2C_delay(); 
	if(SDA_read) return FALSE; //SDA线为高电平则总线出错,退出 
	SDA_L; 	I2C_delay(); 
	return TRUE; 
} 

void I2C_Stop(void) 
{ 	
    SCL_L; 	I2C_delay(); 
	SDA_L; 	I2C_delay(); 
	SCL_H; 	I2C_delay(); 
	SDA_H; 	I2C_delay(); 
} 

void I2C_Ack(void) 
{ 	
    SCL_L; 	I2C_delay(); 
	SDA_L; 	I2C_delay(); 
	SCL_H; 	I2C_delay(); 
	SCL_L; 	I2C_delay(); 
} 


void I2C_NoAck(void) 
{ 	
    SCL_L; I2C_delay(); 
	SDA_H; I2C_delay(); 
	SCL_H; I2C_delay(); 
	SCL_L; I2C_delay(); 
} 

bool I2C_WaitAck(void)   //返回为:=1有ACK,=0无ACK 
{	
    SCL_L; 	I2C_delay(); 
	SDA_H; 	I2C_delay(); 
	SCL_H; 	I2C_delay(); 
	if(SDA_read) 
	{	
        SCL_L; 
		return FALSE; 
	} 
	SCL_L; 
	return TRUE; 
} 

void I2C_SendByte(u8 SendByte) //数据从高位到低位// 
{ 
	u8 i=8; 
	while(i--) 
	{ 
		SCL_L; 	I2C_delay(); 
		if(SendByte&0x80) 
		{	
            SDA_H; 
		}  
		else  
		{	
            SDA_L;  
		}  
		SendByte<<=1; 
		I2C_delay(); 
		SCL_H; 	I2C_delay(); 
	} 
	SCL_L; 
} 

u8 I2C_ReceiveByte(void)  //数据从高位到低位// 
{	
    u8 i=8; 
	u8 ReceiveByte=0; 
	
	SDA_H; 
	while(i--) 
	{	
        ReceiveByte<<=1;       
		SCL_L; 	I2C_delay(); 
		SCL_H; 	I2C_delay(); 
		if(SDA_read) 
		{	
            ReceiveByte|=0x01; 
		} 
	} 
	SCL_L; 
	return ReceiveByte; 
} 

bool iic_write(u16 WriteAddress, u8 SendByte ) 
{ 
	if(!I2C_Start())return FALSE; 
	I2C_SendByte(SLA_W);//设置高器件地址  
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	I2C_SendByte((u8)(WriteAddress>>8));   //设置低起始地址       
	I2C_WaitAck(); 
	I2C_SendByte((u8)(WriteAddress));   //设置低起始地址       
	I2C_WaitAck(); 
	I2C_SendByte(SendByte); 
	I2C_WaitAck();    
	I2C_Stop();  	
	//注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(5ms) 
    //Systick_Delay_1ms(10); 
	delay_ms(10);
    return TRUE; 
} 

//写入页 字节数必须小于 页大小
bool iic_wr_page(u8 *ptr,u16 addr,u8 no)
{	
    u8  i;
	
	if(!I2C_Start())return FALSE; 
	I2C_SendByte(SLA_W);//设置高器件地址  
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	I2C_SendByte((u8)(addr>>8));   //设置低起始地址       
	I2C_WaitAck(); 
	I2C_SendByte((u8)(addr));   //设置低起始地址       
	I2C_WaitAck();  

	for(i=0;i<no;i++)
	{	
        I2C_SendByte(*ptr++); 
		I2C_WaitAck();
	}
	I2C_Stop(); 
	delay_ms(5);
	return TRUE;
}

//写入若干字节
bool iic_writes(u8 *ptr,u16 addr,u16 no)
{
	unsigned int i = addr;
	
	for(i=0;i<no;i++)
	{
		iic_write(addr+i,ptr[i]);
	}

	return TRUE;
}

//读取一字节
bool iic_read( u8* ptr, u16 addr) 
{ 	
    if(!I2C_Start())return FALSE; 
	I2C_SendByte(SLA_W);//设置器件地址  
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	I2C_SendByte((u8)(addr>>8));   //设置低起始地址       
	I2C_WaitAck(); 
	I2C_SendByte((u8)(addr));   //设置低起始地址       
	I2C_WaitAck(); 
	
	I2C_Start(); 
	I2C_SendByte(SLA_R); 
	I2C_WaitAck(); 
	*ptr = I2C_ReceiveByte(); 
	I2C_NoAck(); 
	I2C_Stop(); 
	return TRUE; 
} 

//读取若干字节
bool iic_reads( u8*ptr, u16 addr, u16 len)
{ 	
    if(!I2C_Start())return FALSE; 
	I2C_SendByte(SLA_W);//设置器件地址  
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	I2C_SendByte((u8)(addr>>8));   //设置低起始地址       
	I2C_WaitAck(); 
	I2C_SendByte((u8)(addr));   //设置低起始地址       
	I2C_WaitAck(); 
	
	I2C_Start(); 
	I2C_SendByte(SLA_R); 
	I2C_WaitAck();
	
	while(len)
	{	
        *ptr = I2C_ReceiveByte(); 
		if(len == 1)
		{
			I2C_NoAck();
		}
		else
		{	
            I2C_Ack();
		}
		len--;
		ptr++;
	}
	I2C_Stop(); 
	return TRUE;
}

//清除开始512字节
void iic_clear(void)
{
	u8 buf[64];
	memset(buf,0,sizeof(buf));
	iic_writes(buf,0,sizeof(buf));
}
