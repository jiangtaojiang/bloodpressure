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
* 本文件实现基于RTC的日期功能，提供年月日的读写。（基于ANSI-C的time.h）

* RTC中保存的时间格式，是UNIX时间戳格式的。即一个32bit的time_t变量（实为u32）
*
* ANSI-C的标准库中，提供了两种表示时间的数据  型：
* time_t:   	UNIX时间戳（从1970-1-1起到某时间经过的秒数）
* 	typedef unsigned int time_t;
* 
* struct tm:	Calendar格式（年月日形式）
*   tm结构如下：
*   struct tm {
*   	int tm_sec;   // 秒 seconds after the minute, 0 to 60
*   					 (0 - 60 allows for the occasional leap second)
*   	int tm_min;   // 分 minutes after the hour, 0 to 59
*		int tm_hour;  // 时 hours since midnight, 0 to 23
*		int tm_mday;  // 日 day of the month, 1 to 31
*		int tm_mon;   // 月 months since January, 0 to 11
*		int tm_year;  // 年 years since 1900
*		int tm_wday;  // 星期 days since Sunday, 0 to 6
*		int tm_yday;  // 从元旦起的天数 days since January 1, 0 to 365
* 		int tm_isdst; // 夏令时？？Daylight Savings Time flag
* 		...
* 	}
* 	其中wday，yday可以自动产生，软件直接读取
* 	mon的取值为0-11
*	***注意***：
*	tm_year:在time.h库中定义为1900年起的年份，即2008年应表示为2008-1900=108
* 	这种表示方法对用户来说不是十分友好，与现实有较大差异。
* 	所以在本文件中，屏蔽了这种差异。
* 	即外部调用本文件的函数时，tm结构体类型的日期，tm_year即为2008
* 	注意：若要调用系统库time.c中的函数，需要自行将tm_year-=1900
* 
* 成员函数说明：
* struct tm Time_ConvUnixToCalendar(time_t t);
* 	输入一个Unix时间戳（time_t），返回Calendar格式日期
* time_t Time_ConvCalendarToUnix(struct tm t);
* 	输入一个Calendar格式日期，返回Unix时间戳（time_t）
* time_t Time_GetUnixTime(void);
* 	从RTC取当前时间的Unix时间戳值
* struct tm Time_GetCalendarTime(void);
* 	从RTC取当前时间的日历时间
* void Time_SetUnixTime(time_t);
* 	输入UNIX时间戳格式时间，设置为当前RTC时间
* void Time_SetCalendarTime(struct tm t);
* 	输入Calendar格式时间，设置为当前RTC时间
* 
* 外部调用实例：
* 定义一个Calendar格式的日期变量：
* struct tm now;
* now.tm_year = 2008;
* now.tm_mon = 11;		//12月
* now.tm_mday = 20;
* now.tm_hour = 20;
* now.tm_min = 12;
* now.tm_sec = 30;
* 
* 获取当前日期时间：
* tm_now = Time_GetCalendarTime();
* 然后可以直接读tm_now.tm_wday获取星期数
* 
* 设置时间：
* Step1. tm_now.xxx = xxxxxxxxx;
* Step2. Time_SetCalendarTime(tm_now);
* 
* 计算两个时间的差
* struct tm t1,t2;
* t1_t = Time_ConvCalendarToUnix(t1);
* t2_t = Time_ConvCalendarToUnix(t2);
* dt = t1_t - t2_t;
* dt就是两个时间差的秒数
* dt_tm = mktime(dt);	//注意dt的年份匹配，ansi库中函数为相对年份，注意超限
* 另可以参考相关资料，调用ansi-c库的格式化输出等功能，ctime，strftime等
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
* Description    : 转换UNIX时间戳为日历时间
* Input 		 : u32 t  当前时间的UNIX时间戳
* Output		 : None
* Return		 : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtime转换结果的tm_year是相对值，需要转成绝对值
	return *t_tm;
}

/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : 写入RTC时钟当前时间
* Input 		 : struct tm t
* Output		 : None
* Return		 : time_t
*******************************************************************************/
time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  //外部tm结构体存储的年份为2008格式
						//而time.h中定义的年份格式为1900年开始的年份
						//所以，在日期转换时要考虑到这个因素。
	return mktime(&t);
}

/*******************************************************************************
* Function Name  : Time_GetUnixTime()
* Description    : 从RTC取当前时间的Unix时间戳值
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
* Description    : 从RTC取当前时间的日历时间（struct tm）
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
* Description    : 输入UNIX时间辍，转化为日历格式时间
* Input 		 : None
* Output		 : None
* Return		 : time_t t
*******************************************************************************/
struct tm TimeConv_LifeScanData(time_t t_t) //输入UNIX时间辍，转化为日历格式时间
{
	struct tm t_tm;

	t_tm = Time_ConvUnixToCalendar(t_t);
	return t_tm;
}
/*******************************************************************************
* Function Name  : Time_SetUnixTime()
* Description    : 将给定的Unix时间戳写入RTC
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
* Description    : 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
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
**函数名:GetRTCTime
**功能:获取实际时间
**注意事项：获取值将放进参数指针内，注意获取前需要关闭RTC中断，则可以避免秒越界的问题
**
*******************************/
static T_STRUCT Real_Time;//实时时间值,只允许本模块调用,外部获取时间一律使用GetTime函数
void GetRTCTime(T_STRUCT* time)
{
    struct tm Rtime;
//    int ttime[30];
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//关闭秒中断
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
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //打开秒中断
#ifdef DEBUG   
//	printf("%d年%d月%d日%d:%d:%d \r\n",time->year,time->month,time->day,time->hour,time->minute,time->sec);
#endif
}
/***********************************
**函数名:GetDate
**功能:计算星期
**注意事项:白菜星期算法,这个一般用在GPS获取日期后计算出星期.
************************************/
const u8 TAB_DATE[12]={6,2,2,5,0,3,5,1,4,6,2,4,};
u8 GetDate(T_STRUCT* time)
{
	return( (time->year + time->year/4 - ( (time->month<3)&&(time->year%4==0) ) + TAB_DATE[time->month-1] + time->day )%7);
}
/*********************************
**函数名:SetRTCTime
**功能:设置时间，除了把Real_Time的值改变外,还要把时分秒转换为RTC计数值,年月日存到后备寄存器上
**注意事项:函数内会自动根据年月日计算星期,并且返回到*time上
**********************************/
void SetRTCTime(T_STRUCT* time)
{
	u32 count;
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//关闭秒中断

	RTC_WaitForLastTask();
	//付时间值到Real_Time上
	Real_Time.year=time->year;
	Real_Time.month=time->month;
	Real_Time.day=time->day;
	Real_Time.hour=time->hour;
	Real_Time.minute=time->minute;
	Real_Time.sec=time->sec;
	//计算星期
	time->date=Real_Time.date=GetDate(time);

	//把新的年月日存到掉电寄存器上

	BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
//	RTC_WaitForLastTask();

	//计算新的RTC count值
	count=Real_Time.hour*3600+Real_Time.minute*60+Real_Time.sec;
	RTC_WaitForLastTask();
	RTC_SetCounter(count);
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //打开秒中断
}
/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : 来重新配置RTC和BKP，仅在检测到后备寄存器数据丢失时使用
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
	//启用PWR和BKP的时钟（from APB1）
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	//后备域解锁
	PWR_BackupAccessCmd(ENABLE);

	//备份寄存器模块复位
	BKP_DeInit();

	//外部32.768K其哟偶那个
	RCC_LSEConfig(RCC_LSE_ON);
	//等待稳定
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	//RTC时钟源配置成LSE（外部32.768K）
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	//RTC开启
	RCC_RTCCLKCmd(ENABLE);

	//开启后需要等待APB1时钟与RTC时钟同步，才能读写寄存器
	RTC_WaitForSynchro();

	//读写寄存器前，要确定上一个操作已经结束
	RTC_WaitForLastTask();

	//设置RTC分频器，使RTC时钟为1Hz
	//RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
	RTC_SetPrescaler(32767);


	//等待寄存器写入完成
	RTC_WaitForLastTask();

	//使能秒中断
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
    //Enable the RTC Alarm interrupt 
//  RTC_ITConfig(RTC_IT_ALR, ENABLE);

	//等待写入完成
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
	/*后备寄存器1中，存了一个特殊字符0xA5A5
	第一次上电或后备电源掉电后，该寄存器数据丢失，
	表明RTC数据丢失，需要重新配置 */
    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) //检查是否第一次上电或后备电池已经掉电，
    {       
        Write_Log("Backup VBAT PowerDown or First time PowerUp,Initialize RTC\r\n");
        RTC_Configuration();
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
        
        time_now.tm_year = 2011;
        time_now.tm_mon = 10; //月份表示为0~11
        time_now.tm_mday = 13;
        time_now.tm_hour = 13;
        time_now.tm_min = 16;
        time_now.tm_sec = 38;
        Time_SetCalendarTime(time_now);//设置初始时间
    } 
    else //若后备寄存器没有掉电，则无需重新配置RTC
    {
        Write_Log("Backup VBAT Keep, Don't RTC Configuralation\r\n");
                    //等待RTC与APB同步
  		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	
  		//使能秒中断 
  		RTC_ITConfig(RTC_IT_SEC, ENABLE);  
  		RTC_WaitForLastTask();
    }
      //这里我们可以利用RCC_GetFlagStatus()函数查看本次复位类型
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
    else if(PWR_GetFlagStatus(PWR_FLAG_WU)!= RESET)  //wakeup唤醒
    {
        Write_Log("WakeUp...\r\n");     
    }
    if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET) //检查是否由待机模式下唤醒，如是则不需要配置RTC
        /* System resumed from STANDBY mode */      
         /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);

        //清除RCC中复位标志
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