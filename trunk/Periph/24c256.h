/******************************************************************************
* file Name     : 24c256.h                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chenshuiqing                                                                           
* Updata        : chenshuiqing   
* brief         : 
*******************************************************************************/
#ifndef __24C256_H 
#define __24C256_H 

/* Includes ------------------------------------------------------------------*/ 

//EEPROM memory map
//-----------------------------------------------------------------------------

#define SEND_ITEM_BYTES 		2		//发送条数
#define SAVE_ITEM_BYTES 		2		//存储条数
#define BTH_MAC_BYTES   		12		//绑定的蓝牙地址
#define UPDATA_FLAG_BYTES 		1		//升级标记
#define TIME_BYTES				4		//开始测量时基
#define DEV_BYTES				1		//设备状态0:DEV_BACK， 非0:DEV_IN_USE
#define PHONE_FLAG_BYTES		1		//手机蓝牙协议标记
#define RENEW_INFO_FLAG_BYTES	1		//需要更新参数标记
#define RENEW_INFO_BYTES		17		//更新参数的数据
#define CM_SAVE_ITEM_BYTES		2		//采集器存的血压数据条目
#define DEV_INFO_BYTES          12      //设备信息，iap、硬件、软件版本
#define BE_UPDATED_FLAG_BYTES   1       //是否升级过的标记

#define SEND_ITEM_BASE 			(0)										//2byte
#define SAVE_ITEM_BASE 			(SEND_ITEM_BYTES+SEND_ITEM_BASE)		//2byte
#define BTH_MAC_BASE   			(SAVE_ITEM_BYTES+SAVE_ITEM_BASE)		//00195d24af18 12bytebyte
#define UPDATA_FLAG_BASE 		(BTH_MAC_BYTES+BTH_MAC_BASE)			//1byte
#define TIME_BASE				(UPDATA_FLAG_BYTES+UPDATA_FLAG_BASE)	//4byte
#define DEV_FLAG				(TIME_BYTES+TIME_BASE)					//1byte  0:DEV_BACK， 非0:DEV_IN_USE
#define PHONE_FLAG_BASE  		(DEV_BYTES+DEV_FLAG) 					//1byte
#define RENEW_INFO_FLAG_BASE 	(PHONE_FLAG_BYTES+PHONE_FLAG_BASE)				//1byte
#define RENEW_INFO_BASE			(RENEW_INFO_FLAG_BYTES+RENEW_INFO_FLAG_BASE)	//17byte
#define CM_SAVE_ITEM_BASE		(RENEW_INFO_BYTES+RENEW_INFO_BASE)
#define DEV_INFO_BASE           (CM_SAVE_ITEM_BYTES+CM_SAVE_ITEM_BASE)
#define BE_UPDATED_FLAG_BASE    (DEV_INFO_BYTES+DEV_INFO_BASE)

#define DATA_BASE      		512
/*   __________________
    |                  |
    |                  |512byte 配置信息区域
    |__________________|
    |                  |
    |                  |(32k-512)byte 血压数据区域
    |                  |
    |                  |
    |                  |
    |__________________|
*/
//-----------------------------------------------------------------------------

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned short
#endif

#ifndef u32
#define u32 unsigned long
#endif

#ifndef bool
#define bool unsigned char
#endif

#define SLA_W 0xa0
#define SLA_R 0xa1
#define I2C_PageSize  32 	 //24C64每页64字节
#define PAGES         256	//24c64

#define SCL_H         GPIOB->BSRR = GPIO_Pin_6 
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6  
    
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7 
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7 

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6 
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7

#ifdef AT24c256_c
void iic_init(void); 
bool iic_read( u8* ptr, u16 addr) ;//读取1字节
bool iic_reads( u8*ptr, u16 addr, u16 len);//读取n字节
bool iic_write(u16 WriteAddress, u8 SendByte ) ;//写入1字节
bool iic_writes(u8 *ptr,u16 addr,u16 no);//写入n字节
void iic_clear(void);
#else
extern void iic_init(void); 
extern bool iic_read( u8* ptr, u16 addr) ;
extern bool iic_reads( u8*ptr, u16 addr, u16 len);
extern bool iic_write(u16 WriteAddress, u8 SendByte );
extern bool iic_writes(u8 *ptr,u16 addr,u16 no);
extern void iic_clear(void);
#endif

#endif 


