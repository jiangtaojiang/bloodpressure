/********************************************
 	DX_gui�еĴ���������.
 	�û����ṩһ��Ӳ���洢��ʽ�洢������У��ֵ.�ⲿ�ֺ�Ӳ�����
*******************************************/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "GuiDef.h"
#include "tftdriver.h"
#include "DX_GUI.h"
#include "WIDGET.h"
#include "hal.h"
#include "touch.h"

/**************************************************************
	������λ�ı���
***************************************************************/
u16 LCDX_Begin_AD=0xffff;		//LCD X����ӦAD-��ʼ��Ϊffff,����жϴ�ֵ��֪���������Ƿ��Ѿ�У��.�������ĸ�ֵУ����ŵ�FLASH��.
u16 LCD_Length_AD;				//LCD X���ȶ�ӦAD
u16 LCDY_Begin_AD;				//LCD Y����ӦAD
u16 LCD_Height_AD;				//LCD Y�߶ȶ�ӦAD


/**********************************************
	��Ӳ����صĺ���,��Ҫ�û��ṩһ���ռ�洢����У�����Ϣ
**********************************************/

/**************************************************************
** ������:SaveTouchAdjInfo
** ����:���津��У����Ϣ(4����)
** ע������:������λ�ı���,�����ں󱸼Ĵ�����,����־���󱸼Ĵ���2
***************************************************************/
void SaveTouchAdjInfo(void)
{
	BKP_WriteBackupRegister(BKP_TOUCH_X_BEGIN,LCDX_Begin_AD );  
	BKP_WriteBackupRegister(BKP_TOUCH_LENGTH,LCD_Length_AD );
	BKP_WriteBackupRegister(BKP_TOUCH_Y_BEGIN,LCDY_Begin_AD );
	BKP_WriteBackupRegister(BKP_TOUCH_HEIGHT,LCD_Height_AD );

	//��־��2
	BKP_WriteBackupRegister(BKP_TOUCH_SET,0xA55A);
}

/**************************************************************
** ������:LoadTouchAdjInfo
** ����:��ȡ����У����Ϣ(4����)
** ע������:
***************************************************************/
void LoadTouchAdjInfo(void)
{
	LCDX_Begin_AD=BKP_ReadBackupRegister(BKP_TOUCH_X_BEGIN);
	LCD_Length_AD=BKP_ReadBackupRegister(BKP_TOUCH_LENGTH);
	LCDY_Begin_AD=BKP_ReadBackupRegister(BKP_TOUCH_Y_BEGIN);
	LCD_Height_AD=BKP_ReadBackupRegister(BKP_TOUCH_HEIGHT);
}

/**************************************************************
** ������:NeedTouchAdj
** ����:�ж��Ƿ���ҪУ��,��Ҫ�Ļ�����TRUE
** ע������:У����Ϣ������CPU�ĺ󱸼Ĵ�����,BKP_DR2���Ƿ�У����Ϣ,BKP_DR3,BKP_DR4��У����ֵ 
** 			
***************************************************************/
bool NeedTouchAdj(void)
{
	if(BKP_ReadBackupRegister(BKP_TOUCH_SET)!=0xA55A)
		return TRUE;
	else
	{	
		LoadTouchAdjInfo();
		return FALSE;
	}
}

/**************************************************************
** ������:TouchAdj
** ����:������У��
** ע������:����һ���򵥵Ĵ�����У������,У���ɹ��򷵻�TADJ_RTN_OK,���Ҵ�����λ���ĸ������õ���Ӧ��ֵ 
** 			����ʹ�÷�������ʽ��ʵ��(״̬����ʽ)
***************************************************************/
#define TOUCH_ADJ_STATE_NULL		0
#define TOUCH_ADJ_STATE_PUSH1		1
#define TOUCH_ADJ_STATE_RELEASE1	2
#define TOUCH_ADJ_STATE_PUSH2		3
#define TOUCH_ADJ_STATE_RELEASE2	4
#define TOUCH_ADJ_STATE_PUSH3		5
#define TOUCH_ADJ_STATE_RELEASE3	6
#define TOUCH_ADJ_STATE_PUSH4		7
#define TOUCH_ADJ_STATE_RELEASE4	8

u8 TouchAdj(void)
{
	static u16 x0,y0,x1,y1,x2,y2,x3,y3;
	static u8 Touch_Adj_State=TOUCH_ADJ_STATE_NULL;
	u16 tmp;
	//����->ˢ��,��ʾ�ĸ�˳���->�ȴ��û�����
	switch(Touch_Adj_State)
	{
		//1.��ʼ״̬-����,����һ����
		case TOUCH_ADJ_STATE_NULL:
			//����Ӧ����ĳ����������,�Է��ô���������ʹ������WIN��Ӧ��
			//GuiWindowDraw(xx);
			
			TFTFill(WHITE);
			//��ʾҪ����У������������Ϣ
			TFTShowString16(50,50,"Push the Point!",BLACK,0);
			TFTRectFill(TADJ_XB-1,TADJ_YB-1,2,2,BLACK);
			Touch_Adj_State=TOUCH_ADJ_STATE_PUSH1;
			break;
		//2.PUSH1״̬,�ȴ���һ���㱻����
		case TOUCH_ADJ_STATE_PUSH1:
			if(GetTouchPush(&x0,&y0)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_RELEASE1;
				TFTRectFill(TADJ_XB-1,TADJ_YB-1,2,2,WHITE);
			}
			break;
		//3.RELEASE1״̬,�ȴ��ͷ�.
		case TOUCH_ADJ_STATE_RELEASE1:
			if(GetTouchRelease(NULL,NULL)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_PUSH2;
				TFTRectFill(TADJ_XE-1,TADJ_YB-1,2,2,BLACK);
			}
			break;
		//4.PUSH2
		case TOUCH_ADJ_STATE_PUSH2:
			if(GetTouchPush(&x1,&y1)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_RELEASE2;
				TFTRectFill(TADJ_XE-1,TADJ_YB-1,2,2,WHITE);
			}
			break;
		//5.RELEASE2
		case TOUCH_ADJ_STATE_RELEASE2:
			if(GetTouchRelease(NULL,NULL)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_PUSH3;
				TFTRectFill(TADJ_XB-1,TADJ_YE-1,2,2,BLACK);
			}
			break;
		//6.PUSH3
		case TOUCH_ADJ_STATE_PUSH3:
			if(GetTouchPush(&x2,&y2)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_RELEASE3;
				TFTRectFill(TADJ_XB-1,TADJ_YE-1,2,2,WHITE);
			}
			break;
		//7.RELEASE3
		case TOUCH_ADJ_STATE_RELEASE3:
			if(GetTouchRelease(NULL,NULL)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_PUSH4;
				TFTRectFill(TADJ_XE-1,TADJ_YE-1,2,2,BLACK);
			}
			break;
		//8.PUSH4
		case TOUCH_ADJ_STATE_PUSH4:
			if(GetTouchPush(&x3,&y3)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_RELEASE4;
				TFTRectFill(TADJ_XE-1,TADJ_YE-1,2,2,WHITE);
			}
			break;
		//9.RELEASE4
		case TOUCH_ADJ_STATE_RELEASE4:
			if(GetTouchRelease(NULL,NULL)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_NULL;
				goto Calculate;
			}
			break;
		default:
			break;
	}
	
	return TADJ_RTN_CONTINUE;

Calculate:

	//�����ĸ����Ƿ����-���׵ķ���,����X0��X2,X1��X3���,Y0��Y1,Y2��Y3���.
	if(x0>x2)
		tmp=x0-x2;
	else
		tmp=x2-x0;
	if(tmp>TOUCH_ADJ_TOLERANCE)
		return TADJ_RTN_FALSE;

	if(x1>x3)
		tmp=x1-x3;
	else
		tmp=x3-x1;
	if(tmp>TOUCH_ADJ_TOLERANCE)
		return TADJ_RTN_FALSE;

	if(y0>y1)
		tmp=y0-y1;
	else
		tmp=y1-y0;
	if(tmp>TOUCH_ADJ_TOLERANCE)
		return TADJ_RTN_FALSE;

	if(y2>y3)
		tmp=y2-y3;
	else
		tmp=y3-y2;
	if(tmp>TOUCH_ADJ_TOLERANCE)
		return TADJ_RTN_FALSE;

	//���㶨λ�ı���
	x0=(x0+x2)/2;
	x1=(x1+x3)/2;
	y0=(y0+y1)/2;
	y1=(y2+y3)/2;

	LCDX_Begin_AD=(TADJ_XE*x0-TADJ_XB*x1)/(TADJ_XE-TADJ_XB);
	LCD_Length_AD=(x0-LCDX_Begin_AD)*MAX_X/TADJ_XB;

	LCDY_Begin_AD=(TADJ_YE*y0-TADJ_YB*y1)/(TADJ_YE-TADJ_YB);
	LCD_Height_AD=(y0-LCDY_Begin_AD)*MAX_Y/TADJ_YB;
	
	//���津����Ϣ
	SaveTouchAdjInfo();

	//����ɿ��ǳ���ĳ������
	//GuiWindowDraw(xx);
	return TADJ_RTN_OK;
}

/**************************************************************
** ������:GuiTranTouchXY
** ����:ͨ��������������(ADֵ)ת��ΪLCD�����XY
** ע������:һ���ڻ�ȡ����������ٵ��ô˺�����ת��,ת����Ľ��������x,yָ����
**  		ת���Ļ������ɿ�˵��(��ûд)
***************************************************************/
bool GuiTranTouchXY(u16 touch_x,u16 touch_y,LOC_X* x,LOC_Y* y)
{
	if(touch_x>LCDX_Begin_AD)
	{
		*x=((touch_x-LCDX_Begin_AD)*MAX_X)/LCD_Length_AD;
	}
	else
		return FALSE;

	if(touch_y>LCDY_Begin_AD)
	{
		*y=((touch_y-LCDY_Begin_AD)*MAX_Y)/LCD_Height_AD;
	}
	else
		return FALSE;
	return TRUE;
}



