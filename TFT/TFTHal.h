/**************************************************************
��ģ�����ڶ���TFT��һЩ�ض���,����ɫ,����,�͵ײ�Ӧ�ú���
***************************************************************/
#ifndef TFTHAL_H
#define TFTHAL_H


/**************************************************************
��������״̬,ֻ�ж���д����
***************************************************************/

#define SET_BUS_OUT		GPIOE->CRL=0x33333333	/*�������50MHZ�ٶ�?*/
#define SET_BUS_IN		GPIOE->CRL=0x44444444	/*��������*/

#define ST7781			1

//TFT��ʾ������
#define TFT_TYPE	1		//1��һ��,2�Ƕ�����

//����������
#define TOUCH_TYPE	2		//1�Ǿɰ�,2���°�
#ifdef TFTHAL_c
void wr_cmd(u16 cmd);
void wr_data(u16 dat);
void lcd_initialize(void);
void SetBusWrite(void);
void SetBusRead(void);

#else
extern void TFT_Config(void);
extern void wr_cmd(u16 cmd);
extern void wr_data(u16 dat);
extern void SetBusWrite(void);
extern void SetBusRead(void);
extern void lcd_initialize(void);


#endif

#endif