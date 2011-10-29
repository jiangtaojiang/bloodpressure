/******************************************************************************
* file Name     : RTC.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#define RTC_c
#include "usr.h"
/*******************************************************************************
* ���ļ�ʵ�ֻ���RTC�����ڹ��ܣ��ṩ�����յĶ�д��������ANSI-C��time.h��

* RTC�б����ʱ���ʽ����UNIXʱ�����ʽ�ġ���һ��32bit��time_t������ʵΪu32��
*
* ANSI-C�ı�׼���У��ṩ�����ֱ�ʾʱ�������  �ͣ�
* time_t:   	UNIXʱ�������1970-1-1��ĳʱ�侭����������
* 	typedef unsigned int time_t;
* 
* struct tm:	Calendar��ʽ����������ʽ��
*   tm�ṹ���£�
*   struct tm {
*   	int tm_sec;   // �� seconds after the minute, 0 to 60
*   					 (0 - 60 allows for the occasional leap second)
*   	int tm_min;   // �� minutes after the hour, 0 to 59
*		int tm_hour;  // ʱ hours since midnight, 0 to 23
*		int tm_mday;  // �� day of the month, 1 to 31
*		int tm_mon;   // �� months since January, 0 to 11
*		int tm_year;  // �� years since 1900
*		int tm_wday;  // ���� days since Sunday, 0 to 6
*		int tm_yday;  // ��Ԫ��������� days since January 1, 0 to 365
* 		int tm_isdst; // ����ʱ����Daylight Savings Time flag
* 		...
* 	}
* 	����wday��yday�����Զ����������ֱ�Ӷ�ȡ
* 	mon��ȡֵΪ0-11
*	***ע��***��
*	tm_year:��time.h���ж���Ϊ1900�������ݣ���2008��Ӧ��ʾΪ2008-1900=108
* 	���ֱ�ʾ�������û���˵����ʮ���Ѻã�����ʵ�нϴ���졣
* 	�����ڱ��ļ��У����������ֲ��졣
* 	���ⲿ���ñ��ļ��ĺ���ʱ��tm�ṹ�����͵����ڣ�tm_year��Ϊ2008
* 	ע�⣺��Ҫ����ϵͳ��time.c�еĺ�������Ҫ���н�tm_year-=1900
* 
* ��Ա����˵����
* struct tm Time_ConvUnixToCalendar(time_t t);
* 	����һ��Unixʱ�����time_t��������Calendar��ʽ����
* time_t Time_ConvCalendarToUnix(struct tm t);
* 	����һ��Calendar��ʽ���ڣ�����Unixʱ�����time_t��
* time_t Time_GetUnixTime(void);
* 	��RTCȡ��ǰʱ���Unixʱ���ֵ
* struct tm Time_GetCalendarTime(void);
* 	��RTCȡ��ǰʱ�������ʱ��
* void Time_SetUnixTime(time_t);
* 	����UNIXʱ�����ʽʱ�䣬����Ϊ��ǰRTCʱ��
* void Time_SetCalendarTime(struct tm t);
* 	����Calendar��ʽʱ�䣬����Ϊ��ǰRTCʱ��
* 
* �ⲿ����ʵ����
* ����һ��Calendar��ʽ�����ڱ�����
* struct tm now;
* now.tm_year = 2008;
* now.tm_mon = 11;		//12��
* now.tm_mday = 20;
* now.tm_hour = 20;
* now.tm_min = 12;
* now.tm_sec = 30;
* 
* ��ȡ��ǰ����ʱ�䣺
* tm_now = Time_GetCalendarTime();
* Ȼ�����ֱ�Ӷ�tm_now.tm_wday��ȡ������
* 
* ����ʱ�䣺
* Step1. tm_now.xxx = xxxxxxxxx;
* Step2. Time_SetCalendarTime(tm_now);
* 
* ��������ʱ��Ĳ�
* struct tm t1,t2;
* t1_t = Time_ConvCalendarToUnix(t1);
* t2_t = Time_ConvCalendarToUnix(t2);
* dt = t1_t - t2_t;
* dt��������ʱ��������
* dt_tm = mktime(dt);	//ע��dt�����ƥ�䣬ansi���к���Ϊ�����ݣ�ע�ⳬ��
* ����Բο�������ϣ�����ansi-c��ĸ�ʽ������ȹ��ܣ�ctime��strftime��
* 
*******************************************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "RTC.h"
#include "stm32f10x_conf.h"
#include "main.h"
#include "RCC_config.h"
#include "usart.h"

/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar(time_t t)
* Description    : ת��UNIXʱ���Ϊ����ʱ��
* Input 		 : u32 t  ��ǰʱ���UNIXʱ���
* Output		 : None
* Return		 : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtimeת�������tm_year�����ֵ����Ҫת�ɾ���ֵ
	return *t_tm;
}

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : д��RTCʱ�ӵ�ǰʱ��
* Input 		 : struct tm t
* Output		 : None
* Return		 : time_t
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  //�ⲿtm�ṹ��洢�����Ϊ2008��ʽ
						//��time.h�ж������ݸ�ʽΪ1900�꿪ʼ�����
						//���ԣ�������ת��ʱҪ���ǵ�������ء�
	return mktime(&t);
}

/*******************************************************************************
* Function Name  : Time_GetUnixTime()
* Description    : ��RTCȡ��ǰʱ���Unixʱ���ֵ
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
time_t Time_GetUnixTime(void)
{
	return (time_t)RTC_GetCounter();
}

/*******************************************************************************
* Function Name  : Time_GetCalendarTime()
* Description    : ��RTCȡ��ǰʱ�������ʱ�䣨struct tm��
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
struct tm Time_GetCalendarTime(void)
{
	time_t t_t;
	struct tm t_tm;

	t_t = (time_t)RTC_GetCounter();
	t_tm = Time_ConvUnixToCalendar(t_t); 
	return t_tm;
}
/*******************************************************************************
* Function Name  : TimeConv_LifeScanData()
* Description    : ����UNIXʱ��ꡣ�ת��Ϊ������ʽʱ��
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
struct tm TimeConv_LifeScanData(time_t t_t) //����UNIXʱ��ꡣ�ת��Ϊ������ʽʱ��
{
	struct tm t_tm;

	t_tm = Time_ConvUnixToCalendar(t_t);
	return t_tm;
}
/*******************************************************************************
* Function Name  : Time_SetUnixTime()
* Description    : ��������Unixʱ���д��RTC
* Input 		 : time_t t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RTC_WaitForLastTask();
        RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime()
* Description    : ��������Calendar��ʽʱ��ת����UNIXʱ���д��RTC
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}
/******************************
**������:GetRTCTime
**����:��ȡʵ��ʱ��
**ע�������ȡֵ���Ž�����ָ���ڣ�ע���ȡǰ��Ҫ�ر�RTC�жϣ�����Ա�����Խ�������
**
*******************************/
static T_STRUCT Real_Time;//ʵʱʱ��ֵ,ֻ����ģ�����,�ⲿ��ȡʱ��һ��ʹ��GetTime����
void GetRTCTime(T_STRUCT* time)
{
    struct tm Rtime;
//    int ttime[30];
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//�ر����ж�
	RTC_WaitForLastTask();
    Rtime = Time_GetCalendarTime();
    
	time->year = Rtime.tm_year%100;
	time->month = Rtime.tm_mon+1;
	time->day = Rtime.tm_mday;
	time->hour = Rtime.tm_hour;
	time->minute = Rtime.tm_min;
	time->sec = Rtime.tm_sec;
	time->date = Rtime.tm_wday;

	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //�����ж�
#ifdef DEBUG   
//	printf("%d��%d��%d��%d:%d:%d \r\n",time->year,time->month,time->day,time->hour,time->minute,time->sec);
#endif
}
/***********************************
**������:GetDate
**����:��������
**ע������:�ײ������㷨,���һ������GPS��ȡ���ں���������.
************************************/
const u8 TAB_DATE[12]={6,2,2,5,0,3,5,1,4,6,2,4,};
u8 GetDate(T_STRUCT* time)
{
	return( (time->year + time->year/4 - ( (time->month<3)&&(time->year%4==0) ) + TAB_DATE[time->month-1] + time->day )%7);
}
/*********************************
**������:SetRTCTime
**����:����ʱ�䣬���˰�Real_Time��ֵ�ı���,��Ҫ��ʱ����ת��ΪRTC����ֵ,�����մ浽�󱸼Ĵ�����
**ע������:�����ڻ��Զ����������ռ�������,���ҷ��ص�*time��
**********************************/
void SetRTCTime(T_STRUCT* time)
{
	u32 count;
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//�ر����ж�

	RTC_WaitForLastTask();
	//��ʱ��ֵ��Real_Time��
	Real_Time.year=time->year;
	Real_Time.month=time->month;
	Real_Time.day=time->day;
	Real_Time.hour=time->hour;
	Real_Time.minute=time->minute;
	Real_Time.sec=time->sec;
	//��������
	time->date=Real_Time.date=GetDate(time);

	//���µ������մ浽����Ĵ�����

	BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
//	RTC_WaitForLastTask();

	//�����µ�RTC countֵ
	count=Real_Time.hour*3600+Real_Time.minute*60+Real_Time.sec;
	RTC_WaitForLastTask();
	RTC_SetCounter(count);
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //�����ж�
}
/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : ����������RTC��BKP�����ڼ�⵽�󱸼Ĵ������ݶ�ʧʱʹ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
	//����PWR��BKP��ʱ�ӣ�from APB1��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	//�������
	PWR_BackupAccessCmd(ENABLE);

	//���ݼĴ���ģ�鸴λ
	BKP_DeInit();

	//�ⲿ32.768K��Ӵż�Ǹ�
	RCC_LSEConfig(RCC_LSE_ON);
	//�ȴ��ȶ�
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	//RTCʱ��Դ���ó�LSE���ⲿ32.768K��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	//RTC����
	RCC_RTCCLKCmd(ENABLE);

	//��������Ҫ�ȴ�APB1ʱ����RTCʱ��ͬ�������ܶ�д�Ĵ���
	RTC_WaitForSynchro();

	//��д�Ĵ���ǰ��Ҫȷ����һ�������Ѿ�����
	RTC_WaitForLastTask();

	//����RTC��Ƶ����ʹRTCʱ��Ϊ1Hz
	//RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_SetPrescaler(32767);


	//�ȴ��Ĵ���д�����
	RTC_WaitForLastTask();

	//ʹ�����ж�
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
    //Enable the RTC Alarm interrupt 
//  RTC_ITConfig(RTC_IT_ALR, ENABLE);

	//�ȴ�д�����
	RTC_WaitForLastTask();

	return;
}
/*******************************************************************************
* Function Name  : RTC_Config
* Description    : RTC_Config
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
struct tm time_now;
void RTC_Config(void)	
{
	/*�󱸼Ĵ���1�У�����һ�������ַ�0xA5A5
	��һ���ϵ��󱸵�Դ����󣬸üĴ������ݶ�ʧ��
	����RTC���ݶ�ʧ����Ҫ�������� */
    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) //����Ƿ��һ���ϵ��󱸵���Ѿ����磬
    {       
        Write_Log("Backup VBAT PowerDown or First time PowerUp,Initialize RTC\r\n");
        RTC_Configuration();
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
        
        time_now.tm_year = 2011;
        time_now.tm_mon = 10; //�·ݱ�ʾΪ0~11
        time_now.tm_mday = 13;
        time_now.tm_hour = 13;
        time_now.tm_min = 16;
        time_now.tm_sec = 38;
        Time_SetCalendarTime(time_now);//���ó�ʼʱ��
    } 
    else //���󱸼Ĵ���û�е��磬��������������RTC
    {
        Write_Log("Backup VBAT Keep, Don't RTC Configuralation\r\n");
                    //�ȴ�RTC��APBͬ��
  		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	
  		//ʹ�����ж� 
  		RTC_ITConfig(RTC_IT_SEC, ENABLE);  
  		RTC_WaitForLastTask();
    }
      //�������ǿ�������RCC_GetFlagStatus()�����鿴���θ�λ����
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
		por_rst_flag = 1;	
        Write_Log("PowerUp Reset\r\n");
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
		pin_rst_flag = 1;
        Write_Log("pin Reset\r\n");
    }
    else if(PWR_GetFlagStatus(PWR_FLAG_WU)!= RESET)  //wakeup����
    {
        Write_Log("WakeUp...\r\n");     
    }
    if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET) //����Ƿ��ɴ���ģʽ�»��ѣ���������Ҫ����RTC
        /* System resumed from STANDBY mode */      
         /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);

        //���RCC�и�λ��־
    RCC_ClearFlag();
	return;
}
/*******************************************************************************
* Function Name  : curr_time
* Description    : curr_time
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void curr_time(void)
{
	uint32_t curr_time = 0;
	uint8_t ts[30] = {0};			
	curr_time += (uint32_t)(RTC->CNTH)<<16;
	curr_time +=  RTC->CNTL;
	//strftime((char *)ts,30,"%Y-%m-%d  %H:%M:%S  :",localtime((time_t *)&curr_time));
	strftime((char *)ts,30,"%H:%M:%S",localtime((time_t *)&curr_time));
	printf("\r\n%s  ",ts); 	
}