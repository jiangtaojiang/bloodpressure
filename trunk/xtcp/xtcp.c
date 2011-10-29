/******************************************************************************
* file Name     : xtcp.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#define xtcp_c
#include "buff.h"
#include "usart.h"
#include "xtcp.h"
#include "business.h"
#include <string.h>
#include "rtc.h"

#include "common.h"
#include "bt.h"

/*************************************************************************
 内部函数
 ************************************************************************/
static U16 pk_poll(void);
static void send_ack(void);
static void send_pk(void *pk);

const U8 crc_table[256]=
{
    0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,
    0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
    0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,
    0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
    0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,
    0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
    0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,
    0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
    0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,
    0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
    0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,
    0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
    0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,
    0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
    0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,
    0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
    0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,
    0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
    0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,
    0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
    0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,
    0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
    0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,
    0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
    0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,
    0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
    0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,
    0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
    0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,
    0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
    0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,
    0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};


/*******************************************************************************
* Function Name  : xsocket_init
* Description    : 初始化通道套接字
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void xsocket_init(void)
{
	memset(&xsck[0],0,sizeof(xsocket));
//	memset(&xsck[1],0,sizeof(xsocket));
}
/*******************************************************************************
* Function Name  : send_ack
* Description    : send_ack
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
static void send_ack(void)
{ 	U8 i;
	xys_tcp_hdr hdr = pk_buf.hdr;
	u8 *ptr = (u8 *)&hdr;
	
	_log("tcp ack:");
	hdr.flag &= 0x0f;
	hdr.flag |= ACK_PK;
	hdr.len[0] = 0;
	hdr.len[1] = 0;	 
	hdr.crc = crc_cal((U8 *)&hdr,HDR_SIZE-1);
	for(i=0;i<HDR_SIZE;i++)
	{	xtcp_outb(ptr[i]);
	}
}
/*******************************************************************************
* Function Name  : pk_poll
* Description    : 提取串口接收队列数据，并进行组包
                    若受到0x55 0xaa，而后面包头校验错误时，只丢掉0x55 0xaa，而不是将整包丢掉
                    若包头校验正确，而业务数据错误，则将全部丢掉
* Input          : none
* Output         : None
* Return         : 返回包长   收到一包数据
                    0          没有收满一包数据
*******************************************************************************/
static U16 pk_poll(void)
{	static U16 bus_len  = 0;//业务数据长度，需要静态
	static U8  state    = 0;//组包状态，(分成四个状态 0 1 2 3->收到SYN1，收到SYN2，包头数据处理,业务数据处理)需要静态
	
	U8  rcv_char = 0;
	U16 pk_len   = 0;

	if(!buff_dlen(&xtcp_buf))		 //buff无数据，直接返回
		return 0;

	switch (state){
		case 0://检测同步字段SYN1，若检测到，则转向state 1
			buff_out(&xtcp_buf, &rcv_char, 1);
			if( rcv_char == SYN1 )
			{	state = 1;
			}
			break;
		case 1://检测同步字段SYN2，若检测到，则转向state 2
			buff_out(&xtcp_buf, &rcv_char, 1);
			if( (rcv_char == SYN2) )
			{	state = 2;
				pk_buf.hdr.sync[0] = 0x55;
				pk_buf.hdr.sync[1] = 0xAA;
				pk_time_out   = PK_RCV_TIMEOUT;
			}		
			break;
		case 2://找到同步字段，接收包头，若包头正确，且有业务数据，转向state3
			if( buff_dlen(&xtcp_buf) >= (sizeof(xys_tcp_hdr)-2) )//收到至少一个包头，采用数据块出队，增加效率
			{	buff_try( &xtcp_buf, ((U8 *)(&pk_buf))+2, sizeof(xys_tcp_hdr)-2 );//包头的55AA已经处理
				if( pk_buf.hdr.crc == crc_cal(&pk_buf, HDR_SIZE-1) )	//包头CRC校验
	//			if(1)		//不管包头CRC校验
				{
					//包头采用buff_try出队，若包头校验错误，只丢掉包头的0x55 0xaa，程序将继续从0x55 aa后开始寻找
				    //包头同步字段
					buff_out( &xtcp_buf, (BYTE *)0, sizeof(xys_tcp_hdr)-2 ); //若包头校验正确 采用buff_out出队

					bus_len = pk_buf.hdr.len[0] + pk_buf.hdr.len[1] * 256;//1字节的校验
					if(bus_len > 0)
					{	if(bus_len <= OP_CODE_LEN)
						{	state = 3;			//有业务数据，数据两小于定义的最大长度 转向state3接收业务数据
						
						}
						else
						{	state = 0;}//有数据，但数据长度超过定义的程度，丢弃该包
					}
					else
					{	state  = 0;	//包结束，转向state0
						pk_len = sizeof(xys_tcp_hdr);	//无业务数据，返回包头长度（该包可能是ACK包）
					}
				}
				else
				{	state = 0;//若校验错误，丢弃该包，转向state0
				}
				
			}
			break;
		case 3://接收业务数据，若业务数据校验错误，丢弃该包，并转向state0
			if( buff_dlen(&xtcp_buf) >= bus_len )
			{	buff_out( &xtcp_buf, ((U8 *)(&pk_buf))+HDR_SIZE, bus_len );
//				if( pk_buf.business[bus_len-1] == SumCheck((U8 *)&pk_buf.business, bus_len-1) )	 //业务数据和校验
				if(1)	//不进行业务数据和校验
				{	pk_len = bus_len + sizeof(xys_tcp_hdr);
				} 
				state = 0;
			}
			break;
		default:
			break;
	}
	return pk_len;
}

/*******************************************************************************
* Function Name  : pk_read
* Description    : 在指定超时时间内   收取一个数据包 timeout  
：超时时间 ms			一般一包数据小于500ms
* Input          : none
* Output         : None
* Return         : 返回包长度：收到一包数据
*******************************************************************************/
U16 pk_read(void)
{
	U16 pk_len;


	pk_time_out = 0;	//初始设置超时为0，在pk_poll（）中检测到包开始时，才启动超时定时器
//	do//数据包超时也显得没意义了
//	{
		pk_len = pk_poll();	
		if(pk_len >= 10)	 //包长合法才接收
		{
#if DEBUG == 1			
			U16 i;
			U8 * ptr= (U8 *)&pk_buf;
			xys_tcp_hdr *hdr = &pk_buf.hdr;

			_log("rcv  pk:");
			for(i=0;i<pk_len;i++)
			{	printf("%02x ",ptr[i]);
			}
			printf("id:%d sn:%d busi_len:%d",hdr->id,hdr->sn,hdr->len[0]+hdr->len[1]*256);		
#endif
			return pk_len;
		}
//	}while(pk_time_out);
	return 0;		
}
/*******************************************************************************
* Function Name  : xtcp_in
* Description    : 到数据包后调用，处理回复，若有数据，提交给应用层
* Input          : none
* Output         : None
* Return         : 
*******************************************************************************/
void xtcp_in(void)
{	xsocket *sck;
	xys_tcp_hdr *hdr = &pk_buf.hdr;
	uint16_t len = 0;

	if(pk_buf.hdr.port == 0)//通道验证
	{	sck = &xsck[0];
	}
	else
	{	sck = NULL;
	}
	
	if(sck)		//通道验证
	{	switch(pk_buf.hdr.flag)
		{	case DATA_PK:
				send_ack();	//以原序号回ACK
				if(hdr->sn == sck->sn_d)	//序号验证
				{	
					len = hdr->len[0] + hdr->len[1]*256;			
					if(len)								//业务数据长度验证
					{	sck->flag |= SCK_NEWDATA;
						sck->sn_d++; 					//更新希望收到的序号
						_log("xtcp: dt pk, busi deal");
						business_deal(pk_buf.business,len);				//若有数据，提交给应用层
					}
					else
					{	_log("xtcp: dt pk without busi data");
					}
				}
				else						 
				{	_log("xtcp: dt pk, sn error");
				#if DEBUG == 1
					printf("id:%d sn:%d busi_len:%d",hdr->id,hdr->sn,hdr->len[0]+hdr->len[1]*256);		
				#endif
				}				
				break;
			case ACK_PK:
				if( sck->flag & SCK_REMIT_FLAG ) 		//是否有未确认的包	 若未收到应答，包重传标志为1
				{	if(sck->sn_s == hdr->sn)			//序号验证
					{	sck->flag &= ~SCK_REMIT_FLAG;	//包发送成功，清除重传标志
						sck->sn_s++;					//下一个发送包的序号
						_log("xtcp: ack, send ok");
					}
					else
					{	_log("xtcp: ack, sn error");
					#if DEBUG == 1
						printf("id:%d sn:%d busi_len:%d",hdr->id,hdr->sn,hdr->len[0]+hdr->len[1]*256);		
					#endif
					}
				}
				else
				{	_log("xtcp: ack, useless ack");
				}
				break;
			case DISCONNECT_PK:
				_log("xtcp: disc pk");
				{
					U8 i;
					xys_tcp_hdr *hdr = &pk_buf.hdr;	  //修改收到的数据包后，直接发送，因为业务层不再管包头
					U8 *ptr = (U8 *)hdr;

					hdr->flag &= 0x0f;
					hdr->flag |= DISCONNECT_PK;
					hdr->len[0] = 0;
					hdr->len[1] = 0;	 
					hdr->crc = crc_cal((U8 *)hdr,sizeof(xys_tcp_hdr)-1);
					for(i=0;i<sizeof(xys_tcp_hdr);i++)
					{	xtcp_outb(ptr[i]);
					}
					
					delay_ms(3000);
					at("+disc",2000);
//					BT_PWR_OFF();
					standby();
				}
				break;
			case HEART_PK:
				_log("xtcp: heart pk");
				if(hdr->sn == sck->sn_d)			//序号验证
				{	sck->sn_d++;
				}
				send_ack();
				break;
			default:
				_log("xtcp: useless pk");
				break;
		}
	}
}

/*******************************************************************************
* Function Name  : xtcp_out
* Description    : 送数据包，填充通道buf后，调用该函数启动发送
* Input          : channel 通道号  busi_len 业务数据长度（预先在业务层填充好）
* Output         : None
* Return         : 
*******************************************************************************/
void xtcp_out(U8 channel,U16 busi_len)
{	xsocket *sck;

	if(channel == 0)
	{	sck = &xsck[0];
	}
	else
	{	sck = NULL;
	}
	
	if(sck)
	{	if( !(sck->flag & SCK_REMIT_FLAG) )	//若之前的包发送成功	
		{	//填充包头
			xys_tcp_hdr *hdr = (xys_tcp_hdr *)&sck->buf;
			hdr->sync[0] = 0x55;
			hdr->sync[1] = 0xaa;
			hdr->port    = 0;
			hdr->flag    = DATA_PK;
			hdr->id      = CM_ID;
			hdr->sn      = sck->sn_s;	
			hdr->len[0]  = busi_len & 0x00ff; 
			hdr->len[1]  = busi_len >> 8;
			hdr->reserve = 0;
			hdr->crc = crc_cal(&xsck[0].buf,sizeof(xys_tcp_hdr)-1);

			//配置套接字信息，以启用发送
			sck->pk_len      = HDR_SIZE + busi_len + 1;	//包长度，用于重传			
			sck->flag       |= SCK_REMIT_FLAG;					
			sck->flag       &= ~SCK_REMIT_TIMEOUT;
			sck->time_out    = PK_REMIT_TIMEOUT;
			sck->remit_count = PK_REMIT_COUNT-1;
			sck->buf[HDR_SIZE + busi_len] = SumCheck(pBUSI,busi_len);
	
			//send_pk(sck->buf); //send packet data
			_log("snd pk:");
			xtcp_outs(sck->buf,sck->pk_len);
#if DEBUG == 1
			printf("id:%d sn:%d busi_len:%d",hdr->id,hdr->sn,hdr->len[0]+hdr->len[1]*256);	
#endif			
		}
		else
		{	_log("KO xtcp: the prev pk is sending, can't send a new pk");
		}
	}
	else
	{	_log("KO xtcp: channel error");
	}		
}

/*******************************************************************************
* Function Name  : xtcp_disc
* Description    : xtcp_disc
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
void xtcp_disc(void)
{	xys_tcp_hdr hdr;
	hdr.sync[0] = 0x55;
	hdr.sync[1] = 0xaa;
	hdr.port = 0;	
	hdr.id = ID;	
	hdr.flag = DISCONNECT_PK;
	hdr.sn = xsck[0].sn_s;
	hdr.len[0] = 0;
	hdr.len[1] = 0;
	hdr.reserve = 0;
	hdr.crc = crc_cal(&hdr,HDR_SIZE-1);
	xtcp_out(0,HDR_SIZE);
	
	//bt power off
	//enter standby
}
/*******************************************************************************
* Function Name  : dis_pk
* Description    : 发送dis_pk
				 ：
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void dis_pk(void)
{
	xsocket *sck;
	sck = &xsck[0];
	if( !(sck->flag & SCK_REMIT_FLAG) )	//若之前的包发送成功	
	{	//填充包头
		xys_tcp_hdr *hdr = (xys_tcp_hdr *)&sck->buf;
		hdr->sync[0] = 0x55;
		hdr->sync[1] = 0xaa;
		hdr->port    = 0;
		hdr->flag    = DISCONNECT_PK;
		hdr->id      = CM_ID;
		hdr->sn      = sck->sn_s;	
		hdr->len[0]  = 0; 
		hdr->len[1]  = 0;
		hdr->reserve = 0;
		hdr->crc = crc_cal(&xsck[0].buf,sizeof(xys_tcp_hdr)-1);

		//配置套接字信息，以启用发送
		sck->pk_len      = HDR_SIZE;	//包长度，用于重传			
		sck->flag       |= SCK_REMIT_FLAG;					
		sck->flag       &= ~SCK_REMIT_TIMEOUT;
		sck->time_out    = PK_REMIT_TIMEOUT;
		sck->remit_count = PK_REMIT_COUNT-1;
		sck->buf[HDR_SIZE] = SumCheck(pBUSI,HDR_SIZE);

		//send_pk(sck->buf); //send packet data
		xtcp_outs(sck->buf,sck->pk_len);
	}
}
/*******************************************************************************
* Function Name  : xtcp_remit
* Description    : xtcp_remit
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
void xtcp_remit(void)
{
	U8 i;
									 
	for(i=0;i<CHANNELS;i++)
	{	if(xsck[i].flag & SCK_REMIT_FLAG)
		{	if(xsck[i].remit_count)
			{	if(xsck[i].time_out == 0)	 //若超时，启动发送，并将重传次数减一
			{	_log("xtcp:remit:");
				xsck[i].remit_count--;
				xsck[i].time_out = PK_REMIT_TIMEOUT;
				send_pk(xsck[i].buf);    //do remit
				}
			}
			else
			{	_log("remit timeout, call timeout_deal ...");
				xsck[i].flag |= SCK_REMIT_TIMEOUT;  //重传超时，告知应用层
				timeout_deal(i);
			}
		}
	}
}
/*******************************************************************************
* Function Name  : send_pk
* Description    : 发送业务数据包
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
static void send_pk(void *pk)
{
	xys_tcp_hdr *hdr = (xys_tcp_hdr *)pk;
	U16 pk_len,i;
	U8 *ptr_tmp = (U8 *)pk;

	if(hdr)
	{
		pk_len = hdr->len[0] + hdr->len[1] * 256 + sizeof(xys_tcp_hdr);
		if(pk_len > sizeof(xys_tcp_hdr))
		{
			pk_len +=1;	//1字节和校验
		}

		for(i=0;i<pk_len;i++)
		{
			xtcp_outb(ptr_tmp[i]);
		}
	}
}

/*******************************************************************************
* Function Name  : SumCheck
* Description    : SumCheck
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
unsigned char SumCheck(unsigned char *buf, unsigned int len)
{	unsigned short i;
	unsigned char sum_check = 0;
	for(i = 0; i < len; i++)
	{	sum_check ^=  buf[i];
	}
	return sum_check;
}

/*******************************************************************************
* Function Name  : crc_cal
* Description    : crc_cal
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
U8 crc_cal(void *pt, U32 cnt)
{
    U8 crc = 0;
    U8 *ptr = (U8 *)pt;
		while (cnt--)
    {
        crc = crc_table[crc ^ *ptr++];
    }
    return crc;
}


