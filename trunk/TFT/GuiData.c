/**************************************************************
**ģ����:GuiData.c
**����:GUI���û�����,�ؼ��ʹ��嶼������ʵ��Ԥ����. ע��ĳЩ�ڲ�ָ��ָ����ڴ�ռ�,�粻ϣ���ⲿֱ�ӵ���.��Ҫ����Ϊstatic. 
**�Ƿ������û��ⲿֱ�ӵ���,���û�������. 
**�޸ļ�¼: 
***************************************************************/
#define GuiData_c
#include "stm32f10x.h"
#include "GuiDef.h"
#include "DX_gui.h"
#include "BMP\\bmp_data.h"
#include "usr.h"


extern DX_WINDOW BAR_WINDOW;
extern DX_WINDOW MAIN_WINDOW;
extern DX_WINDOW TOUCH_ADJ_WINDOW;
extern DX_WINDOW SET_TIME_WINDOW;
extern DX_WINDOW TOUCH_TEST_WINDOW;
extern DX_EDIT TOUCH_TEST_EDIT_LOCX;
extern DX_EDIT TOUCH_TEST_EDIT_LOCY;


//ʱ��������
#define BAR_WIN_X			0
#define BAR_WIN_Y			0
#define BAR_WIN_L			240
#define BAR_WIN_H			20
//״̬������
#define STU_WIN_X			0
#define STU_WIN_Y			300
#define STU_WIN_L			240
#define STU_WIN_H			20
//������
#define MAIN_WIN_X			0
#define MAIN_WIN_Y			(BAR_WIN_H)
#define MAIN_WIN_L			240
#define MAIN_WIN_H			(MAX_Y-MAIN_WIN_Y*2)
//������У������
#define TOUCH_ADJ_WIN_X		0
#define TOUCH_ADJ_WIN_Y		0
#define TOUCH_ADJ_WIN_L		240
#define TOUCH_ADJ_WIN_H		320
//ʱ��У������
#define SET_TIME_WIN_X			0
#define SET_TIME_WIN_Y			(BAR_WIN_H)
#define SET_TIME_WIN_L			240
#define SET_TIME_WIN_H			(MAX_Y-SET_TIME_WIN_Y)
//���������Դ���
#define TOUCH_TEST_WIN_X		0                       
#define TOUCH_TEST_WIN_Y        (BAR_WIN_H)             
#define TOUCH_TEST_WIN_L        240                     
#define TOUCH_TEST_WIN_H        (MAX_Y-SET_TIME_WIN_Y)  

/////////////////////////////////////////////////////////////////////////////
//BAR_WINDOW�ؼ�����
/////////////////////////////////////////////////////////////////////////////
////ʱ��λ��
#define LOC_TIME_YEAR_X			0
#define LOC_TIME_MONTH_X		(LOC_TIME_YEAR_X+24)
#define LOC_TIME_DAY_X			(LOC_TIME_MONTH_X+24)
//#define LOC_TIME_HOUR_X			(LOC_TIME_DAY_X+24)
#define LOC_TIME_HOUR_X			(LOC_TIME_YEAR_X+176)
#define LOC_TIME_MINUTE_X		(LOC_TIME_HOUR_X+24)
#define LOC_TIME_SECOND_X		(LOC_TIME_MINUTE_X+24)
#define LOC_TIME_DATE_X			(LOC_TIME_SECOND_X+24)

#define LOC_TIME_Y				2

#define	TIME_L				16
#define TIME_H				16


//��Ӧ����
void BarWin(u8 param)
{
	if(param==WIDGET_PARAM_SHOW)
	{
		//TFTShowString16(16,LOC_TIME_Y,"/  /     :  :",WHITE,0);
      TFTShowString16(176,LOC_TIME_Y,"  :  :",WHITE,0);
       TFTShowString16(0,LOC_TIME_Y,"Ѫѹ�ɼ���",WHITE,0);
	}
}

//��
static EPRO  __ext_pro_time_year;
static SPRO  __self_pro_time_year;
static s32 __tmp_edit_time_year;

DX_EDIT BAR_WIN_TIME_YEAR={
/*����ߴ�*/    LOC_TIME_YEAR_X,LOC_TIME_Y,TIME_L,TIME_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_time_year,
/*������_*/     &BAR_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		WHITE,
/*EDIT����*/    &__self_pro_time_year,
/*��ʱֵ*/      &__tmp_edit_time_year,
/*ʵ��ֵ*/      NULL,
/*���ֵ*/      99,
/*��Сֵ*/      0,

};

//��
static EPRO  __ext_pro_time_month;
static SPRO  __self_pro_time_month;
static s32 __tmp_time_month;

DX_EDIT BAR_WIN_TIME_MONTH={
/*����ߴ�*/    LOC_TIME_MONTH_X,LOC_TIME_Y,TIME_L,TIME_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_time_month,
/*������_*/     &BAR_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		WHITE,
/*EDIT����*/    &__self_pro_time_month,
/*��ʱֵ*/      &__tmp_time_month,
/*ʵ��ֵ*/      NULL,
/*���ֵ*/      12,
/*��Сֵ*/      1,
};

//��
static EPRO  __ext_pro_time_day;
static SPRO  __self_pro_time_day;
static s32 __tmp_time_day;

DX_EDIT BAR_WIN_TIME_DAY={
/*����ߴ�*/    LOC_TIME_DAY_X,LOC_TIME_Y,TIME_L,TIME_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_time_day,
/*������_*/     &BAR_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		WHITE,
/*EDIT����*/    &__self_pro_time_day,
/*��ʱֵ*/      &__tmp_time_day,
/*ʵ��ֵ*/      NULL,
/*���ֵ*/      31,
/*��Сֵ*/      1,
};

//ʱ
static EPRO  __ext_pro_time_hour;
static SPRO  __self_pro_time_hour;
static s32 __tmp_time_hour;

DX_EDIT BAR_WIN_TIME_HOUR={
/*����ߴ�*/    LOC_TIME_HOUR_X,LOC_TIME_Y,TIME_L,TIME_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_time_hour,
/*������_*/     &BAR_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		WHITE,
/*EDIT����*/    &__self_pro_time_hour,
/*��ʱֵ*/      &__tmp_time_hour,
/*ʵ��ֵ*/      NULL,
/*���ֵ*/      23,
/*��Сֵ*/      0,
};

//��
static EPRO  __ext_pro_time_minute;
static SPRO  __self_pro_time_minute;
static s32 __tmp_time_minute;

DX_EDIT BAR_WIN_TIME_MINUTE={
/*����ߴ�*/    LOC_TIME_MINUTE_X,LOC_TIME_Y,TIME_L,TIME_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_time_minute,
/*������_*/     &BAR_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		WHITE,
/*EDIT����*/    &__self_pro_time_minute,
/*��ʱֵ*/      &__tmp_time_minute,
/*ʵ��ֵ*/      NULL,
/*���ֵ*/      59,
/*��Сֵ*/      0,
};

//��
static EPRO  __ext_pro_time_second;
static SPRO  __self_pro_time_second;
static s32 __tmp_time_second;

DX_EDIT BAR_WIN_TIME_SECOND={
/*����ߴ�*/    LOC_TIME_SECOND_X,LOC_TIME_Y,TIME_L,TIME_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_time_second,
/*������_*/     &BAR_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		WHITE,
/*EDIT����*/    &__self_pro_time_second,
/*��ʱֵ*/      &__tmp_time_second,
/*ʵ��ֵ*/      NULL,
/*���ֵ*/      59,
/*��Сֵ*/      0,
};

//����
char const* const __str_date_combo[7]={"������","����һ","���ڶ�","������","������","������","������",};
 
static EPRO __ext_pro_time_date;
static SPRO __self_pro_time_date;
static u8 __tmp_edit_time_date;
u8 Date;
DX_COMBO BAR_WIN_TIME_DATE={
/*����ߴ�*/	LOC_TIME_DATE_X,LOC_TIME_Y,48,TIME_H,
/*��������*/	BASE_PRO_COMBO|BASE_PRO_FONT16|BASE_PRO_COMBO_STYLE_NULL|BASE_PRO_COMBO_TYPE_STRING|BASE_PRO_FORBID_FOCUS,
/*��չ����*/	&__ext_pro_time_date,
/*������_*/     &BAR_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		WHITE,
/*COMBO����*/	&__self_pro_time_date,
/*��ʱֵ*/		&__tmp_edit_time_date,
/*ʵ��ֵ*/		NULL,
/*Ԫ����*/		(void**)__str_date_combo,
/*���ֵ*/		7,
/*2Dɫ*/		0,
};

DX_WIDGET* const BAR_WINDOW_CHILD[]={
//	&(BAR_WIN_TIME_YEAR.widget),
//	&(BAR_WIN_TIME_MONTH.widget),
//	&(BAR_WIN_TIME_DAY.widget),
	&(BAR_WIN_TIME_HOUR.widget),
	&(BAR_WIN_TIME_MINUTE.widget),
	&(BAR_WIN_TIME_SECOND.widget),
//	&(BAR_WIN_TIME_DATE.widget),
};

static EPRO __ext_pro_test_window;
static FNO __p_focus_no_test_window;
DX_WINDOW BAR_WINDOW={
	BAR_WIN_X,BAR_WIN_Y,BAR_WIN_L,BAR_WIN_H,	//�����������С
	0,											//��������
	&__ext_pro_test_window,						//��չ����
	NULL,										// ����������
	BarWin,										//��Ӧ����
	BLUE,										//���ڱ���ɫ
	BAR_WINDOW_CHILD,							//�ӿؼ���
	sizeof(BAR_WINDOW_CHILD)/4,					//�ؼ���
	&__p_focus_no_test_window,					//�����

};

DX_WINDOW STU_WINDOW={
	STU_WIN_X,STU_WIN_Y,STU_WIN_L,STU_WIN_H,	//�����������С
	0,											//��������
	&__ext_pro_test_window,						//��չ����
	NULL,										// ����������
	NULL,//BarWin,										//��Ӧ����
	BLUE,										//���ڱ���ɫ
	BAR_WINDOW_CHILD,							//�ӿؼ���
	0,//sizeof(BAR_WINDOW_CHILD)/4,					//�ؼ���
	&__p_focus_no_test_window,					//�����

};
//////////////////////////////////////////////////////////////////
//MAIN_WIN
//////////////////////////////////////////////////////////////////
#define CHECK_BOX16_X		80
#define CHECK_BOX24_X		96
#define CHECK_BOX32_X		120
#define CHECK_BOX_Y			32

#define PROGRESS_AD_X		56
#define PROGRESS_AD_Y		64
#define PROGRESS_AD_L		100
#define PROGRESS_AD_H		32

#define CHECK_UDISK_X		196
#define CHECK_UDISK_Y		64
#define CHECK_UDISK_L		39
#define CHECK_UDISK_H		28

#define CHECK_NET_X			56
#define CHECK_NET_Y			100
#define CHECK_NET_L			32
#define CHECK_NET_H			32

#define BUTTON_LED_X     	136
#define BUTTON_LED_Y     	100
#define BUTTON_LED_L     	64
#define BUTTON_LED_H     	30

#define EDIT_X				40
#define EDIT_Y				136
#define EDIT_L				80
#define EDIT_H				30

#define ADC_X				0
#define ADC_Y				250
#define ADC_L				80
#define ADC_H				45

#define ADC1_X				80
#define ADC1_Y				250
#define ADC1_L				80
#define ADC1_H				45

#define VBAT_X				160
#define VBAT_Y				250
#define VBAT_L				80
#define VBAT_H				45

#define SBP_X				116
#define SBP_Y				45
#define SBP_L				100
#define SBP_H				58

#define DBP_X				116
#define DBP_Y				115
#define DBP_L				100
#define DBP_H				58

#define PR_X				116
#define PR_Y				185
#define PR_L				100
#define PR_H				58

#define BUTTON_TOUCH_ADJ_X     	40
#define BUTTON_TOUCH_ADJ_Y     	174
#define BUTTON_TOUCH_ADJ_L     	72
#define BUTTON_TOUCH_ADJ_H     	30

#define BUTTON_SET_TIME_X     	128
#define BUTTON_SET_TIME_Y     	174
#define BUTTON_SET_TIME_L     	72
#define BUTTON_SET_TIME_H     	30

#define BUTTON_TOUCH_TEST_X    	40
#define BUTTON_TOUCH_TEST_Y    	208
#define BUTTON_TOUCH_TEST_L     72
#define BUTTON_TOUCH_TEST_H     30


void MainWin(u8 param)
{
	if(param==WIDGET_PARAM_SHOW)
	{
        TFTShowString16(16,30,"����ʱ��:",BLUE,0);
		TFTShowString24(16,70,"����ѹ",BLUE,0);
		TFTShowString24(16,140,"����ѹ",BLUE,0);
		TFTShowString24(16,210,"����",BLUE,0);
//        TFTShowString32(16+100,60,"777",BLUE,0);
//        TFTShowString32(16+100,130,"999",BLUE,0);
//        TFTShowString32(16+100,200,"888",BLUE,0);
//		TFTShowString24(50,250,"000",BLUE,0);
//		TFTShowString16(0,64+8,"��ǰAD:",BLACK,0);

	}
}
//����У��
void RspButtonTouchAdj(u8 param)
{
 	if(param==WIDGET_PARAM_ENTER)
	{
		GuiWindowDraw(&TOUCH_ADJ_WINDOW);
	}
}
//ʱ���趨
void RspButtonSetTime(u8 param)
{
 	if(param==WIDGET_PARAM_ENTER)
	{
		GuiWindowDraw(&SET_TIME_WINDOW);
	}
}
//��������
void RspButtonTouchTest(u8 param)
{
	if(param==WIDGET_PARAM_ENTER)
	{
		GuiWindowDraw(&TOUCH_TEST_WINDOW);
	}
}

//CHECK16,24,32-16,24,32�Ĺ�����һ��
u8 __ext_pro_check16;
u8 __self_pro_check16;
bool __tmp_check16=TRUE;
DX_CHECK MAIN_WIN_CHECK16={
/*����ߴ�*/    CHECK_BOX16_X,CHECK_BOX_Y,0,0,
/*��������*/    BASE_PRO_CHECK|BASE_PRO_FONT16|BASE_PRO_CHECK_TYPE_HOOK,
/*��չ����*/    &__ext_pro_check16,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*CHECK����*/   &__self_pro_check16,
/*��ʱֵ*/		&__tmp_check16,
/*ʵ��ֵ*/		NULL,
/*��Ŀ��*/		NULL,
/*2Dɫ*/		0,
};

u8 __ext_pro_check24;
u8 __self_pro_check24;
bool __tmp_check24=TRUE;
DX_CHECK MAIN_WIN_CHECK24={
/*����ߴ�*/    CHECK_BOX24_X,CHECK_BOX_Y,0,0,
/*��������*/    BASE_PRO_CHECK|BASE_PRO_FONT24|BASE_PRO_CHECK_TYPE_HOOK,
/*��չ����*/    &__ext_pro_check24,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*CHECK����*/   &__self_pro_check24,
/*��ʱֵ*/		&__tmp_check24,
/*ʵ��ֵ*/		NULL,
/*��Ŀ��*/		NULL,
/*2Dɫ*/		0,
};

u8 __ext_pro_check32;
u8 __self_pro_check32;
bool __tmp_check32=TRUE;
DX_CHECK MAIN_WIN_CHECK32={
/*����ߴ�*/    CHECK_BOX32_X,CHECK_BOX_Y,0,0,
/*��������*/    BASE_PRO_CHECK|BASE_PRO_FONT32|BASE_PRO_CHECK_TYPE_HOOK,
/*��չ����*/    &__ext_pro_check32,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*CHECK����*/   &__self_pro_check32,
/*��ʱֵ*/		&__tmp_check32,
/*ʵ��ֵ*/		NULL,
/*��Ŀ��*/		NULL,
/*2Dɫ*/		0,
};

//PROGRESS--��ʾAD
u8 __ext_pro_progress;
u8 __self_pro_progress;
s32 __tmp_progress;

DX_PROGRESS MAIN_WIN_PROGRESS={
/*����ߴ�*/    PROGRESS_AD_X,PROGRESS_AD_Y,PROGRESS_AD_L,PROGRESS_AD_H,
/*��������*/    BASE_PRO_PROGRESS|BASE_PRO_PROGRESS_TYPE_NUM|BASE_PRO_PROGRESS_STYLE_3D/*|BASE_PRO_FORBID_FOCUS*/,
/*��չ����*/    &__ext_pro_progress,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*CHECK����*/   &__self_pro_progress,
/*��ʱֵ*/		&__tmp_progress,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		4095,
/*��Сֵ*/		0,
/*��ɫ*/		WHITE,
/*��ɫ*/		BLUE,
};

//CHECK--STRING���� ���U��״̬
u8 __ext_pro_check_udisk;
u8 __self_pro_check_udisk;
bool __tmp_check_udisk=FALSE;
const char * const __string_check_udisk[2]={"����","��Ч"};
DX_CHECK MAIN_WIN_CHECK_UDISK={
/*����ߴ�*/    CHECK_UDISK_X,CHECK_UDISK_Y,CHECK_UDISK_L,CHECK_UDISK_H,
/*��������*/    BASE_PRO_CHECK|BASE_PRO_CHECK_TYPE_STRING|BASE_PRO_FORBID_FOCUS|BASE_PRO_CHECK_STYLE_2D,
/*��չ����*/    &__ext_pro_check_udisk,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*CHECK����*/   &__self_pro_check_udisk,
/*��ʱֵ*/		&__tmp_check_udisk,
/*ʵ��ֵ*/		NULL,
/*��Ŀ��*/		(void**)__string_check_udisk,
/*2Dɫ*/		GREEN,
};


//CHECK--BMP���� ����ɸı��ͼ�� 
u8 __ext_pro_check_net;
u8 __self_pro_check_net;
bool __tmp_check_net=FALSE;
const COLOR * const __bmp_check_net[2]={Bmp_Check1,Bmp_Check2};
DX_CHECK MAIN_WIN_CHECK_NET={
/*����ߴ�*/    CHECK_NET_X,CHECK_NET_Y,CHECK_NET_L,CHECK_NET_H,
/*��������*/    BASE_PRO_CHECK|BASE_PRO_CHECK_TYPE_BMP|BASE_PRO_FORBID_FOCUS,
/*��չ����*/    &__ext_pro_check_net,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*CHECK����*/   &__self_pro_check_net,
/*��ʱֵ*/		&__tmp_check_net,
/*ʵ��ֵ*/		NULL,
/*��Ŀ��*/		(void**)__bmp_check_net,
/*2Dɫ*/		0,
};

//BUTTON-���Ըı�LED�Ƶ�״̬
static u8 __ext_pro_button_led;
static u8 __self_pro_button_led;
DX_BUTTON MAIN_WIN_BUTTON_LED={
/*����ߴ�*/	BUTTON_LED_X,BUTTON_LED_Y,BUTTON_LED_L,BUTTON_LED_H,
/*��������*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*��չ����*/	&__ext_pro_button_led,
/*������_*/		&MAIN_WINDOW,
/*��Ӧ����*/    RspButtonLed,
/*��ɫ*/		0,
/*��������*/	&__self_pro_button_led,
/*�ַ���*/		"���Լ�",
};

//EDIT-0P����,��ʼ��λһ������
u8  __ext_pro_edit;
u8  __self_pro_edit;
s32 __tmp_edit=-1024;
s32 Data_Edit;		//�˱������ⲿ����
DX_EDIT MAIN_WIN_EDIT={
/*����ߴ�*/    EDIT_X,EDIT_Y,EDIT_L,EDIT_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16,
/*��չ����*/    &__ext_pro_edit,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_edit,
/*��ʱֵ*/		&__tmp_edit,
/*ʵ��ֵ*/		&Data_Edit,
/*���ֵ*/		99999,
/*��Сֵ*/		-9999,
};

//BUTTON-������У������
static u8 __ext_pro_button_touch_adj;
static u8 __self_pro_button_touch_adj;
DX_BUTTON MAIN_WIN_BUTTON_TOUCH_ADJ={
/*����ߴ�*/	BUTTON_TOUCH_ADJ_X,BUTTON_TOUCH_ADJ_Y,BUTTON_TOUCH_ADJ_L,BUTTON_TOUCH_ADJ_H,
/*��������*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*��չ����*/	&__ext_pro_button_touch_adj,
/*������_*/		&MAIN_WINDOW,
/*��Ӧ����*/    RspButtonTouchAdj,
/*��ɫ*/		0,
/*��������*/	&__self_pro_button_touch_adj,
/*�ַ���*/		"����У��",
};

//BUTTON-ʱ���趨
static u8 __ext_pro_button_set_time;
static u8 __self_pro_button_set_time;
DX_BUTTON MAIN_WIN_BUTTON_SET_TIME={
/*����ߴ�*/	BUTTON_SET_TIME_X,BUTTON_SET_TIME_Y,BUTTON_SET_TIME_L,BUTTON_SET_TIME_H,
/*��������*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*��չ����*/	&__ext_pro_button_set_time,
/*������_*/		&MAIN_WINDOW,
/*��Ӧ����*/    RspButtonSetTime,
/*��ɫ*/		0,
/*��������*/	&__self_pro_button_set_time,
/*�ַ���*/		"ʱ���趨",
};

//BUTTON-��������
static u8 __ext_pro_button_touch_test;
static u8 __self_pro_button_touch_test;
DX_BUTTON MAIN_WIN_BUTTON_TOUCH_TEST={
/*����ߴ�*/	BUTTON_TOUCH_TEST_X,BUTTON_TOUCH_TEST_Y,BUTTON_TOUCH_TEST_L,BUTTON_TOUCH_TEST_H,
/*��������*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*��չ����*/	&__ext_pro_button_touch_test,
/*������_*/		&MAIN_WINDOW,
/*��Ӧ����*/    RspButtonTouchTest,
/*��ɫ*/		0,
/*��������*/	&__self_pro_button_touch_test,
/*�ַ���*/		"��������",
};
//ADC show
static EPRO  __ext_pro_adc;
u8  __self_pro_adc;
s32 __tmp_adc = 0;
s32 Data_Edit_adc;		//�˱������ⲿ����
DX_EDIT MAIN_WIN_ADC={
/*����ߴ�*/    ADC_X,ADC_Y,ADC_L,ADC_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT24|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*��չ����*/    &__ext_pro_adc,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_adc,
/*��ʱֵ*/		&__tmp_adc,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		4096,
/*��Сֵ*/		0,
};
//ADC1 show
static EPRO  __ext_pro_adc1;
u8  __self_pro_adc1;
s32 __tmp_adc1 = 0;
s32 Data_Edit_adc1;		//�˱������ⲿ����
DX_EDIT MAIN_WIN_ADC1={
/*����ߴ�*/    ADC1_X,ADC1_Y,ADC1_L,ADC1_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT24|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*��չ����*/    &__ext_pro_adc1,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_adc1,
/*��ʱֵ*/		&__tmp_adc1,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		4096,
/*��Сֵ*/		0,
};
//VBAT show
static EPRO  __ext_pro_vbat;
u8  __self_pro_vbat;
s32 __tmp_vbat = 0;
s32 Data_Edit_vbat;		//�˱������ⲿ����
DX_EDIT MAIN_WIN_VBAT={
/*����ߴ�*/    VBAT_X,VBAT_Y,VBAT_L,VBAT_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT24|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*��չ����*/    &__ext_pro_vbat,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_vbat,
/*��ʱֵ*/		&__tmp_vbat,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		330,
/*��Сֵ*/		0,
};
//SBP show ����ѹ
static EPRO  __ext_pro_sbp;
u8  __self_pro_sbp;
s32 __tmp_sbp = 300;
s32 Data_Edit_sbp;		//�˱������ⲿ����
DX_EDIT MAIN_WIN_SBP={
/*����ߴ�*/    SBP_X,SBP_Y,SBP_L,SBP_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT32|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*��չ����*/    &__ext_pro_sbp,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		BLUE,
/*EDIT����*/    &__self_pro_sbp,
/*��ʱֵ*/		&__tmp_sbp,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		300,
/*��Сֵ*/		0,
};
//DBP show ����ѹ
static EPRO  __ext_pro_dbp;
u8  __self_pro_dbp;
s32 __tmp_dbp = 200;
s32 Data_Edit_dbp;		//�˱������ⲿ����
DX_EDIT MAIN_WIN_DBP={
/*����ߴ�*/    DBP_X,DBP_Y,DBP_L,DBP_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT32|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*��չ����*/    &__ext_pro_dbp,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		BLUE,
/*EDIT����*/    &__self_pro_dbp,
/*��ʱֵ*/		&__tmp_dbp,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		300,
/*��Сֵ*/		0,
};
//PR show ����
static EPRO  __ext_pro_pr;
u8  __self_pro_pr;
s32 __tmp_pr = 100;
s32 Data_Edit_pr;		//�˱������ⲿ����
DX_EDIT MAIN_WIN_PR={
/*����ߴ�*/    PR_X,PR_Y,PR_L,PR_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT32|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*��չ����*/    &__ext_pro_pr,
/*������_*/     &MAIN_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		BLUE,
/*EDIT����*/    &__self_pro_pr,
/*��ʱֵ*/		&__tmp_pr,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		180,
/*��Сֵ*/		0,
};

DX_WIDGET* const MAIN_WINDOW_CHILD[]={
    &(MAIN_WIN_ADC.widget),
    &(MAIN_WIN_ADC1.widget),
	&(MAIN_WIN_SBP.widget),
	&(MAIN_WIN_DBP.widget),
	&(MAIN_WIN_PR.widget),
	&(MAIN_WIN_VBAT.widget),
//	&(MAIN_WIN_CHECK_UDISK.widget),
//	&(MAIN_WIN_CHECK_NET.widget),
//	&(MAIN_WIN_BUTTON_LED.widget),
	//&(MAIN_WIN_EDIT.widget),
//	&(MAIN_WIN_BUTTON_TOUCH_ADJ.widget),
//	&(MAIN_WIN_BUTTON_SET_TIME.widget),
//	&(MAIN_WIN_BUTTON_TOUCH_TEST.widget),
};


static EPRO __ext_pro_main_window;
static FNO __p_focus_no_main_window;
DX_WINDOW MAIN_WINDOW={
	MAIN_WIN_X,MAIN_WIN_Y,MAIN_WIN_L,MAIN_WIN_H,						//�����������С
	0,									//��������
	&__ext_pro_main_window,				//��չ����
	NULL,								// ����������
	MainWin,
	WHITE,								//���ڱ���ɫ
	MAIN_WINDOW_CHILD,					//�ӿؼ���
	sizeof(MAIN_WINDOW_CHILD)/4,		//�ؼ���
	&__p_focus_no_main_window,			//�����

};

/////////////////////////////////////////////////////////////////////
// 	������У������
////////////////////////////////////////////////////////////////////
extern bool Touch_Adj;
void TouchAdjWin(u8 param)
{
	if(param==WIDGET_PARAM_SHOW)
	{
		//�˹�ȥ��BARWIN
		GuiWindowDec(&BAR_WINDOW);
		Touch_Adj=TRUE;
	}
}

//DX_WIDGET* const TOUCH_ADJ_WINDOW_CHILD[]={};


static EPRO __ext_pro_touch_adj_window;
static FNO __p_focus_no_touch_adj_window;
DX_WINDOW TOUCH_ADJ_WINDOW={
	TOUCH_ADJ_WIN_X,TOUCH_ADJ_WIN_Y,TOUCH_ADJ_WIN_L,TOUCH_ADJ_WIN_H,
	BASE_PRO_NOT_FILL,					//��������-�����Զ����ɫ,�������˹����
	&__ext_pro_touch_adj_window,		//��չ����
	NULL,								// ����������
	TouchAdjWin,
	WHITE,								//���ڱ���ɫ
	NULL,								//�ӿؼ���
	0,									//�ؼ���
	&__p_focus_no_touch_adj_window,		//�����

};


/////////////////////////////////////////////////////////////////////////////
// 	ʱ���趨
////////////////////////////////////////////////////////////////////////////
#define EDIT_SET_YEAR_X			0	
#define EDIT_SET_MONTH_X		32
#define EDIT_SET_DAY_X			64
#define EDIT_SET_HOUR_X			96
#define EDIT_SET_MINUTE_X		128
#define BUTTON_SET_OK_X			160

#define EDIT_SET_YEAR_Y			64
#define EDIT_SET_MONTH_Y		64
#define EDIT_SET_DAY_Y			64
#define EDIT_SET_HOUR_Y			64
#define EDIT_SET_MINUTE_Y		64
#define BUTTON_SET_OK_Y			60

#define EDIT_SET_YEAR_L			22
#define EDIT_SET_MONTH_L		22
#define EDIT_SET_DAY_L			22
#define EDIT_SET_HOUR_L			22
#define EDIT_SET_MINUTE_L		22

#define EDIT_SET_YEAR_H			24
#define EDIT_SET_MONTH_H		24
#define EDIT_SET_DAY_H			24
#define EDIT_SET_HOUR_H			24
#define EDIT_SET_MINUTE_H		24

#define BUTTON_SET_OK_L	40
#define BUTTON_SET_OK_H	32


DX_EDIT SET_TIME_WIN_EDIT_YEAR;
DX_EDIT SET_TIME_WIN_EDIT_MONTH;
DX_EDIT SET_TIME_WIN_EDIT_DAY;
DX_EDIT SET_TIME_WIN_EDIT_HOUR;
DX_EDIT SET_TIME_WIN_EDIT_MINUTE;

void RspButtonSetOk(u8 param)
{
	T_STRUCT time;
	if(param==WIDGET_PARAM_ENTER)
	{
		//����ʱ��
		time.year=GetEditTmp(&SET_TIME_WIN_EDIT_YEAR); 
		time.month=GetEditTmp(&SET_TIME_WIN_EDIT_MONTH);
		time.day=GetEditTmp(&SET_TIME_WIN_EDIT_DAY);   
		time.hour=GetEditTmp(&SET_TIME_WIN_EDIT_HOUR); 
		time.minute=GetEditTmp(&SET_TIME_WIN_EDIT_MINUTE);
		time.sec=0;
		time.date=GetDate(&time);
		SetRTCTime(&time);

		//ˢ��һ����������ʱ��
		SetRTCBar(&time);

		//����MAIN_WIN
		GuiWindowDraw(&MAIN_WINDOW);
	}
}

void SetTimeWin(u8 param)
{
	T_STRUCT time;
	if(param==WIDGET_PARAM_SHOW)
	{
		TFTShowString16(0,40,"ʱ���趨:",BLACK,0);
		TFTShowString16(24,68,"/   /   :   :",BLACK,0);

		//��ȡʱ��
		GetRTCTime(&time);

		//��ֵ�������ؼ���
		SetEditTmp(&SET_TIME_WIN_EDIT_YEAR,time.year);
		SetEditTmp(&SET_TIME_WIN_EDIT_MONTH,time.month);
		SetEditTmp(&SET_TIME_WIN_EDIT_DAY,time.day);
		SetEditTmp(&SET_TIME_WIN_EDIT_HOUR,time.hour);
		SetEditTmp(&SET_TIME_WIN_EDIT_MINUTE,time.minute);

	
	}
}

//EDIT-������
u8  __ext_pro_edit_year;
u8  __self_pro_edit_year;
s32 __tmp_edit_year;
DX_EDIT SET_TIME_WIN_EDIT_YEAR={
/*����ߴ�*/    EDIT_SET_YEAR_X,EDIT_SET_YEAR_Y,EDIT_SET_YEAR_L,EDIT_SET_YEAR_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_edit_year,
/*������_*/     &SET_TIME_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_edit_year,
/*��ʱֵ*/		&__tmp_edit_year,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		99,
/*��Сֵ*/		0,
};

//EDIT-������
u8  __ext_pro_edit_month;
u8  __self_pro_edit_month;
s32 __tmp_edit_month;
DX_EDIT SET_TIME_WIN_EDIT_MONTH={
/*����ߴ�*/    EDIT_SET_MONTH_X,EDIT_SET_MONTH_Y,EDIT_SET_MONTH_L,EDIT_SET_MONTH_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_edit_month,
/*������_*/     &SET_TIME_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_edit_month,
/*��ʱֵ*/		&__tmp_edit_month,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		12,
/*��Сֵ*/		1,
};

//EDIT-������
u8  __ext_pro_edit_day;
u8  __self_pro_edit_day;
s32 __tmp_edit_day;
DX_EDIT SET_TIME_WIN_EDIT_DAY={
/*����ߴ�*/    EDIT_SET_DAY_X,EDIT_SET_DAY_Y,EDIT_SET_DAY_L,EDIT_SET_DAY_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_edit_day,
/*������_*/     &SET_TIME_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_edit_day,
/*��ʱֵ*/		&__tmp_edit_day,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		31,
/*��Сֵ*/		1,
};

//EDIT-����ʱ
u8  __ext_pro_edit_hour;
u8  __self_pro_edit_hour;
s32 __tmp_edit_hour;
DX_EDIT SET_TIME_WIN_EDIT_HOUR={
/*����ߴ�*/    EDIT_SET_HOUR_X,EDIT_SET_HOUR_Y,EDIT_SET_HOUR_L,EDIT_SET_HOUR_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_edit_hour,
/*������_*/     &SET_TIME_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_edit_hour,
/*��ʱֵ*/		&__tmp_edit_hour,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		23,
/*��Сֵ*/		0,
};

//EDIT-���÷�
u8  __ext_pro_edit_minute;
u8  __self_pro_edit_minute;
s32 __tmp_edit_minute;
DX_EDIT SET_TIME_WIN_EDIT_MINUTE={
/*����ߴ�*/    EDIT_SET_MINUTE_X,EDIT_SET_MINUTE_Y,EDIT_SET_MINUTE_L,EDIT_SET_MINUTE_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*��չ����*/    &__ext_pro_edit_minute,
/*������_*/     &SET_TIME_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_edit_minute,
/*��ʱֵ*/		&__tmp_edit_minute,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		59,
/*��Сֵ*/		0,
};

//BUTTON-����OK
static u8 __ext_pro_button_set_ok;
static u8 __self_pro_button_set_ok;
DX_BUTTON MAIN_WIN_BUTTON_SET_OK={
/*����ߴ�*/	BUTTON_SET_OK_X,BUTTON_SET_OK_Y,BUTTON_SET_OK_L,BUTTON_SET_OK_H,
/*��������*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*��չ����*/	&__ext_pro_button_set_ok,
/*������_*/		&SET_TIME_WINDOW,
/*��Ӧ����*/    RspButtonSetOk,
/*��ɫ*/		0,
/*��������*/	&__self_pro_button_set_ok,
/*�ַ���*/		"ȷ��",
};

DX_WIDGET* const SET_TIME_WINDOW_CHILD[]={
	(&SET_TIME_WIN_EDIT_YEAR.widget),
	(&SET_TIME_WIN_EDIT_MONTH.widget),
	(&SET_TIME_WIN_EDIT_DAY.widget),
	(&SET_TIME_WIN_EDIT_HOUR.widget),
	(&SET_TIME_WIN_EDIT_MINUTE.widget),
	(&MAIN_WIN_BUTTON_SET_OK.widget),

};


static EPRO __ext_pro_set_time_window;
static FNO __p_focus_no_set_time_window;
DX_WINDOW SET_TIME_WINDOW={
	SET_TIME_WIN_X,SET_TIME_WIN_Y,SET_TIME_WIN_L,SET_TIME_WIN_H,
	0,								//��������
	&__ext_pro_set_time_window,		//��չ����
	NULL,							// ����������
	SetTimeWin,
	GRAY1,							//���ڱ���ɫ
	SET_TIME_WINDOW_CHILD,			//�ӿؼ���
	sizeof(SET_TIME_WINDOW_CHILD)/4,//�ؼ���
	&__p_focus_no_set_time_window,	//�����

};

/////////////////////////////////////////////////////////////////////
// 		���������Խ���
/////////////////////////////////////////////////////////////////////
#define TOUCH_TEST_EDIT_LOCX_X		72
#define TOUCH_TEST_EDIT_LOCX_Y		88
#define TOUCH_TEST_EDIT_LOCX_L		24
#define TOUCH_TEST_EDIT_LOCX_H		16

#define TOUCH_TEST_EDIT_LOCY_X		144
#define TOUCH_TEST_EDIT_LOCY_Y		88
#define TOUCH_TEST_EDIT_LOCY_L		24
#define TOUCH_TEST_EDIT_LOCY_H		16

#define TOUCH_TEST_BUTTON_BACK_X		96
#define TOUCH_TEST_BUTTON_BACK_Y		108
#define TOUCH_TEST_BUTTON_BACK_L		48
#define TOUCH_TEST_BUTTON_BACK_H		24

void TouchTestWin(u8 param)
{
	u16 ad_x,ad_y;
	LOC_X x;
	LOC_Y y;

	if(param==WIDGET_PARAM_SHOW)
	{
		TFTShowString16(TOUCH_TEST_EDIT_LOCX_X-16,TOUCH_TEST_EDIT_LOCX_Y,"X:",BLACK,0);
		TFTShowString16(TOUCH_TEST_EDIT_LOCY_X-16,TOUCH_TEST_EDIT_LOCY_Y,"Y:",BLACK,0);
	}
	else if(param==WIDGET_PARAM_ACTIVE)
	{
		//���´�������
		if(GetTouchHold(&ad_x,&ad_y)==TRUE)
		{
			if(GuiTranTouchXY(ad_x,ad_y,&x,&y)==TRUE)
			{
				SetEditTmp(&TOUCH_TEST_EDIT_LOCX,x);
				SetEditTmp(&TOUCH_TEST_EDIT_LOCY,y);
			}
		}
	}
}

void RspButtonTouchButtonBack(u8 param)
{
	if(param==WIDGET_PARAM_ENTER)
	{
		GuiWindowDraw(&MAIN_WINDOW);
	}
}

//EDIT-LOCX
u8  __ext_pro_edit_locx;
u8  __self_pro_edit_locx;
s32 __tmp_edit_locx;
DX_EDIT TOUCH_TEST_EDIT_LOCX={
/*����ߴ�*/    TOUCH_TEST_EDIT_LOCX_X,TOUCH_TEST_EDIT_LOCX_Y,TOUCH_TEST_EDIT_LOCX_L,TOUCH_TEST_EDIT_LOCX_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_3,
/*��չ����*/    &__ext_pro_edit_locx,
/*������_*/     &TOUCH_TEST_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_edit_locx,
/*��ʱֵ*/		&__tmp_edit_locx,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		999,
/*��Сֵ*/		0,
};

//EDIT-LOCY
u8  __ext_pro_edit_locy;
u8  __self_pro_edit_locy;
s32 __tmp_edit_locy;
DX_EDIT TOUCH_TEST_EDIT_LOCY={
/*����ߴ�*/    TOUCH_TEST_EDIT_LOCY_X,TOUCH_TEST_EDIT_LOCY_Y,TOUCH_TEST_EDIT_LOCY_L,TOUCH_TEST_EDIT_LOCY_H,
/*��������*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_3,
/*��չ����*/    &__ext_pro_edit_locy,
/*������_*/     &TOUCH_TEST_WINDOW,
/*��Ӧ����*/    NULL,
/*��ɫ*/		0,
/*EDIT����*/    &__self_pro_edit_locy,
/*��ʱֵ*/		&__tmp_edit_locy,
/*ʵ��ֵ*/		NULL,
/*���ֵ*/		999,
/*��Сֵ*/		0,
};

//����
//BUTTON-��������
static u8 __ext_pro_button_back;
static u8 __self_pro_button_back;
DX_BUTTON MAIN_WIN_BUTTON_BACK={
/*����ߴ�*/	TOUCH_TEST_BUTTON_BACK_X,TOUCH_TEST_BUTTON_BACK_Y,TOUCH_TEST_BUTTON_BACK_L,TOUCH_TEST_BUTTON_BACK_H,
/*��������*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*��չ����*/	&__ext_pro_button_back,
/*������_*/		&TOUCH_TEST_WINDOW,
/*��Ӧ����*/    RspButtonTouchButtonBack,
/*��ɫ*/		0,
/*��������*/	&__self_pro_button_back,
/*�ַ���*/		"����",
};


DX_WIDGET* const TOUCH_TEST_WINDOW_CHILD[]={
	(&TOUCH_TEST_EDIT_LOCX.widget),
	(&TOUCH_TEST_EDIT_LOCY.widget),
	(&MAIN_WIN_BUTTON_BACK.widget),
};

static EPRO __ext_pro_touch_test_window;
static FNO __p_focus_no_touch_test_window;
DX_WINDOW TOUCH_TEST_WINDOW={
	TOUCH_TEST_WIN_X,TOUCH_TEST_WIN_Y,TOUCH_TEST_WIN_L,TOUCH_TEST_WIN_H,
	0,								//��������
	&__ext_pro_touch_test_window,	//��չ����
	NULL,							// ����������
	TouchTestWin,
	WHITE,							//���ڱ���ɫ
	TOUCH_TEST_WINDOW_CHILD,			//�ӿؼ���
	sizeof(TOUCH_TEST_WINDOW_CHILD)/4,//�ؼ���
	&__p_focus_no_touch_test_window,	//�����

};


void UsrInterface(void)
{
	GuiWindowAdd(&BAR_WINDOW);
    GuiWindowAdd(&STU_WINDOW);
	GuiWindowDraw(&MAIN_WINDOW);
}

void TouchAdjInterface(void)
{
 	GuiWindowDraw(&TOUCH_ADJ_WINDOW);
}