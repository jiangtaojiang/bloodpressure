#ifndef __XTCP_H__
#define __XTCP_H__

#define U8  unsigned char
#define U16 unsigned short
#define U32 unsigned long


#include "stdint.h"
/*************************************************************************
 ���ú궨��
 ************************************************************************/
//ҵ�����ݳ���(��������ͷ�ĳ���)
//�����ֻ���ҵ������һ�㲻��̫��
#define OP_CODE_LEN 			(80) 

//�������ݰ������С
#define BUF_SIZE 			(512)

//ͨ����
#define CHANNELS 1



#define PK_RCV_TIMEOUT   1000
#define PK_REMIT_TIMEOUT 3000 
#define PK_REMIT_COUNT   10	


/*************************************************************************
 �ӿں�������
 ************************************************************************/
extern void business_deal(void *ptr, uint16_t len);
extern void timeout_deal(uint8_t ch);
/*************************************************************************
 
 ************************************************************************/
//ͬ���ֶζ���
#define SYN1 					(0x55)
#define SYN2 					(0xAA)

//���ݰ������ֶζ���
#define DATA_PK 			(0x01<<4)
#define ACK_PK	 			(0x02<<4)
#define HEART_PK 			(0x03<<4)
#define DISCONNECT_PK	(0x04<<4)

//���ڽ��ջ�������Ҫ����ڰ�������
#define PK_BUF_SIZE 			(sizeof(packet))	

//xsocket.flag��־����
#define SCK_NEWDATA       (1<<0)
#define SCK_REMIT_TIMEOUT (1<<1)
#define SCK_REMIT_FLAG    (1<<2)



#define CM_ID 0x06  //Ѫѹ�ɼ���ID

/*************************************************************************
 
 ************************************************************************/
//��ͷ�ṹ
typedef struct {
	U8 sync[2];		 //���ֶν����հ������ʶ����ʼ�����ؼ��봫���
	U8 port;
	U8 flag;
	U8 id;
	U8 sn;
	U8 len[2];
	U8 reserve;
	U8 crc;
}xys_tcp_hdr;


//���ݰ��ṹ
typedef struct{
	xys_tcp_hdr hdr;
	U8 business[OP_CODE_LEN];
}packet;

//�˿��׽���
typedef struct{
	U8  sn_d;		//ϣ���յ�����Ҫ
	U8  sn_s;		//����ʱ���ø���ţ����ͳɹ����1
	U16 time_out;			 					//�ش���ʱ��ʱ��
	U8  flag;                   //��ʶ �����ݵ������ش��������ش���ʱ��
	U16 pk_len;		//����  �����ش�ʹ��
	U8  remit_count;						//�ش�����
	U8  buf[BUF_SIZE];	 				//���������������ش�
}xsocket;

/*************************************************************************
 ȫ�ֱ���
 ************************************************************************/
#define CONNECTED    0x01
#define DISCONNECTED 0x02

#define HDR_SIZE (sizeof(xys_tcp_hdr))
#define pBUSI (&(xsck[0].buf[HDR_SIZE]))

#define xtcp_init xsocket_init
#define xtcp_read pk_read



/*************************************************************************
 �ӿں���
 ************************************************************************/\

#ifdef xtcp_c	
//ȫ�ֱ���
volatile U16 pk_time_out = 0;	  	//�հ���ʱ��ʱ������������һ����ֵ����ʱ�жϻ�ÿ��1�����1��Ϊ0��ʾ��ʱ
packet   pk_buf;					//�����ջ�����
xsocket  xsck[CHANNELS];

U8 xtcp_state = 0;   
   
void xsocket_init(void);
U16 pk_read(void);
void xtcp_in(void);
void xtcp_out(U8 channel,U16 busi_len);//ҵ����������������
//��������ǰ Ӧ����xtcp_out_wait()�ȴ�֮ǰ���ݰ�������ɣ���ʱ�����������

void xtcp_remit(void);
U8 crc_cal(void *pt, U32 cnt);
unsigned char SumCheck(unsigned char *buf, unsigned int len);
void xtcp_disc(void);
void dis_pk(void);

#else
extern U8 xtcp_state;
extern packet pk_buf;	//�����ջ�����							  
extern xsocket  xsck[1];
extern volatile U16 pk_time_out; //�հ���ʱ��ʱ������������һ����ֵ����ʱ�жϻ�ÿ��1�����1��Ϊ0��ʾ��ʱ

extern void xsocket_init(void);
extern U16 pk_read(void);
extern void xtcp_in(void);
extern void xtcp_out(U8 channel,U16 busi_len);
extern void xtcp_remit(void);
extern void xtcp_disc(void);
extern void dis_pk(void);
#endif
#endif


