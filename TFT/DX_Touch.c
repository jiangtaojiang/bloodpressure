/********************************************
 	DX_gui中的触摸屏部分.
 	用户需提供一个硬件存储方式存储触摸的校验值.这部分和硬件相关
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
	触摸定位四变量
***************************************************************/
u16 LCDX_Begin_AD=0xffff;		//LCD X起点对应AD-初始化为ffff,软件判断此值可知道触摸屏是否已经校正.建议这四个值校正后放到FLASH内.
u16 LCD_Length_AD;				//LCD X长度对应AD
u16 LCDY_Begin_AD;				//LCD Y起点对应AD
u16 LCD_Height_AD;				//LCD Y高度对应AD


/**********************************************
	和硬件相关的函数,需要用户提供一个空间存储触摸校验的信息
**********************************************/

/**************************************************************
** 函数名:SaveTouchAdjInfo
** 功能:保存触摸校正信息(4变量)
** 注意事项:触摸定位四变量,保存在后备寄存器中,并标志化后备寄存器2
***************************************************************/
void SaveTouchAdjInfo(void)
{
	BKP_WriteBackupRegister(BKP_TOUCH_X_BEGIN,LCDX_Begin_AD );  
	BKP_WriteBackupRegister(BKP_TOUCH_LENGTH,LCD_Length_AD );
	BKP_WriteBackupRegister(BKP_TOUCH_Y_BEGIN,LCDY_Begin_AD );
	BKP_WriteBackupRegister(BKP_TOUCH_HEIGHT,LCD_Height_AD );

	//标志化2
	BKP_WriteBackupRegister(BKP_TOUCH_SET,0xA55A);
}

/**************************************************************
** 函数名:LoadTouchAdjInfo
** 功能:读取触摸校正信息(4变量)
** 注意事项:
***************************************************************/
void LoadTouchAdjInfo(void)
{
	LCDX_Begin_AD=BKP_ReadBackupRegister(BKP_TOUCH_X_BEGIN);
	LCD_Length_AD=BKP_ReadBackupRegister(BKP_TOUCH_LENGTH);
	LCDY_Begin_AD=BKP_ReadBackupRegister(BKP_TOUCH_Y_BEGIN);
	LCD_Height_AD=BKP_ReadBackupRegister(BKP_TOUCH_HEIGHT);
}

/**************************************************************
** 函数名:NeedTouchAdj
** 功能:判断是否需要校正,需要的话返回TRUE
** 注意事项:校正信息保存在CPU的后备寄存器上,BKP_DR2放是否校正信息,BKP_DR3,BKP_DR4放校正数值 
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
** 函数名:TouchAdj
** 功能:触摸屏校正
** 注意事项:这是一个简单的触摸屏校正程序,校正成功则返回TADJ_RTN_OK,并且触摸定位的四个变量得到响应的值 
** 			这里使用非阻塞方式来实现(状态机方式)
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
	//流程->刷屏,显示四个顺序点->等待用户触摸
	switch(Touch_Adj_State)
	{
		//1.初始状态-清屏,画第一个点
		case TOUCH_ADJ_STATE_NULL:
			//这里应出现某个调整界面,以放置触摸的坐标使本来的WIN响应了
			//GuiWindowDraw(xx);
			
			TFTFill(WHITE);
			//显示要触摸校正触摸屏的信息
			TFTShowString16(50,50,"Push the Point!",BLACK,0);
			TFTRectFill(TADJ_XB-1,TADJ_YB-1,2,2,BLACK);
			Touch_Adj_State=TOUCH_ADJ_STATE_PUSH1;
			break;
		//2.PUSH1状态,等待第一个点被触摸
		case TOUCH_ADJ_STATE_PUSH1:
			if(GetTouchPush(&x0,&y0)==TRUE)
			{
				Touch_Adj_State=TOUCH_ADJ_STATE_RELEASE1;
				TFTRectFill(TADJ_XB-1,TADJ_YB-1,2,2,WHITE);
			}
			break;
		//3.RELEASE1状态,等待释放.
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

	//分析四个点是否合理-简易的分析,就是X0与X2,X1与X3相近,Y0与Y1,Y2与Y3相近.
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

	//计算定位四变量
	x0=(x0+x2)/2;
	x1=(x1+x3)/2;
	y0=(y0+y1)/2;
	y1=(y2+y3)/2;

	LCDX_Begin_AD=(TADJ_XE*x0-TADJ_XB*x1)/(TADJ_XE-TADJ_XB);
	LCD_Length_AD=(x0-LCDX_Begin_AD)*MAX_X/TADJ_XB;

	LCDY_Begin_AD=(TADJ_YE*y0-TADJ_YB*y1)/(TADJ_YE-TADJ_YB);
	LCD_Height_AD=(y0-LCDY_Begin_AD)*MAX_Y/TADJ_YB;
	
	//保存触摸信息
	SaveTouchAdjInfo();

	//这里可考虑出现某个界面
	//GuiWindowDraw(xx);
	return TADJ_RTN_OK;
}

/**************************************************************
** 函数名:GuiTranTouchXY
** 功能:通过触摸屏的坐标(AD值)转换为LCD坐标的XY
** 注意事项:一般在获取触摸坐标后再调用此函数作转换,转换后的结果保存在x,y指针中
**  		转换的基本规律看说明(还没写)
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



