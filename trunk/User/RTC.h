/******************************************************************************
* file Name     : RTC.h                                                                         
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#include "stm32f10x.h"
#include <time.h>
#ifdef  RTC_c

void RTC_Config(void);
void curr_time(void);

u8 por_rst_flag;
u8 pin_rst_flag;
#else
extern void RTC_Config(void);
extern struct tm Time_ConvUnixToCalendar(time_t t);
extern time_t Time_ConvCalendarToUnix(struct tm t);
extern time_t Time_GetUnixTime(void);
extern struct tm Time_GetCalendarTime(void);
extern void Time_SetUnixTime(time_t);
extern void Time_SetCalendarTime(struct tm t);
extern struct tm TimeConv_LifeScanData(time_t t_t);
extern void curr_time(void);

extern u8 por_rst_flag;
extern u8 pin_rst_flag;
#endif