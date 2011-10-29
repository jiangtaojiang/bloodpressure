#ifndef __EVT_H__
#define __EVT_H__

//-------------------------------------------------------------
extern volatile unsigned long msTicks;
#define LINE_LENGTH_MASK 63
extern char line[64];

#define RE_SEND_TIME (60)		//RTC唤醒间隔 单位:秒
#define LINK_WAIT_TIME 60		//按键唤醒后，等待连接时间 单位:秒
#define WAIT_FOR_LINK (40)		//开机后等待时间 单位:秒

#define NO_NEED_DISC 1
#define NEED_DISC 2
//-------------------------------------------------------------

#define soft_reset() *((u32*)0xE000ED0C)=0x05fa0004


extern volatile unsigned long evt_state;
//extern volatile unsigned char send_flag;

#define _BV(x) (1<<(x))
								//调试事件  
								//usart3收到一行数据后 evt_set(DBG_EVT)
								//主循环evt_chk(DBG_EVT)
								//如果有事件 evt_clr(DBG_EVT) 完成deg()执行一次
#define DBG_EVT 		(1<<0)		

								//tim2读取血压数据timer到了 就 evt_set
								//主循环evt_chk 后
								//若有事件 evt_clr, 执行cm_down() 
#define CM_DOWN_EVT	 	(1<<1)	//下载采集模块事件

#define BLP_SND_ENA		(1<<3)	//允许发送数据包
#define BUSI_ACK		(1<<4)	//需要发送业务ACK


#define DISC_FLAG		(1<<5)	//数据发送完标志，数据发送完数据包 发送成功后置位 用于断线处理
#define BLP_DT_OVER		_BV(6)
#define UPDATA_FLAG		_BV(7)	//升级指令回复
#define DEV_INFO		_BV(8)	//返回设备信息

#define evt_set(x) evt_state |= x 
#define evt_clr(x) evt_state &= ~x
#define evt_chk(x) (evt_state & x)
//-------------------------------------------------------------

#define RTC_BKP BKP_DR1		//RTC配置使用
#define FLAG_BKP BKP_DR5	//按键唤醒 和 RTC闹钟唤醒 标志 寄存器
	#define RTC_WAKE_FLAG 0x55AA
	#define PA0_WAKE_FLAG 0xAA55

//血压数据包重发寄存器 存重传的时间 单位:s 
//比如 一次发送失败，5min后启动下一次发送
//再失败 10min后发送 多次失败 待机 不再理财
#define RE_SND_CNT_BKB	BKP_DR6
#define MAX_RE_SND_CNT  3

//-------------------------------------------------------------

void dbg(void);
void delay_ms(unsigned long dlyTicks);
void delay_sec(unsigned long dlys);
void delay(unsigned long t);


#endif
