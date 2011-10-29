/******************************************************************************
* file Name     : bt.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#define bt_c
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usart.h"
#include "common.h"
#include "buff.h"
#include "bt.h"
#include "xtcp.h"
#include "24c256.h"
#include "rtc.h"

/*******************************************************************************
* Function Name  : BT_Init
* Description    : BT_Init IO配置，初始化。
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void BT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BT_PWR_PIN | BT_AT_PIN | BT_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BT_LINK_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	BT_PWR_OFF();											// 开蓝牙电源
	BT_EXIT_RST();
}

/*******************************************************************************
* Function Name  : at
* Description    : at +link= ;at  ;at +name
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void at(char *ptr, int timeout)
{	BYTE rcv_ch[30];
	char temp;
	char i;

	Usart_Config(bt_uart,38400);
	BT_ENTER_ATMODE();
	delay_ms(50);
		
	buff_out(&xtcp_buf,0,buff_dlen(&xtcp_buf));//clear buffer
	memset(rcv_ch,0,sizeof(rcv_ch));
	xtcp_outs("at",2);
	if(ptr)
	{	
		xtcp_outs(ptr,strlen(ptr));
	}
	xtcp_outs("\r\n",2);
	delay_ms(timeout);
	temp = buff_dlen(&xtcp_buf);
	buff_out(&xtcp_buf,rcv_ch,temp);
	for(i=0;i<temp;i++)
	{	printf("%c",rcv_ch[i]);
	}
	
	BT_EXIT_ATMODE();
	Usart_Config(bt_uart,115200);	
	delay_ms(100);
}
/*******************************************************************************
* Function Name  : bt_reset
* Description    : bt_reset
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void bt_reset(void)
{
	BT_PWR_OFF();
	BT_PWR_ON();
	delay_ms(100);
	BT_ENTER_RST();
	delay_ms(200);
	BT_EXIT_RST();
}
/*******************************************************************************
* Function Name  : link_test
* Description    : 主动发起link
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
unsigned char link_test(void)
{
	unsigned int timeout;	
	uint8_t phone_flag = 0;
	
	//           0123456789abcdef0123
	char addr[]="+link=0000,00,000000";
	uint8_t addr_asc[12];
	
	BT_PWR_ON();		//有新数据，打开蓝牙
	delay_ms(1000);
	
	iic_reads(addr_asc,BTH_MAC_BASE,sizeof(addr_asc));
	memcpy(&addr[6],&addr_asc[0],4);
	memcpy(&addr[0xb],&addr_asc[4],2);
	memcpy(&addr[0xe],&addr_asc[6],6);
	_log(addr);
	_log("主动连接");
	at(addr,2000);
	
//------------------修改 2011-3-4 14:47	-------------------
	iic_reads(&phone_flag,PHONE_FLAG_BASE,1);
#if DEBUG == 1
	printf("\r\nflag:%d\r\n",phone_flag);
#endif
	if((phone_flag != NO_NEED_DISC) && (phone_flag != NEED_DISC))
	{
		_log("phone_flag error");
	//	return 0;
	}
	if(phone_flag == NEED_DISC)
	{
		uint16_t tm_out;
		
		tm_out = 100;	//连接超时，若连上，就进行断开
		do{
			delay_ms(40);				
			if(BT_LINK_STATE())
			{
				_log("linked");
				break;
			}
		}while(--tm_out);
		
		if(tm_out == 0)
		{
			_log("link tmo");
		//	return 0;
		}
		
		_log("disc");
		at("+disc",500);
		tm_out = 8;
		do{
			//delay_ms(10);		
			if(BT_LINK_STATE() == 0)
			{
				_log("disced");
				break;
			}
			at("+disc",500);
		}while(--tm_out);
		
		if(tm_out == 0)	//断开超时
		{
		//	at("+disc",100);
			_log("disc tmo");
		//	return 0;
		}
	}
//----------修改结束--------------		
	
	_log("wait for link");
	timeout = 1000;					//2000*10 = 20s	
	do{
		delay_ms(10);			
		if(BT_LINK_STATE())
			return 1;
	}while(--timeout);

	return 0;
}
/*******************************************************************************
* Function Name  : bt_disc
* Description    : bt_disc
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void bt_disc(void)
{
	BYTE rcv_ch[30];
	char addr[]="at+disc\r";
	char temp;
	char i;

	Usart_Config(bt_uart,38400);
	BT_ENTER_ATMODE();
	delay_ms(50);
		
	buff_out(&xtcp_buf,0,buff_dlen(&xtcp_buf));//clear buffer
	memset(rcv_ch,0,sizeof(rcv_ch));
	xtcp_outs(addr,strlen(addr));
	delay_ms(200);
	temp = buff_dlen(&xtcp_buf);
	buff_out(&xtcp_buf,rcv_ch,temp);
	for(i=0;i<temp;i++)
	{	printf("%c",rcv_ch[i]);
	}
	BT_EXIT_ATMODE();
	Usart_Config(bt_uart,115200);	
}
/*******************************************************************************
* Function Name  : bt_wait_connect
* Description    : 等待蓝牙连接
* Input          : 超时时间 单位 秒
* Output         : None
* Return         : 返回：1：成功  0：失败
*******************************************************************************/
char bt_wait_connect(unsigned long t)
{	
	t=t*100;
	do
	{	if(!t)
		{	return 0;
		}
		t--;
		delay_ms(10);
	}while(xtcp_state != CONNECTED);	
	return 1;
}
/*******************************************************************************
* Function Name  : bt_state
* Description    : 显示蓝牙连接状态
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void bt_state(void)
{
	if(BT_LINK_STATE())
		printf("蓝牙连上\r\n");
	else
		printf(" 蓝牙断开\r\n");
}
