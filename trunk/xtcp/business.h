#ifndef __BUSINESS_H__
#define __BUSINESS_H__
#include <stdint.h>


#define ID 0x06

#define PAGE_SIZE             (0x400)	//1k
#define FLASH_SIZE            (0xE000) //64-8K = 56K
//CRC 添加在业务包末尾
//ver:1.x
#define DEV_CFG   0x01		//设备配置
#define BLP_DATA  0x02		//血压数据	
#define CFG_TIME  0x03		//同步时间
#define RE_TIME   0x04		//重配置时间
#define DEV_STATE 0x05		//设备状态
	#define S_LOW_POWER  0x01	//电量低
	#define S_LACK_POWER 0x02	//电量不足
	#define S_XLOW_POWER 0x03	//取消电量低
	#define S_CM_ERROR   0x04	//采集器通讯错误
	#define S_RTC_ERROR  0x05	//RTC错误
#define DEV_BACK  0x06		//设备回收
#define RE_ADDR   0x07		//重设蓝牙地址
#define START     0x08		//启动一次测量
#define SND_OVER  0x09		//数据发送完成
#define PWR_LEV  0x0A		//发送电量

//v2.x
#define RENEW_CFG 	0x0b		//重新更新测量间隔和报警阈值
#define DEV_CFG_NEW 0x0c		//发放设备时的设备信息

#define REQ_DEV_INFO	0xfa		//设备信息
//IAP
#define UPDATA_OVER	0xfb	//IAP下载完成包
#define UPDATA_DT	0xfd	//IAP数据包
#define UPDATA_DEV_INFO	0xfe	//升级设备信息
#define UPDATA_CMD	0xff	//升级指令
#define UPDATA_ACK	0xfc	//升级应答包
//#define DEV_INFO        0xfa
//v1.x
//-----------------------------------------------------------------------------
/* 蓝牙端数包 */
//血压仪参数配置包
typedef struct
{
	uint8_t type;		//0x01
	uint8_t display;	//结果显示开关 1:ON 0:OFF
	uint8_t manual;		//手动测量开关 1:ON 0:OFF(手机flag)
	uint8_t remit;		//重发时间间隔 (当通讯失败后，下次发送的时间间隔) 单位:min
	uint8_t time[7];	//时间
	uint8_t mtime[7];	//第一次测量时间
	uint8_t sleep[2];	//睡觉时间hhmm
	uint8_t sleep_interval;	//睡觉测量间隔 单位：min
	uint8_t awake[2];		//睡醒时间hhmm
	uint8_t awake_interval;	//睡醒测量间隔 单位：min
	uint8_t high_alert[2];
	uint8_t low_alert;	
}struct_dev_config;

//发放配置后回复的ACK
typedef struct
{
	uint8_t type;	//0x01包的应答，值必须为0x01
	uint8_t state;	//响应信息 00：成功 01：失败
	uint8_t hdw_ver;//硬件版本
	uint8_t soft_ver;//软件版本
}struct_dev_config_ack;

//血压数据
#pragma pack(1)
typedef struct
{
	uint8_t  type;	 //0x02
	uint16_t index;
	uint16_t high;
	uint8_t low;
	uint16_t avr;
	uint8_t rate;
	uint8_t time[7];
}struct_bl_data;
#pragma pack()

//血压数据ACK
typedef struct
{
	uint8_t type;		//0x02
	uint8_t state;		//00:succ 01:fail
	uint8_t index[2];	//index
}struct_bl_data_ack;

//时间同步
typedef struct
{
	uint8_t type;	//0x03
	uint8_t time[7];
}struct_cfg_time;

//设备回收
typedef struct
{
	uint8_t type;	//0x06
}struct_dev_back;

//设置蓝牙地址
typedef struct
{	uint8_t type;	//0x07
	uint8_t addr[12];
}struct_re_addr;

//血压数据发送完成
typedef struct
{
	uint8_t type;	//0x09
}struct_snd_over;

//电量百分比
typedef struct
{
	uint8_t type;	//0x0a
	uint8_t lev;	//电量百分比
}struct_pwr_lev;

//通用ACK，没有指定ACK的指令采用通用ACK
typedef struct
{
	uint8_t type;	//0x04
	uint8_t state;	//0x00:succ 0x01:fail  0x03:INVALID
}struct_general_ack;

//v2.x
//------------------------------------------------------------------------------
#pragma pack(1)

//重设时间和报警阈值
typedef struct
{
	uint8_t type;	//0x0b
	uint8_t rsv;
	uint8_t slp_tm[2];	//睡眠时间
	uint16_t slp_itv;	//睡眠测量间隔
	uint8_t awk_tm[2];	//睡醒时间
	uint16_t awk_itv;	//睡醒测量间隔
	uint16_t h_hgate;	//高压报警高阈值
	uint8_t  l_hgate;	//低压报警高阈值
	uint16_t h_lgate;	//高压报警低阈值
	uint8_t l_lgate;	//低压报警低阈值
}struct_renew_cfg;

//2.0发放时的配置包
typedef struct
{
	uint8_t type;	//0x0c
	uint8_t rsv;
	uint8_t stack_flag;	//蓝牙协议栈  1:微软/adroid协议栈  2: 博通
	uint8_t time[7];	//时间同步
	uint8_t slp_tm[2];	//睡眠时间
	uint16_t slp_itv;	//睡眠测量间隔
	uint8_t awk_tm[2];	//睡醒时间
	uint16_t awk_itv;	//睡醒测量间隔
	uint16_t h_hgate;	//高压报警高阈值
	uint8_t  l_hgate;	//低压报警高阈值
	uint16_t h_lgate;	//高压报警低阈值
	uint8_t l_lgate;	//低压报警低阈值
}struct_dev_cfg_new;

//设备信息，
typedef struct
{
	uint8_t type;			//0xfe
    uint8_t iap_ver[4];
	uint8_t hdw_ver[4];		//useless, for bootloader can't updata
	uint8_t soft_ver[4];
	uint8_t flash_size[2];	//n*1kbyte
	uint8_t page_size;		//n*128byte
}dev_info;
//请求设备信息
typedef struct{
	uint8_t type;			//0xfe
	uint16_t hdw_ver;		//硬件版本
	uint16_t soft_ver;		//软件版本
	uint16_t flash_size;	//flash大小
	uint8_t  page_size;		//n*128byte
}struct_dev_info;

#pragma pack()

//------------------------------------------------------------------------------
extern void (*fp_busi_ack)(u8,u8);
extern u8 busi_ack_type;
extern u8 ack_state;

void business_deal(void *ptr, uint16_t len);

void timeout_deal(uint8_t ch);

//检查是否有数据要发送 若有 则提取一条记录发送
void data_send(void);
uint8_t put_start_time(uint32_t t);
uint32_t get_start_time(void);

#endif
