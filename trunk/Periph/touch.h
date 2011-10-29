#ifndef TOUCH_H
#define TOUCH_H


//TOUCH-������־λ��Ϊ�˽�ʡ�ڴ棬ʹ��λ��ʽ
#define TOUCH_F_NULL			0
#define TOUCH_F_PUSH 			1				//������Ч
#define TOUCH_F_BCK_TIME 		(1<<1)			//������ʼ���������˱�־��û��������Ч
#define TOUCH_F_FILTERING		(1<<2)			//������ʼ�˲�
#define TOUCH_F_RELEASE			(1<<3)			//�����ͷ�*���������־������û�������.�������PUSH,HOLDͬʱЯ��,���ܱ�ʾ�ͷŵ�״̬
//#define TOUCH_F_BPK_TIME		(1<<4)			//������־-����
#define TOUCH_F_WAIT_HOLD		(1<<5)			//�ȴ�����
#define TOUCH_F_HOLD			(1<<6)			//���ֱ�־

//TOUCH �˲�������
#define MAX_TOUCH_FILTER_NO			10				//�˲�10��

//�����ļ���ֵ
#define TOUCH_CNT_FLITER_TIME		50				//���������ʱ��,�����ʱ��������˲�״̬
#define TOUCH_CNT_WAIT_HOLD_TIME	5				//WAIT_HOLD�˲��������,��Ҫ���������ȫʱ���,������ʽ��ΪHOLD������
#define TOUCH_CNT_HOLD_TIME			5				//��һ�δ�����Ч��,����ʼHOLD�˲���ʱ��

extern void ManageTouch(void);					//����������-1MS����һ��
extern bool GetTouchPush(u16* x,u16* y);
extern bool GetTouchHold(u16* x,u16* y);
extern bool GetTouchRelease(u16* x,u16* y);
extern void ClearTouchFlag(void);

#endif
