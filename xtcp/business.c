//#include <stm32f10x_lib.h>
#include "stm32f10x.h"
#include <time.h>
#include "business.h"
#include "usart.h"
#include "buff.h"
#include "xtcp.h"
#include "rtc.h"
//#include "cm.h"
#include "24c256.h"
#include "common.h"
#include "bt.h"
#include <time.h>

long kernel_mktime(struct tm * tm);

void (*fp_busi_ack)(u8,u8);
const u8 ver[12] = {0x01,0x00,0x04,0x01,0x01,0x01,0x00,0x00,0x01,0x03,0x02,0x03};
//ͨ��ACK�Ĳ���
u8 busi_ack_type; 	//ACK�������
u8 ack_state;		//ack״̬

//�յ�ʱ��ͬ���������ڸ��±���ʱ��
void Updata_Time(void)
{	uint8_t *ptr = pk_buf.business;
	struct tm tm_temp;
	int temp;
	
	//year
	temp =  (ptr[1]>>4)*1000 + (ptr[1]&0x0f)*100 + (ptr[2]>>4)*10 + (ptr[2]&0x0f) - 1900;
	tm_temp.tm_year = temp;
	//month
	temp = (ptr[3]>>4)*10 + (ptr[3]&0x0f) - 1;//ϵͳ�·�Ϊ0~11
	if(temp > 11) goto UpdataTimeError;
	tm_temp.tm_mon = temp;
	//day
	temp = (ptr[4]>>4)*10 + (ptr[4]&0x0f);
	if(temp > 31) goto UpdataTimeError;//from 1 to x
	tm_temp.tm_mday = temp;
	//hour
	temp = (ptr[5]>>4)*10 + (ptr[5]&0x0f)+1;
	if(temp > 24) goto UpdataTimeError;
	tm_temp.tm_hour = temp;
	//minitue
	temp = (ptr[6]>>4)*10 + (ptr[6]&0x0f);
	if(temp >59) goto UpdataTimeError;
	tm_temp.tm_min = temp;
	//second
	temp = (ptr[7]>>4)*10 + (ptr[7]&0x0f);
	if(temp >59) goto UpdataTimeError;
	tm_temp.tm_sec = temp;
	Time_Config(&tm_temp);
	_log("Date Updated");
	return;
UpdataTimeError:
	_log("Invalid Pk, Updata time Error");	
}
//-----------------------------------------------------------------------------
/*******************************************************************************
* Function Name  : send_dev_info
* Description    : �豸��Ϣ 
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void send_dev_info(u8 type, u8 state)
{
    dev_info *info = (dev_info *)pBUSI;
#ifdef DEBUG
	printf("\r\n Dev info\r\n");
#endif
    state = state;
    u32 vap;
    u32 *ap = (u32*)0x00000200;
    vap = *ap;
	info->type = UPDATA_DEV_INFO;
    info->iap_ver[0] = vap >> 24;
    info->iap_ver[1] = vap >> 16;
    info->iap_ver[2] = vap >> 8;
    info->iap_ver[3] = vap & 0xff;
	info->hdw_ver[0] = ver[7]; //read_ver
	info->hdw_ver[1] = ver[6];
	info->hdw_ver[2] = ver[5];
    info->hdw_ver[3] = ver[4];
	info->soft_ver[0] = ver[11];
	info->soft_ver[1] = ver[10];
	info->soft_ver[2] = ver[9];
    info->soft_ver[3] = ver[8];
	info->flash_size[0] = (uint8_t)(FLASH_SIZE / 1024);   //n kbyte
	info->flash_size[1] = (uint8_t)(FLASH_SIZE /1024 / 256); 
	info->page_size = (uint8_t)(PAGE_SIZE / 128);		  //n *128byte
	xtcp_out(0,sizeof(dev_info));
}
//�����豸��Ϣ
void req_dev_info(u8 type, u8 state)
{
	struct_dev_info *busi = (struct_dev_info *)pBUSI;
	state = state;
	busi->type       = REQ_DEV_INFO;
//	busi->hdw_ver    = HDW_VER;
//	busi->soft_ver   = SOFT_VER;
	busi->flash_size = (64-8);	//56kbyte
	busi->page_size  = 8;		//8*128byte
	xtcp_out(0,sizeof(struct_dev_info));
}
//�������òɼ���ACK
void busi_cfgdev_ack(u8 type,u8 state)
{
	struct_dev_config_ack *busi = (struct_dev_config_ack *)pBUSI;
	state = state;
	busi->type = type;
	busi->state=0;	//succ
//	busi->hdw_ver = HDW_VER;
//	busi->soft_ver = SOFT_VER;
	xtcp_out(0,sizeof(struct_dev_config_ack));
}

//ͨ��ACK
void busi_general_ack(u8 type,u8 state)
{
	struct_general_ack *busi = (struct_general_ack *)pBUSI;
	
	busi->type = type;
	busi->state= state;	//succ
	xtcp_out(0,sizeof(struct_general_ack));
}

//����ACK
void updata_ack(u8 type, u8 state)
{
	uint8_t *busi = (uint8_t *)pBUSI;
	state = state;
	busi[0] = type;
	xtcp_out(0,1);
}
/*************************************************
	Function	:
	Description	:����ʼ����ʱ�������EEPROM
	Calls		:
	Called By	:
	Input		:NONE
	Output		:NONE
	Return		:OK
	Others		:NONE
*************************************************/
uint8_t put_start_time(uint32_t t)
{
	iic_write(TIME_BASE  ,t&0xff);
	iic_write(TIME_BASE+1,t>>8);
	iic_write(TIME_BASE+2,t>>16);
	iic_write(TIME_BASE+3,t>>24);	
	return 0;
}

/*************************************************
	Function	:
	Description	:��ȡ��ʼ������ʱ���
	Calls		:
	Called By	:
	Input		:NONE
	Output		:NONE
	Return		:ʱ���
	Others		:NONE
*************************************************/
uint32_t get_start_time(void)
{
	uint8_t dt[4];
	uint32_t *ptr = (uint32_t *)dt;
	
	iic_reads(dt,TIME_BASE,sizeof(dt));
	return (*ptr);
}

//�����豸�����òɼ���
//����: *p�����ֻ����ʹ���
uint8_t dev_cfg_new(uint8_t *pflag)
{
	struct_dev_cfg_new *sdev = (struct_dev_cfg_new *)pk_buf.business;
	struct tm stime;
	
	*pflag = sdev->stack_flag;	
	iic_clear();
	menu_newPatient_DIS();			
	//time deal
	{	
		uint8_t *pbcd_t = sdev->time;
		time_t t;
		stime.tm_year = (pbcd_t[0]/16)*1000 + (pbcd_t[0]&0x0f)*100 + 
			(pbcd_t[1]>>4)*10 + (pbcd_t[1]&0x0f)-1900;  //from 1900
		stime.tm_mon  = (pbcd_t[2]/16)*10 + (pbcd_t[2]&0x0f)-1;     	//from 0
		stime.tm_mday = (pbcd_t[3]/16)*10 + (pbcd_t[3]&0x0f);     	//from 1
		stime.tm_hour = (pbcd_t[4]/16)*10 + (pbcd_t[4]&0x0f);     	//from 0
		stime.tm_min  = (pbcd_t[5]/16)*10 + (pbcd_t[5]&0x0f);     	//from 0
		stime.tm_sec  = (pbcd_t[6]/16)*10 + (pbcd_t[6]&0x0f); 		//from 0 
		t = kernel_mktime(&stime);
		put_start_time(t);		//������ʱ�����EEPROM
		
		//����ͨѶģ��ʱ��	
		RTC_WaitForLastTask();
		RTC_SetCounter(t);
		RTC_WaitForLastTask();
	}
	//alarm deal
	{	sAlarm alarm;
		alarm.highH[0] = (sdev->h_hgate&0xff00) >> 8;	//highH[1] + highH[0] * 256
		alarm.highH[1] = sdev->h_hgate & 0xff;	
		alarm.highL[0] = 0;
		alarm.highL[1] = sdev->h_lgate;
		alarm.lowH     = sdev->l_hgate;
		alarm.lowL     = sdev->l_lgate;
		alarm.avrH     = 180;
		alarm.avrL     = 30;
		alarm.rateH    = 180;
		alarm.rateL    = 30;
		cm_set_alarm(&alarm);
	}
//	uint8_t ch,ch1;
	//interval deal
	{
		sInterval interval;
		memset(&interval,0,sizeof(sInterval));
		interval.awake[0] = ((sdev->awk_tm[0])>>4)*10+((sdev->awk_tm[0])&0x0f);
		interval.awake[1] = ((sdev->awk_tm[1])>>4)*10+((sdev->awk_tm[1])&0x0f);
		interval.sleep[0] = ((sdev->slp_tm[0])>>4)*10+((sdev->slp_tm[0])&0x0f);
		interval.sleep[1] = ((sdev->slp_tm[1])>>4)*10+((sdev->slp_tm[1])&0x0f);	
		interval.awake_interval  = (sdev->awk_itv&0xff);
		interval.awake_intervalH = (sdev->awk_itv&0xff00)>>8;	
		interval.sleep_interval  = (sdev->slp_itv&0xff);
		interval.sleep_intervalH = (sdev->slp_itv&0xff00)>>8;	
		cm_set_interval(&interval);
//#if DEBUG == 1
//	printf("\r\n˯�Ѽ����%02x %02x",ch,ch1);
//    printf("\r\n˯�ѣ�%d %d",ch,ch1);
//#endif
	}

	
	//��ø��²ɼ���ʱ�䣬ͬʱ�ɼ���ִ���������
	delay_ms(100);
	cm_sync_time(stime.tm_hour,stime.tm_min);
	
	return OK;
}

//����: *p�����ֻ����ʹ���
uint8_t dev_cfg(uint8_t *pflag)
{
	struct_dev_config *sdev = (struct_dev_config *)pk_buf.business;
	sAlarm alarm;
	sInterval interval;
	struct tm stime;
	time_t t_time;
	uint8_t *pbcd_t = sdev->time;
	
	menu_newPatient_DIS();
	
	if((sdev->manual != NO_NEED_DISC) && (sdev->manual != NEED_DISC))
	{
		_log("cfg fail");
		standby();
	}
	*pflag = sdev->manual;
	
	iic_clear();	
				
	//time deal
	memset(&stime,0,sizeof(stime));
	stime.tm_year = (pbcd_t[0]/16)*1000 + (pbcd_t[0]&0x0f)*100 + (pbcd_t[1]>>4)*10 + (pbcd_t[1]&0x0f)-1900;  //from 1900
	stime.tm_mon  = (pbcd_t[2]/16)*10 + (pbcd_t[2]&0x0f)-1;     	//from 0
	stime.tm_mday = (pbcd_t[3]/16)*10 + (pbcd_t[3]&0x0f);     	//from 1
	stime.tm_hour = (pbcd_t[4]/16)*10 + (pbcd_t[4]&0x0f);     	//from 0
	stime.tm_min  = (pbcd_t[5]/16)*10 + (pbcd_t[5]&0x0f);     	//from 0
	stime.tm_sec  =     30;     								//from 0 
	t_time = kernel_mktime(&stime);
	put_start_time(t_time);

	//����ͨѶģ��ʱ��	
	RTC_WaitForLastTask();
	RTC_SetCounter(t_time);
	RTC_WaitForLastTask();
	
	//alarm deal
	memset(&alarm,0,sizeof(sAlarm));
	alarm.highH[0] = sdev->high_alert[1];
	alarm.highH[1] = sdev->high_alert[0];
	alarm.highL[0] = 0;
	alarm.highL[1] = 60;
	alarm.lowH     = sdev->low_alert;
	alarm.lowL     = 50;
	alarm.avrH     = 120;
	alarm.avrL     = 50;
	alarm.rateH    = 160;
	alarm.rateL    = 40;
	
	//interval deal
	memset(&interval,0,sizeof(sInterval));
	interval.awake[0] = ((sdev->awake[0])>>4)*10+((sdev->awake[0])&0x0f);
	interval.awake[1] = ((sdev->awake[1])>>4)*10+((sdev->awake[1])&0x0f);
	interval.sleep[0] = ((sdev->sleep[0])>>4)*10+((sdev->sleep[0])&0x0f);
	interval.sleep[1] = ((sdev->sleep[1])>>4)*10+((sdev->sleep[1])&0x0f);	
	interval.awake_interval = sdev->awake_interval;
	interval.awake_intervalH = sdev->awake_interval >> 8;	
	interval.sleep_interval = sdev->sleep_interval;	
	interval.sleep_intervalH = sdev->sleep_interval >> 8;	
		
	if(KO == cm_set_alarm(&alarm))
	{	_log("set alarm error");
	}
	
	if(KO == cm_set_interval(&interval))
	{
		_log("set itv error");
	}
	delay_ms(100);
	if(KO == cm_sync_time(stime.tm_hour,stime.tm_min))
	{
		_log("sync time error");
	}
	return OK;
}
/**
  * @brief :����ҵ�����ݰ�
	* @param :None
	* @retval:None
	*/
void business_deal(void *p, uint16_t len)
{	uint8_t *ptr = (uint8_t *)p;
	xsocket *sck;
	uint8_t phone_flag = 0;	//�ֻ����ͱ��

	ack_state = 0;
	
	if(pk_buf.hdr.port == 0)   //Ŀǰֻ����ͨ��0����
	{	sck = &xsck[0];
	}
	else
	{	sck = NULL;
	}

	if(sck)
	{	if(sck->flag & SCK_NEWDATA)
		{	sck->flag &= ~	SCK_NEWDATA;
			switch (ptr[0])
			{
		//�յ��������ð���1.xʹ�ã�����
			case DEV_CFG:
				{
					_log("dev cfg");					
					dev_cfg(&phone_flag);				
					BKP_WriteBackupRegister(RE_SND_CNT_BKB,0);	
					iic_write(DEV_FLAG,0x55);						//�豸���ţ���λ��־
					iic_write(PHONE_FLAG_BASE,phone_flag);			//д�� �ֻ���ʶ
					busi_ack_type = DEV_CFG;						//���ñ�־ ����ѭ������
					fp_busi_ack = busi_cfgdev_ack;						
					evt_set(BUSI_ACK);
				}
				break;	
		//�յ��������ð� 2.0
			case DEV_CFG_NEW:
				{
					if(len == sizeof(struct_dev_cfg_new))
					{
					_log("dev_cfg_new");
					dev_cfg_new(&phone_flag);
					BKP_WriteBackupRegister(RE_SND_CNT_BKB,0);	//�����ش�����
					iic_write(DEV_FLAG,0x55);					//ת���豸����״̬
					iic_write(PHONE_FLAG_BASE,phone_flag);		//��¼Э��ջ���
					busi_ack_type = DEV_CFG_NEW;				//�ظ�ͨ��ACK
					fp_busi_ack = busi_general_ack;						
					evt_set(BUSI_ACK);
					}
					else
					{
						_log("dev_cfg_error");
					}
				}
				break;
		//�յ�ѪѹACK
			case BLP_DATA:
				//�ɼ����˲����յ�Ѫѹ���� ֻ���յ�����Ѫѹ���ݵĻ�Ӧ
				//�յ���Ӧ ��ʾѪѹ���ݷ��ͳɹ� ����������1
				{
					if(len == sizeof(struct_bl_data_ack))
					{	u16 sends,saves;
						u16 temp;
						_log("busi: dt ack");
						temp = pk_buf.business[2] + (u16)(pk_buf.business[3]) * 256;
						sends = get_item(SEND_ITEM_BASE);
						if(temp)
						{
							if(temp-1 == sends)//��Ӧ���Ƿ��ͳ�ȥ����һ��
							{
								saves = get_item(SAVE_ITEM_BASE);
								if(saves > sends)
								{	
									sends++;
									put_item(SEND_ITEM_BASE,sends);
								}
								evt_set(BLP_SND_ENA);	//���Է������ݰ�								
							}
						}
					}
					else
					{	_log("busi: dt ack error");
					}
				}
				break;
		//�����豸��Ϣ
			case REQ_DEV_INFO:
				{
					_log("req_dev_info");
					busi_ack_type = UPDATA_DEV_INFO;		//���ñ�־ ����ѭ������
					fp_busi_ack = send_dev_info;						
					evt_set(DEV_INFO);					
				}
				break;
				
		//�յ��������ò�����(���Ƿ���)
			case RENEW_CFG:
				{
					int i;
					if(len == sizeof(struct_renew_cfg))
					{	//save info
						for(i=0; i<len; i++)
						{
							iic_write(RENEW_INFO_BASE+i,ptr[i]);
						}
						iic_write(RENEW_INFO_FLAG_BASE,1);		//update renew flag
						
						busi_ack_type = RENEW_CFG;		//����ͨ��ACK��Ϊ��Ӧ
						fp_busi_ack = busi_cfgdev_ack;						
						evt_set(BUSI_ACK);	
					}
				}
				break;
		//ʱ��ͬ����δ��		
			case CFG_TIME:
				{
					if(len == sizeof(struct_cfg_time))
					{	_log("busi: CFG_TIME");
						Updata_Time();
						busi_ack_type = CFG_TIME;		//���ñ�־ ����ѭ������
						fp_busi_ack = busi_general_ack;						
						evt_set(BUSI_ACK);
					}
					else
					{	_log("busi: CFG_TIME ERROR");
					}
				}
				break;
		//�յ��豸���գ���������δ�����������
			case DEV_BACK:
				{
					_log("busi: dev_back");
					iic_write(DEV_FLAG,0);			
					evt_set(BLP_SND_ENA);
				}
				break;
		//����������ַ
			case RE_ADDR:
				{
					_log("busi: re_addr");
					if(len == sizeof(struct_re_addr))
					{	//save bth addr
						iic_writes(&ptr[1],BTH_MAC_BASE,sizeof(struct_re_addr) - 1);
						busi_ack_type = RE_ADDR;		//���ñ�־ ����ѭ������
						fp_busi_ack = busi_general_ack;						
						evt_set(BUSI_ACK);						
					}
				}
				break;
		//�յ�IAP����ָ��
            //case REQ_DEV_INFO:
			case UPDATA_CMD:
				{
					_log("update_cmd");
					iic_write(UPDATA_FLAG_BASE,0x55);	//����������־
					busi_ack_type = UPDATA_ACK;
					fp_busi_ack = updata_ack;
					evt_set(UPDATA_FLAG);
				}
				break;
		//�յ���֧�ֵİ�
			default:
				{
					_log("invalid_pk");
					busi_ack_type = ptr[0];		//���ñ�־ ����ѭ������
					ack_state = 3;
					fp_busi_ack = busi_general_ack;						
					evt_set(BUSI_ACK);	
				}
				break;
			}
		}
	}
}

/**
  * @brief  ��ʱ����
  * @param  None
  * @retval None
  */
void timeout_deal(uint8_t ch)
{	xsck[ch].flag &= ~(SCK_REMIT_TIMEOUT | SCK_REMIT_FLAG);
	
	//�ϴ�ʧ�� �����´��ش�ʱ��   ���������ϴ�ʧ�ܣ�����Ҳ��Ӱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//	PWR_BackupAccessCmd(ENABLE);
//	RTC_SetAlarm(RTC_GetCounter()+RTC_ALARM_INTERVAL);
		
	at("+disc",500);
	standby();
}


//����Ƿ�������Ҫ���� ���� ����ȡһ����¼����
void data_send(void)
{	
	struct_bl_data blp_pk;
	uint8_t *psnd = (uint8_t *)&blp_pk;
	sBP_SAVE sbp_save;	
	uint8_t *prd = (uint8_t *)&sbp_save;
	uint16_t offset;
	uint16_t sends;
	uint32_t t;
	uint8_t str[30];
	uint8_t i,j;

	sends = get_item(SEND_ITEM_BASE);//��0��ʼ
	offset = sends * sizeof(sBP_SAVE) + DATA_BASE;
	iic_reads(prd,offset,sizeof(sBP_SAVE));
//	printf("dt crc : %d, crc:%d\r\n",crc_cal(&sbp_save,sizeof(sBP_SAVE)-1),sbp_save.crc);
	if(sbp_save.crc == crc_cal(prd,sizeof(sBP_SAVE)-1))
//	if(1)
	{
		blp_pk.type  = BLP_DATA;
		blp_pk.index = sends + 1;//��1��ʼ
		blp_pk.high  = sbp_save.high;
		blp_pk.low   = sbp_save.low;
		blp_pk.avr   = sbp_save.avr;
		blp_pk.rate  = sbp_save.rate;
		
		t = get_start_time();
		t += sbp_save.offset_time;
		strftime((char *)str,sizeof(str),"%Y%m%d%H%M00",localtime(&t));
		//printf("time:%s\r\n",str);
		for(i=0,j=0; i<7; i++,j=j+2)
		{
			blp_pk.time[i] = (str[j]&0x0f)*16 + (str[j+1]&0x0f);
		}
		memcpy((u8 *)pBUSI,psnd,sizeof(struct_bl_data));	
		xtcp_out(0,sizeof(struct_bl_data));
	}
	else
	{
		_log("eeprom error");
		//eeprom error deal
	}
}
