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
    u8 i=20; //��������Ż��ٶ� ����������͵�5����д�� 
	while(i)  
	{	
        i--;
    }  
} 

bool I2C_Start(void) 
{ 	
    SDA_H; 	SCL_H; 	I2C_delay(); 
	if(!SDA_read)return FALSE; //SDA��Ϊ�͵�ƽ������æ,�˳� 
	SDA_L; 	I2C_delay(); 
	if(SDA_read) return FALSE; //SDA��Ϊ�ߵ�ƽ�����߳���,�˳� 
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

bool I2C_WaitAck(void)   //����Ϊ:=1��ACK,=0��ACK 
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

void I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ// 
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

u8 I2C_ReceiveByte(void)  //���ݴӸ�λ����λ// 
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
	I2C_SendByte(SLA_W);//���ø�������ַ  
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	I2C_SendByte((u8)(WriteAddress>>8));   //���õ���ʼ��ַ       
	I2C_WaitAck(); 
	I2C_SendByte((u8)(WriteAddress));   //���õ���ʼ��ַ       
	I2C_WaitAck(); 
	I2C_SendByte(SendByte); 
	I2C_WaitAck();    
	I2C_Stop();  	
	//ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(5ms) 
    //Systick_Delay_1ms(10); 
	delay_ms(10);
    return TRUE; 
} 

//д��ҳ �ֽ�������С�� ҳ��С
bool iic_wr_page(u8 *ptr,u16 addr,u8 no)
{	
    u8  i;
	
	if(!I2C_Start())return FALSE; 
	I2C_SendByte(SLA_W);//���ø�������ַ  
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	I2C_SendByte((u8)(addr>>8));   //���õ���ʼ��ַ       
	I2C_WaitAck(); 
	I2C_SendByte((u8)(addr));   //���õ���ʼ��ַ       
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

//д�������ֽ�
bool iic_writes(u8 *ptr,u16 addr,u16 no)
{
	unsigned int i = addr;
	
	for(i=0;i<no;i++)
	{
		iic_write(addr+i,ptr[i]);
	}

	return TRUE;
}

//��ȡһ�ֽ�
bool iic_read( u8* ptr, u16 addr) 
{ 	
    if(!I2C_Start())return FALSE; 
	I2C_SendByte(SLA_W);//����������ַ  
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	I2C_SendByte((u8)(addr>>8));   //���õ���ʼ��ַ       
	I2C_WaitAck(); 
	I2C_SendByte((u8)(addr));   //���õ���ʼ��ַ       
	I2C_WaitAck(); 
	
	I2C_Start(); 
	I2C_SendByte(SLA_R); 
	I2C_WaitAck(); 
	*ptr = I2C_ReceiveByte(); 
	I2C_NoAck(); 
	I2C_Stop(); 
	return TRUE; 
} 

//��ȡ�����ֽ�
bool iic_reads( u8*ptr, u16 addr, u16 len)
{ 	
    if(!I2C_Start())return FALSE; 
	I2C_SendByte(SLA_W);//����������ַ  
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;} 
	I2C_SendByte((u8)(addr>>8));   //���õ���ʼ��ַ       
	I2C_WaitAck(); 
	I2C_SendByte((u8)(addr));   //���õ���ʼ��ַ       
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

//�����ʼ512�ֽ�
void iic_clear(void)
{
	u8 buf[64];
	memset(buf,0,sizeof(buf));
	iic_writes(buf,0,sizeof(buf));
}
