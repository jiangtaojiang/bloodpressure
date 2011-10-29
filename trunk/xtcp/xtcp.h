#ifndef __XTCP_H__
#define __XTCP_H__

#define U8  unsigned char
#define U16 unsigned short
#define U32 unsigned long


#include "stdint.h"
/*************************************************************************
 配置宏定义
 ************************************************************************/
//业务数据长度(不包括包头的长度)
//来自手机的业务数据一般不会太长
#define OP_CODE_LEN 			(80) 

//发送数据包缓冲大小
#define BUF_SIZE 			(512)

//通道数
#define CHANNELS 1



#define PK_RCV_TIMEOUT   1000
#define PK_REMIT_TIMEOUT 3000 
#define PK_REMIT_COUNT   10	


/*************************************************************************
 接口函数定义
 ************************************************************************/
extern void business_deal(void *ptr, uint16_t len);
extern void timeout_deal(uint8_t ch);
/*************************************************************************
 
 ************************************************************************/
//同步字段定义
#define SYN1 					(0x55)
#define SYN2 					(0xAA)

//数据包类型字段定义
#define DATA_PK 			(0x01<<4)
#define ACK_PK	 			(0x02<<4)
#define HEART_PK 			(0x03<<4)
#define DISCONNECT_PK	(0x04<<4)

//串口接收缓冲区，要求大于包长即可
#define PK_BUF_SIZE 			(sizeof(packet))	

//xsocket.flag标志定义
#define SCK_NEWDATA       (1<<0)
#define SCK_REMIT_TIMEOUT (1<<1)
#define SCK_REMIT_FLAG    (1<<2)



#define CM_ID 0x06  //血压采集器ID

/*************************************************************************
 
 ************************************************************************/
//包头结构
typedef struct {
	U8 sync[2];		 //该字段交由收包程序标识包开始，不必加入传输层
	U8 port;
	U8 flag;
	U8 id;
	U8 sn;
	U8 len[2];
	U8 reserve;
	U8 crc;
}xys_tcp_hdr;


//数据包结构
typedef struct{
	xys_tcp_hdr hdr;
	U8 business[OP_CODE_LEN];
}packet;

//端口套接字
typedef struct{
	U8  sn_d;		//希望收到的需要
	U8  sn_s;		//发送时采用该序号，发送成功后加1
	U16 time_out;			 					//重传超时定时器
	U8  flag;                   //标识 新数据到来、重传、或者重传超时等
	U16 pk_len;		//包长  用于重传使用
	U8  remit_count;						//重传次数
	U8  buf[BUF_SIZE];	 				//缓冲区，可用于重传
}xsocket;

/*************************************************************************
 全局变量
 ************************************************************************/
#define CONNECTED    0x01
#define DISCONNECTED 0x02

#define HDR_SIZE (sizeof(xys_tcp_hdr))
#define pBUSI (&(xsck[0].buf[HDR_SIZE]))

#define xtcp_init xsocket_init
#define xtcp_read pk_read



/*************************************************************************
 接口函数
 ************************************************************************/\

#ifdef xtcp_c	
//全局变量
volatile U16 pk_time_out = 0;	  	//收包超时定时器，对其设置一个初值，定时中断会每隔1毫秒减1，为0表示超时
packet   pk_buf;					//包接收缓冲区
xsocket  xsck[CHANNELS];

U8 xtcp_state = 0;   
   
void xsocket_init(void);
U16 pk_read(void);
void xtcp_in(void);
void xtcp_out(U8 channel,U16 busi_len);//业务数据以启动发送
//发送数据前 应先用xtcp_out_wait()等待之前数据包发送完成（或超时），才能填充

void xtcp_remit(void);
U8 crc_cal(void *pt, U32 cnt);
unsigned char SumCheck(unsigned char *buf, unsigned int len);
void xtcp_disc(void);
void dis_pk(void);

#else
extern U8 xtcp_state;
extern packet pk_buf;	//包接收缓冲区							  
extern xsocket  xsck[1];
extern volatile U16 pk_time_out; //收包超时定时器，对其设置一个初值，定时中断会每隔1毫秒减1，为0表示超时

extern void xsocket_init(void);
extern U16 pk_read(void);
extern void xtcp_in(void);
extern void xtcp_out(U8 channel,U16 busi_len);
extern void xtcp_remit(void);
extern void xtcp_disc(void);
extern void dis_pk(void);
#endif
#endif


