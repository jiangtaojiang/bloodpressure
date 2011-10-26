/****************************************
**usr.h
**�û�����Ҫʹ�õĺ�������һ���ڴ˶��壬��������ĳЩӲ����API����
**����H�ļ���Ӳ�����־����޹�
****************************************/
#ifndef USR_H
#define USR_H
#include "stm32f10x.h"

//ʱ��ṹ��
typedef struct _t_sct{
    u8 year;	//	0-99
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 sec;
    u8 date;
}T_STRUCT;

//û������ʱ���µ�Ĭ��ʱ��
#define DEFAULT_TIME_YEAR	9
#define DEFAULT_TIME_MONTH	11
#define DEFAULT_TIME_DAY	17
#define DEFAULT_TIME_HOUR	17
#define DEFAULT_TIME_MINUTE	19
#define DEFAULT_TIME_SECOND	30

//����ʱ��
#define BKP_TIME_SET		BKP_DR1	//RTC��������־
#define BKP_TIME_YEAR		BKP_DR2
#define BKP_TIME_MONTH		BKP_DR3
#define BKP_TIME_DAY		BKP_DR4
#define BKP_TIME_DATE		BKP_DR5

extern u16 BG_COLOR;	//ȫ�ֵı���ɫ,�˱�����Ϊ�����������ٶȶ����õ�
//RTC
extern void GetRTCTime(T_STRUCT*);
extern void SetRTCTime(T_STRUCT*);
extern u8 GetDate(T_STRUCT* time);

/**************************************************************
�û�API����
***************************************************************/
//SET_TIME
//extern T_STRUCT Set_Time;

//U16��ֵ�˲�
extern u16 DigitFilter(u16* buf,u8 no);
//����GUI�ϵ�����ת�ַ�������
extern u8 NumToString(s32 num,char* str);




/**************************************************************
	�û����ܺ���
***************************************************************/
//����ʱ������ʱ��ֵ
extern void SetRTCBar(T_STRUCT* time);
//ˢ�¿ؼ���ʾ��ʱ��
extern void UpdateRTCBar(void);
//����ͼ�������
extern void SetIcon(u8);
//����ADC������
extern void UpdateAdc(void);
//����U��״̬
extern void UpdateUDisk(void);

/**************************************************************
	�ؼ���Ӧ����
***************************************************************/
extern void RspButtonLed(u8 param);

#endif
