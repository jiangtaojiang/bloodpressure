#ifndef __EVT_H__
#define __EVT_H__

//-------------------------------------------------------------
extern volatile unsigned long msTicks;
#define LINE_LENGTH_MASK 63
extern char line[64];

#define RE_SEND_TIME (60)		//RTC���Ѽ�� ��λ:��
#define LINK_WAIT_TIME 60		//�������Ѻ󣬵ȴ�����ʱ�� ��λ:��
#define WAIT_FOR_LINK (40)		//������ȴ�ʱ�� ��λ:��

#define NO_NEED_DISC 1
#define NEED_DISC 2
//-------------------------------------------------------------

#define soft_reset() *((u32*)0xE000ED0C)=0x05fa0004


extern volatile unsigned long evt_state;
//extern volatile unsigned char send_flag;

#define _BV(x) (1<<(x))
								//�����¼�  
								//usart3�յ�һ�����ݺ� evt_set(DBG_EVT)
								//��ѭ��evt_chk(DBG_EVT)
								//������¼� evt_clr(DBG_EVT) ���deg()ִ��һ��
#define DBG_EVT 		(1<<0)		

								//tim2��ȡѪѹ����timer���� �� evt_set
								//��ѭ��evt_chk ��
								//�����¼� evt_clr, ִ��cm_down() 
#define CM_DOWN_EVT	 	(1<<1)	//���زɼ�ģ���¼�

#define BLP_SND_ENA		(1<<3)	//���������ݰ�
#define BUSI_ACK		(1<<4)	//��Ҫ����ҵ��ACK


#define DISC_FLAG		(1<<5)	//���ݷ������־�����ݷ��������ݰ� ���ͳɹ�����λ ���ڶ��ߴ���
#define BLP_DT_OVER		_BV(6)
#define UPDATA_FLAG		_BV(7)	//����ָ��ظ�
#define DEV_INFO		_BV(8)	//�����豸��Ϣ

#define evt_set(x) evt_state |= x 
#define evt_clr(x) evt_state &= ~x
#define evt_chk(x) (evt_state & x)
//-------------------------------------------------------------

#define RTC_BKP BKP_DR1		//RTC����ʹ��
#define FLAG_BKP BKP_DR5	//�������� �� RTC���ӻ��� ��־ �Ĵ���
	#define RTC_WAKE_FLAG 0x55AA
	#define PA0_WAKE_FLAG 0xAA55

//Ѫѹ���ݰ��ط��Ĵ��� ���ش���ʱ�� ��λ:s 
//���� һ�η���ʧ�ܣ�5min��������һ�η���
//��ʧ�� 10min���� ���ʧ�� ���� �������
#define RE_SND_CNT_BKB	BKP_DR6
#define MAX_RE_SND_CNT  3

//-------------------------------------------------------------

void dbg(void);
void delay_ms(unsigned long dlyTicks);
void delay_sec(unsigned long dlys);
void delay(unsigned long t);


#endif
