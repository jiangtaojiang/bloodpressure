#ifndef DX_TOUCH_H
#define DX_TOUCH_H
#define TOUCH_ADJ_TOLERANCE		200	//����У�������AD����

//�ĸ�У�������������,ע�ⲻ������Ϊ0.�������һ��ķ�ʽ,���������1/10
#define TADJ_XB		(MAX_X/10)
#define TADJ_YB		(MAX_Y/10)
#define TADJ_XE		(MAX_X-TADJ_XB)
#define TADJ_YE		(MAX_Y-TADJ_YB)

//����У����������ֵ
#define TADJ_RTN_OK				0		//�ɹ���ȡ�ĸ�У����
#define TADJ_RTN_FALSE			1		//��ȡУ����ʧ��
#define TADJ_RTN_CONTINUE		2		//��ȡ���ڽ�����

extern bool GuiTranTouchXY(u16 touch_x,u16 touch_y,LOC_X* x,LOC_Y* y);

//��Ҫ��Χ����Ӳ����д��3������
extern bool GetTouchPush(u16* x,u16* y);
extern bool GetTouchHold(u16* x,u16* y);
extern bool GetTouchRelease(u16* x,u16* y);


#endif
