#ifndef __BUSINESS_H__
#define __BUSINESS_H__
#include <stdint.h>


#define ID 0x06

#define PAGE_SIZE             (0x400)	//1k
#define FLASH_SIZE            (0xE000) //64-8K = 56K
//CRC �����ҵ���ĩβ
//ver:1.x
#define DEV_CFG   0x01		//�豸����
#define BLP_DATA  0x02		//Ѫѹ����	
#define CFG_TIME  0x03		//ͬ��ʱ��
#define RE_TIME   0x04		//������ʱ��
#define DEV_STATE 0x05		//�豸״̬
	#define S_LOW_POWER  0x01	//������
	#define S_LACK_POWER 0x02	//��������
	#define S_XLOW_POWER 0x03	//ȡ��������
	#define S_CM_ERROR   0x04	//�ɼ���ͨѶ����
	#define S_RTC_ERROR  0x05	//RTC����
#define DEV_BACK  0x06		//�豸����
#define RE_ADDR   0x07		//����������ַ
#define START     0x08		//����һ�β���
#define SND_OVER  0x09		//���ݷ������
#define PWR_LEV  0x0A		//���͵���

//v2.x
#define RENEW_CFG 	0x0b		//���¸��²�������ͱ�����ֵ
#define DEV_CFG_NEW 0x0c		//�����豸ʱ���豸��Ϣ

#define REQ_DEV_INFO	0xfa		//�豸��Ϣ
//IAP
#define UPDATA_OVER	0xfb	//IAP������ɰ�
#define UPDATA_DT	0xfd	//IAP���ݰ�
#define UPDATA_DEV_INFO	0xfe	//�����豸��Ϣ
#define UPDATA_CMD	0xff	//����ָ��
#define UPDATA_ACK	0xfc	//����Ӧ���
//#define DEV_INFO        0xfa
//v1.x
//-----------------------------------------------------------------------------
/* ���������� */
//Ѫѹ�ǲ������ð�
typedef struct
{
	uint8_t type;		//0x01
	uint8_t display;	//�����ʾ���� 1:ON 0:OFF
	uint8_t manual;		//�ֶ��������� 1:ON 0:OFF(�ֻ�flag)
	uint8_t remit;		//�ط�ʱ���� (��ͨѶʧ�ܺ��´η��͵�ʱ����) ��λ:min
	uint8_t time[7];	//ʱ��
	uint8_t mtime[7];	//��һ�β���ʱ��
	uint8_t sleep[2];	//˯��ʱ��hhmm
	uint8_t sleep_interval;	//˯��������� ��λ��min
	uint8_t awake[2];		//˯��ʱ��hhmm
	uint8_t awake_interval;	//˯�Ѳ������ ��λ��min
	uint8_t high_alert[2];
	uint8_t low_alert;	
}struct_dev_config;

//�������ú�ظ���ACK
typedef struct
{
	uint8_t type;	//0x01����Ӧ��ֵ����Ϊ0x01
	uint8_t state;	//��Ӧ��Ϣ 00���ɹ� 01��ʧ��
	uint8_t hdw_ver;//Ӳ���汾
	uint8_t soft_ver;//����汾
}struct_dev_config_ack;

//Ѫѹ����
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

//Ѫѹ����ACK
typedef struct
{
	uint8_t type;		//0x02
	uint8_t state;		//00:succ 01:fail
	uint8_t index[2];	//index
}struct_bl_data_ack;

//ʱ��ͬ��
typedef struct
{
	uint8_t type;	//0x03
	uint8_t time[7];
}struct_cfg_time;

//�豸����
typedef struct
{
	uint8_t type;	//0x06
}struct_dev_back;

//����������ַ
typedef struct
{	uint8_t type;	//0x07
	uint8_t addr[12];
}struct_re_addr;

//Ѫѹ���ݷ������
typedef struct
{
	uint8_t type;	//0x09
}struct_snd_over;

//�����ٷֱ�
typedef struct
{
	uint8_t type;	//0x0a
	uint8_t lev;	//�����ٷֱ�
}struct_pwr_lev;

//ͨ��ACK��û��ָ��ACK��ָ�����ͨ��ACK
typedef struct
{
	uint8_t type;	//0x04
	uint8_t state;	//0x00:succ 0x01:fail  0x03:INVALID
}struct_general_ack;

//v2.x
//------------------------------------------------------------------------------
#pragma pack(1)

//����ʱ��ͱ�����ֵ
typedef struct
{
	uint8_t type;	//0x0b
	uint8_t rsv;
	uint8_t slp_tm[2];	//˯��ʱ��
	uint16_t slp_itv;	//˯�߲������
	uint8_t awk_tm[2];	//˯��ʱ��
	uint16_t awk_itv;	//˯�Ѳ������
	uint16_t h_hgate;	//��ѹ��������ֵ
	uint8_t  l_hgate;	//��ѹ��������ֵ
	uint16_t h_lgate;	//��ѹ��������ֵ
	uint8_t l_lgate;	//��ѹ��������ֵ
}struct_renew_cfg;

//2.0����ʱ�����ð�
typedef struct
{
	uint8_t type;	//0x0c
	uint8_t rsv;
	uint8_t stack_flag;	//����Э��ջ  1:΢��/adroidЭ��ջ  2: ��ͨ
	uint8_t time[7];	//ʱ��ͬ��
	uint8_t slp_tm[2];	//˯��ʱ��
	uint16_t slp_itv;	//˯�߲������
	uint8_t awk_tm[2];	//˯��ʱ��
	uint16_t awk_itv;	//˯�Ѳ������
	uint16_t h_hgate;	//��ѹ��������ֵ
	uint8_t  l_hgate;	//��ѹ��������ֵ
	uint16_t h_lgate;	//��ѹ��������ֵ
	uint8_t l_lgate;	//��ѹ��������ֵ
}struct_dev_cfg_new;

//�豸��Ϣ��
typedef struct
{
	uint8_t type;			//0xfe
    uint8_t iap_ver[4];
	uint8_t hdw_ver[4];		//useless, for bootloader can't updata
	uint8_t soft_ver[4];
	uint8_t flash_size[2];	//n*1kbyte
	uint8_t page_size;		//n*128byte
}dev_info;
//�����豸��Ϣ
typedef struct{
	uint8_t type;			//0xfe
	uint16_t hdw_ver;		//Ӳ���汾
	uint16_t soft_ver;		//����汾
	uint16_t flash_size;	//flash��С
	uint8_t  page_size;		//n*128byte
}struct_dev_info;

#pragma pack()

//------------------------------------------------------------------------------
extern void (*fp_busi_ack)(u8,u8);
extern u8 busi_ack_type;
extern u8 ack_state;

void business_deal(void *ptr, uint16_t len);

void timeout_deal(uint8_t ch);

//����Ƿ�������Ҫ���� ���� ����ȡһ����¼����
void data_send(void);
uint8_t put_start_time(uint32_t t);
uint32_t get_start_time(void);

#endif
