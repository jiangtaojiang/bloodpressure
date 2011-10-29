/**************************************************************
**模块名:GuiData.c
**功能:GUI的用户数据,控件和窗体都在这里实行预定义. 注意某些内部指针指向的内存空间,如不希望外部直接调用.需要定义为static. 
**是否允许用户外部直接调用,看用户的需求. 
**修改记录: 
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


//时间栏窗体
#define BAR_WIN_X			0
#define BAR_WIN_Y			0
#define BAR_WIN_L			240
#define BAR_WIN_H			20
//状态栏窗体
#define STU_WIN_X			0
#define STU_WIN_Y			300
#define STU_WIN_L			240
#define STU_WIN_H			20
//主窗体
#define MAIN_WIN_X			0
#define MAIN_WIN_Y			(BAR_WIN_H)
#define MAIN_WIN_L			240
#define MAIN_WIN_H			(MAX_Y-MAIN_WIN_Y*2)
//触摸屏校正窗体
#define TOUCH_ADJ_WIN_X		0
#define TOUCH_ADJ_WIN_Y		0
#define TOUCH_ADJ_WIN_L		240
#define TOUCH_ADJ_WIN_H		320
//时间校正窗体
#define SET_TIME_WIN_X			0
#define SET_TIME_WIN_Y			(BAR_WIN_H)
#define SET_TIME_WIN_L			240
#define SET_TIME_WIN_H			(MAX_Y-SET_TIME_WIN_Y)
//触摸屏测试窗体
#define TOUCH_TEST_WIN_X		0                       
#define TOUCH_TEST_WIN_Y        (BAR_WIN_H)             
#define TOUCH_TEST_WIN_L        240                     
#define TOUCH_TEST_WIN_H        (MAX_Y-SET_TIME_WIN_Y)  

/////////////////////////////////////////////////////////////////////////////
//BAR_WINDOW控件部分
/////////////////////////////////////////////////////////////////////////////
////时间位置
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


//响应函数
void BarWin(u8 param)
{
	if(param==WIDGET_PARAM_SHOW)
	{
		//TFTShowString16(16,LOC_TIME_Y,"/  /     :  :",WHITE,0);
      TFTShowString16(176,LOC_TIME_Y,"  :  :",WHITE,0);
       TFTShowString16(0,LOC_TIME_Y,"血压采集器",WHITE,0);
	}
}

//年
static EPRO  __ext_pro_time_year;
static SPRO  __self_pro_time_year;
static s32 __tmp_edit_time_year;

DX_EDIT BAR_WIN_TIME_YEAR={
/*坐标尺寸*/    LOC_TIME_YEAR_X,LOC_TIME_Y,TIME_L,TIME_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_time_year,
/*父窗口_*/     &BAR_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		WHITE,
/*EDIT属性*/    &__self_pro_time_year,
/*临时值*/      &__tmp_edit_time_year,
/*实际值*/      NULL,
/*最大值*/      99,
/*最小值*/      0,

};

//月
static EPRO  __ext_pro_time_month;
static SPRO  __self_pro_time_month;
static s32 __tmp_time_month;

DX_EDIT BAR_WIN_TIME_MONTH={
/*坐标尺寸*/    LOC_TIME_MONTH_X,LOC_TIME_Y,TIME_L,TIME_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_time_month,
/*父窗口_*/     &BAR_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		WHITE,
/*EDIT属性*/    &__self_pro_time_month,
/*临时值*/      &__tmp_time_month,
/*实际值*/      NULL,
/*最大值*/      12,
/*最小值*/      1,
};

//日
static EPRO  __ext_pro_time_day;
static SPRO  __self_pro_time_day;
static s32 __tmp_time_day;

DX_EDIT BAR_WIN_TIME_DAY={
/*坐标尺寸*/    LOC_TIME_DAY_X,LOC_TIME_Y,TIME_L,TIME_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_time_day,
/*父窗口_*/     &BAR_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		WHITE,
/*EDIT属性*/    &__self_pro_time_day,
/*临时值*/      &__tmp_time_day,
/*实际值*/      NULL,
/*最大值*/      31,
/*最小值*/      1,
};

//时
static EPRO  __ext_pro_time_hour;
static SPRO  __self_pro_time_hour;
static s32 __tmp_time_hour;

DX_EDIT BAR_WIN_TIME_HOUR={
/*坐标尺寸*/    LOC_TIME_HOUR_X,LOC_TIME_Y,TIME_L,TIME_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_time_hour,
/*父窗口_*/     &BAR_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		WHITE,
/*EDIT属性*/    &__self_pro_time_hour,
/*临时值*/      &__tmp_time_hour,
/*实际值*/      NULL,
/*最大值*/      23,
/*最小值*/      0,
};

//分
static EPRO  __ext_pro_time_minute;
static SPRO  __self_pro_time_minute;
static s32 __tmp_time_minute;

DX_EDIT BAR_WIN_TIME_MINUTE={
/*坐标尺寸*/    LOC_TIME_MINUTE_X,LOC_TIME_Y,TIME_L,TIME_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_time_minute,
/*父窗口_*/     &BAR_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		WHITE,
/*EDIT属性*/    &__self_pro_time_minute,
/*临时值*/      &__tmp_time_minute,
/*实际值*/      NULL,
/*最大值*/      59,
/*最小值*/      0,
};

//秒
static EPRO  __ext_pro_time_second;
static SPRO  __self_pro_time_second;
static s32 __tmp_time_second;

DX_EDIT BAR_WIN_TIME_SECOND={
/*坐标尺寸*/    LOC_TIME_SECOND_X,LOC_TIME_Y,TIME_L,TIME_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_time_second,
/*父窗口_*/     &BAR_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		WHITE,
/*EDIT属性*/    &__self_pro_time_second,
/*临时值*/      &__tmp_time_second,
/*实际值*/      NULL,
/*最大值*/      59,
/*最小值*/      0,
};

//星期
char const* const __str_date_combo[7]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六",};
 
static EPRO __ext_pro_time_date;
static SPRO __self_pro_time_date;
static u8 __tmp_edit_time_date;
u8 Date;
DX_COMBO BAR_WIN_TIME_DATE={
/*坐标尺寸*/	LOC_TIME_DATE_X,LOC_TIME_Y,48,TIME_H,
/*基础属性*/	BASE_PRO_COMBO|BASE_PRO_FONT16|BASE_PRO_COMBO_STYLE_NULL|BASE_PRO_COMBO_TYPE_STRING|BASE_PRO_FORBID_FOCUS,
/*扩展属性*/	&__ext_pro_time_date,
/*父窗口_*/     &BAR_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		WHITE,
/*COMBO属性*/	&__self_pro_time_date,
/*临时值*/		&__tmp_edit_time_date,
/*实际值*/		NULL,
/*元素组*/		(void**)__str_date_combo,
/*最大值*/		7,
/*2D色*/		0,
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
	BAR_WIN_X,BAR_WIN_Y,BAR_WIN_L,BAR_WIN_H,	//窗口坐标与大小
	0,											//基本属性
	&__ext_pro_test_window,						//扩展属性
	NULL,										// 所属父窗口
	BarWin,										//响应函数
	BLUE,										//窗口背景色
	BAR_WINDOW_CHILD,							//子控件组
	sizeof(BAR_WINDOW_CHILD)/4,					//控件数
	&__p_focus_no_test_window,					//焦点号

};

DX_WINDOW STU_WINDOW={
	STU_WIN_X,STU_WIN_Y,STU_WIN_L,STU_WIN_H,	//窗口坐标与大小
	0,											//基本属性
	&__ext_pro_test_window,						//扩展属性
	NULL,										// 所属父窗口
	NULL,//BarWin,										//响应函数
	BLUE,										//窗口背景色
	BAR_WINDOW_CHILD,							//子控件组
	0,//sizeof(BAR_WINDOW_CHILD)/4,					//控件数
	&__p_focus_no_test_window,					//焦点号

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
        TFTShowString16(16,30,"测量时间:",BLUE,0);
		TFTShowString24(16,70,"收缩压",BLUE,0);
		TFTShowString24(16,140,"舒张压",BLUE,0);
		TFTShowString24(16,210,"脉率",BLUE,0);
//        TFTShowString32(16+100,60,"777",BLUE,0);
//        TFTShowString32(16+100,130,"999",BLUE,0);
//        TFTShowString32(16+100,200,"888",BLUE,0);
//		TFTShowString24(50,250,"000",BLUE,0);
//		TFTShowString16(0,64+8,"当前AD:",BLACK,0);

	}
}
//触摸校正
void RspButtonTouchAdj(u8 param)
{
 	if(param==WIDGET_PARAM_ENTER)
	{
		GuiWindowDraw(&TOUCH_ADJ_WINDOW);
	}
}
//时间设定
void RspButtonSetTime(u8 param)
{
 	if(param==WIDGET_PARAM_ENTER)
	{
		GuiWindowDraw(&SET_TIME_WINDOW);
	}
}
//触摸测试
void RspButtonTouchTest(u8 param)
{
	if(param==WIDGET_PARAM_ENTER)
	{
		GuiWindowDraw(&TOUCH_TEST_WINDOW);
	}
}

//CHECK16,24,32-16,24,32的钩钩各一个
u8 __ext_pro_check16;
u8 __self_pro_check16;
bool __tmp_check16=TRUE;
DX_CHECK MAIN_WIN_CHECK16={
/*坐标尺寸*/    CHECK_BOX16_X,CHECK_BOX_Y,0,0,
/*基础属性*/    BASE_PRO_CHECK|BASE_PRO_FONT16|BASE_PRO_CHECK_TYPE_HOOK,
/*扩展属性*/    &__ext_pro_check16,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*CHECK属性*/   &__self_pro_check16,
/*临时值*/		&__tmp_check16,
/*实际值*/		NULL,
/*项目组*/		NULL,
/*2D色*/		0,
};

u8 __ext_pro_check24;
u8 __self_pro_check24;
bool __tmp_check24=TRUE;
DX_CHECK MAIN_WIN_CHECK24={
/*坐标尺寸*/    CHECK_BOX24_X,CHECK_BOX_Y,0,0,
/*基础属性*/    BASE_PRO_CHECK|BASE_PRO_FONT24|BASE_PRO_CHECK_TYPE_HOOK,
/*扩展属性*/    &__ext_pro_check24,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*CHECK属性*/   &__self_pro_check24,
/*临时值*/		&__tmp_check24,
/*实际值*/		NULL,
/*项目组*/		NULL,
/*2D色*/		0,
};

u8 __ext_pro_check32;
u8 __self_pro_check32;
bool __tmp_check32=TRUE;
DX_CHECK MAIN_WIN_CHECK32={
/*坐标尺寸*/    CHECK_BOX32_X,CHECK_BOX_Y,0,0,
/*基础属性*/    BASE_PRO_CHECK|BASE_PRO_FONT32|BASE_PRO_CHECK_TYPE_HOOK,
/*扩展属性*/    &__ext_pro_check32,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*CHECK属性*/   &__self_pro_check32,
/*临时值*/		&__tmp_check32,
/*实际值*/		NULL,
/*项目组*/		NULL,
/*2D色*/		0,
};

//PROGRESS--显示AD
u8 __ext_pro_progress;
u8 __self_pro_progress;
s32 __tmp_progress;

DX_PROGRESS MAIN_WIN_PROGRESS={
/*坐标尺寸*/    PROGRESS_AD_X,PROGRESS_AD_Y,PROGRESS_AD_L,PROGRESS_AD_H,
/*基础属性*/    BASE_PRO_PROGRESS|BASE_PRO_PROGRESS_TYPE_NUM|BASE_PRO_PROGRESS_STYLE_3D/*|BASE_PRO_FORBID_FOCUS*/,
/*扩展属性*/    &__ext_pro_progress,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*CHECK属性*/   &__self_pro_progress,
/*临时值*/		&__tmp_progress,
/*实际值*/		NULL,
/*最大值*/		4095,
/*最小值*/		0,
/*底色*/		WHITE,
/*面色*/		BLUE,
};

//CHECK--STRING类型 检测U盘状态
u8 __ext_pro_check_udisk;
u8 __self_pro_check_udisk;
bool __tmp_check_udisk=FALSE;
const char * const __string_check_udisk[2]={"插入","无效"};
DX_CHECK MAIN_WIN_CHECK_UDISK={
/*坐标尺寸*/    CHECK_UDISK_X,CHECK_UDISK_Y,CHECK_UDISK_L,CHECK_UDISK_H,
/*基础属性*/    BASE_PRO_CHECK|BASE_PRO_CHECK_TYPE_STRING|BASE_PRO_FORBID_FOCUS|BASE_PRO_CHECK_STYLE_2D,
/*扩展属性*/    &__ext_pro_check_udisk,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*CHECK属性*/   &__self_pro_check_udisk,
/*临时值*/		&__tmp_check_udisk,
/*实际值*/		NULL,
/*项目组*/		(void**)__string_check_udisk,
/*2D色*/		GREEN,
};


//CHECK--BMP类型 网络可改变的图标 
u8 __ext_pro_check_net;
u8 __self_pro_check_net;
bool __tmp_check_net=FALSE;
const COLOR * const __bmp_check_net[2]={Bmp_Check1,Bmp_Check2};
DX_CHECK MAIN_WIN_CHECK_NET={
/*坐标尺寸*/    CHECK_NET_X,CHECK_NET_Y,CHECK_NET_L,CHECK_NET_H,
/*基础属性*/    BASE_PRO_CHECK|BASE_PRO_CHECK_TYPE_BMP|BASE_PRO_FORBID_FOCUS,
/*扩展属性*/    &__ext_pro_check_net,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*CHECK属性*/   &__self_pro_check_net,
/*临时值*/		&__tmp_check_net,
/*实际值*/		NULL,
/*项目组*/		(void**)__bmp_check_net,
/*2D色*/		0,
};

//BUTTON-可以改变LED灯的状态
static u8 __ext_pro_button_led;
static u8 __self_pro_button_led;
DX_BUTTON MAIN_WIN_BUTTON_LED={
/*坐标尺寸*/	BUTTON_LED_X,BUTTON_LED_Y,BUTTON_LED_L,BUTTON_LED_H,
/*基础属性*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*扩展属性*/	&__ext_pro_button_led,
/*父窗口_*/		&MAIN_WINDOW,
/*响应函数*/    RspButtonLed,
/*颜色*/		0,
/*按键属性*/	&__self_pro_button_led,
/*字符串*/		"测试键",
};

//EDIT-0P整数,初始化位一个负数
u8  __ext_pro_edit;
u8  __self_pro_edit;
s32 __tmp_edit=-1024;
s32 Data_Edit;		//此变量可外部引用
DX_EDIT MAIN_WIN_EDIT={
/*坐标尺寸*/    EDIT_X,EDIT_Y,EDIT_L,EDIT_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16,
/*扩展属性*/    &__ext_pro_edit,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_edit,
/*临时值*/		&__tmp_edit,
/*实际值*/		&Data_Edit,
/*最大值*/		99999,
/*最小值*/		-9999,
};

//BUTTON-触摸屏校正界面
static u8 __ext_pro_button_touch_adj;
static u8 __self_pro_button_touch_adj;
DX_BUTTON MAIN_WIN_BUTTON_TOUCH_ADJ={
/*坐标尺寸*/	BUTTON_TOUCH_ADJ_X,BUTTON_TOUCH_ADJ_Y,BUTTON_TOUCH_ADJ_L,BUTTON_TOUCH_ADJ_H,
/*基础属性*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*扩展属性*/	&__ext_pro_button_touch_adj,
/*父窗口_*/		&MAIN_WINDOW,
/*响应函数*/    RspButtonTouchAdj,
/*颜色*/		0,
/*按键属性*/	&__self_pro_button_touch_adj,
/*字符串*/		"触摸校正",
};

//BUTTON-时间设定
static u8 __ext_pro_button_set_time;
static u8 __self_pro_button_set_time;
DX_BUTTON MAIN_WIN_BUTTON_SET_TIME={
/*坐标尺寸*/	BUTTON_SET_TIME_X,BUTTON_SET_TIME_Y,BUTTON_SET_TIME_L,BUTTON_SET_TIME_H,
/*基础属性*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*扩展属性*/	&__ext_pro_button_set_time,
/*父窗口_*/		&MAIN_WINDOW,
/*响应函数*/    RspButtonSetTime,
/*颜色*/		0,
/*按键属性*/	&__self_pro_button_set_time,
/*字符串*/		"时间设定",
};

//BUTTON-触摸测试
static u8 __ext_pro_button_touch_test;
static u8 __self_pro_button_touch_test;
DX_BUTTON MAIN_WIN_BUTTON_TOUCH_TEST={
/*坐标尺寸*/	BUTTON_TOUCH_TEST_X,BUTTON_TOUCH_TEST_Y,BUTTON_TOUCH_TEST_L,BUTTON_TOUCH_TEST_H,
/*基础属性*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*扩展属性*/	&__ext_pro_button_touch_test,
/*父窗口_*/		&MAIN_WINDOW,
/*响应函数*/    RspButtonTouchTest,
/*颜色*/		0,
/*按键属性*/	&__self_pro_button_touch_test,
/*字符串*/		"触摸测试",
};
//ADC show
static EPRO  __ext_pro_adc;
u8  __self_pro_adc;
s32 __tmp_adc = 0;
s32 Data_Edit_adc;		//此变量可外部引用
DX_EDIT MAIN_WIN_ADC={
/*坐标尺寸*/    ADC_X,ADC_Y,ADC_L,ADC_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT24|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*扩展属性*/    &__ext_pro_adc,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_adc,
/*临时值*/		&__tmp_adc,
/*实际值*/		NULL,
/*最大值*/		4096,
/*最小值*/		0,
};
//ADC1 show
static EPRO  __ext_pro_adc1;
u8  __self_pro_adc1;
s32 __tmp_adc1 = 0;
s32 Data_Edit_adc1;		//此变量可外部引用
DX_EDIT MAIN_WIN_ADC1={
/*坐标尺寸*/    ADC1_X,ADC1_Y,ADC1_L,ADC1_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT24|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*扩展属性*/    &__ext_pro_adc1,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_adc1,
/*临时值*/		&__tmp_adc1,
/*实际值*/		NULL,
/*最大值*/		4096,
/*最小值*/		0,
};
//VBAT show
static EPRO  __ext_pro_vbat;
u8  __self_pro_vbat;
s32 __tmp_vbat = 0;
s32 Data_Edit_vbat;		//此变量可外部引用
DX_EDIT MAIN_WIN_VBAT={
/*坐标尺寸*/    VBAT_X,VBAT_Y,VBAT_L,VBAT_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT24|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*扩展属性*/    &__ext_pro_vbat,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_vbat,
/*临时值*/		&__tmp_vbat,
/*实际值*/		NULL,
/*最大值*/		330,
/*最小值*/		0,
};
//SBP show 收缩压
static EPRO  __ext_pro_sbp;
u8  __self_pro_sbp;
s32 __tmp_sbp = 300;
s32 Data_Edit_sbp;		//此变量可外部引用
DX_EDIT MAIN_WIN_SBP={
/*坐标尺寸*/    SBP_X,SBP_Y,SBP_L,SBP_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT32|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*扩展属性*/    &__ext_pro_sbp,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		BLUE,
/*EDIT属性*/    &__self_pro_sbp,
/*临时值*/		&__tmp_sbp,
/*实际值*/		NULL,
/*最大值*/		300,
/*最小值*/		0,
};
//DBP show 舒张压
static EPRO  __ext_pro_dbp;
u8  __self_pro_dbp;
s32 __tmp_dbp = 200;
s32 Data_Edit_dbp;		//此变量可外部引用
DX_EDIT MAIN_WIN_DBP={
/*坐标尺寸*/    DBP_X,DBP_Y,DBP_L,DBP_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT32|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*扩展属性*/    &__ext_pro_dbp,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		BLUE,
/*EDIT属性*/    &__self_pro_dbp,
/*临时值*/		&__tmp_dbp,
/*实际值*/		NULL,
/*最大值*/		300,
/*最小值*/		0,
};
//PR show 脉率
static EPRO  __ext_pro_pr;
u8  __self_pro_pr;
s32 __tmp_pr = 100;
s32 Data_Edit_pr;		//此变量可外部引用
DX_EDIT MAIN_WIN_PR={
/*坐标尺寸*/    PR_X,PR_Y,PR_L,PR_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT32|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_BCD_NULL,
/*扩展属性*/    &__ext_pro_pr,
/*父窗口_*/     &MAIN_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		BLUE,
/*EDIT属性*/    &__self_pro_pr,
/*临时值*/		&__tmp_pr,
/*实际值*/		NULL,
/*最大值*/		180,
/*最小值*/		0,
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
	MAIN_WIN_X,MAIN_WIN_Y,MAIN_WIN_L,MAIN_WIN_H,						//窗口坐标与大小
	0,									//基本属性
	&__ext_pro_main_window,				//扩展属性
	NULL,								// 所属父窗口
	MainWin,
	WHITE,								//窗口背景色
	MAIN_WINDOW_CHILD,					//子控件组
	sizeof(MAIN_WINDOW_CHILD)/4,		//控件数
	&__p_focus_no_main_window,			//焦点号

};

/////////////////////////////////////////////////////////////////////
// 	触摸屏校正界面
////////////////////////////////////////////////////////////////////
extern bool Touch_Adj;
void TouchAdjWin(u8 param)
{
	if(param==WIDGET_PARAM_SHOW)
	{
		//人工去除BARWIN
		GuiWindowDec(&BAR_WINDOW);
		Touch_Adj=TRUE;
	}
}

//DX_WIDGET* const TOUCH_ADJ_WINDOW_CHILD[]={};


static EPRO __ext_pro_touch_adj_window;
static FNO __p_focus_no_touch_adj_window;
DX_WINDOW TOUCH_ADJ_WINDOW={
	TOUCH_ADJ_WIN_X,TOUCH_ADJ_WIN_Y,TOUCH_ADJ_WIN_L,TOUCH_ADJ_WIN_H,
	BASE_PRO_NOT_FILL,					//基本属性-不填自动填充色,而是又人工填充
	&__ext_pro_touch_adj_window,		//扩展属性
	NULL,								// 所属父窗口
	TouchAdjWin,
	WHITE,								//窗口背景色
	NULL,								//子控件组
	0,									//控件数
	&__p_focus_no_touch_adj_window,		//焦点号

};


/////////////////////////////////////////////////////////////////////////////
// 	时间设定
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
		//保存时间
		time.year=GetEditTmp(&SET_TIME_WIN_EDIT_YEAR); 
		time.month=GetEditTmp(&SET_TIME_WIN_EDIT_MONTH);
		time.day=GetEditTmp(&SET_TIME_WIN_EDIT_DAY);   
		time.hour=GetEditTmp(&SET_TIME_WIN_EDIT_HOUR); 
		time.minute=GetEditTmp(&SET_TIME_WIN_EDIT_MINUTE);
		time.sec=0;
		time.date=GetDate(&time);
		SetRTCTime(&time);

		//刷新一下任务栏的时间
		SetRTCBar(&time);

		//返回MAIN_WIN
		GuiWindowDraw(&MAIN_WINDOW);
	}
}

void SetTimeWin(u8 param)
{
	T_STRUCT time;
	if(param==WIDGET_PARAM_SHOW)
	{
		TFTShowString16(0,40,"时间设定:",BLACK,0);
		TFTShowString16(24,68,"/   /   :   :",BLACK,0);

		//获取时间
		GetRTCTime(&time);

		//赋值到各个控件上
		SetEditTmp(&SET_TIME_WIN_EDIT_YEAR,time.year);
		SetEditTmp(&SET_TIME_WIN_EDIT_MONTH,time.month);
		SetEditTmp(&SET_TIME_WIN_EDIT_DAY,time.day);
		SetEditTmp(&SET_TIME_WIN_EDIT_HOUR,time.hour);
		SetEditTmp(&SET_TIME_WIN_EDIT_MINUTE,time.minute);

	
	}
}

//EDIT-设置年
u8  __ext_pro_edit_year;
u8  __self_pro_edit_year;
s32 __tmp_edit_year;
DX_EDIT SET_TIME_WIN_EDIT_YEAR={
/*坐标尺寸*/    EDIT_SET_YEAR_X,EDIT_SET_YEAR_Y,EDIT_SET_YEAR_L,EDIT_SET_YEAR_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_edit_year,
/*父窗口_*/     &SET_TIME_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_edit_year,
/*临时值*/		&__tmp_edit_year,
/*实际值*/		NULL,
/*最大值*/		99,
/*最小值*/		0,
};

//EDIT-设置月
u8  __ext_pro_edit_month;
u8  __self_pro_edit_month;
s32 __tmp_edit_month;
DX_EDIT SET_TIME_WIN_EDIT_MONTH={
/*坐标尺寸*/    EDIT_SET_MONTH_X,EDIT_SET_MONTH_Y,EDIT_SET_MONTH_L,EDIT_SET_MONTH_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_edit_month,
/*父窗口_*/     &SET_TIME_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_edit_month,
/*临时值*/		&__tmp_edit_month,
/*实际值*/		NULL,
/*最大值*/		12,
/*最小值*/		1,
};

//EDIT-设置日
u8  __ext_pro_edit_day;
u8  __self_pro_edit_day;
s32 __tmp_edit_day;
DX_EDIT SET_TIME_WIN_EDIT_DAY={
/*坐标尺寸*/    EDIT_SET_DAY_X,EDIT_SET_DAY_Y,EDIT_SET_DAY_L,EDIT_SET_DAY_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_edit_day,
/*父窗口_*/     &SET_TIME_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_edit_day,
/*临时值*/		&__tmp_edit_day,
/*实际值*/		NULL,
/*最大值*/		31,
/*最小值*/		1,
};

//EDIT-设置时
u8  __ext_pro_edit_hour;
u8  __self_pro_edit_hour;
s32 __tmp_edit_hour;
DX_EDIT SET_TIME_WIN_EDIT_HOUR={
/*坐标尺寸*/    EDIT_SET_HOUR_X,EDIT_SET_HOUR_Y,EDIT_SET_HOUR_L,EDIT_SET_HOUR_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_edit_hour,
/*父窗口_*/     &SET_TIME_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_edit_hour,
/*临时值*/		&__tmp_edit_hour,
/*实际值*/		NULL,
/*最大值*/		23,
/*最小值*/		0,
};

//EDIT-设置分
u8  __ext_pro_edit_minute;
u8  __self_pro_edit_minute;
s32 __tmp_edit_minute;
DX_EDIT SET_TIME_WIN_EDIT_MINUTE={
/*坐标尺寸*/    EDIT_SET_MINUTE_X,EDIT_SET_MINUTE_Y,EDIT_SET_MINUTE_L,EDIT_SET_MINUTE_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_2,
/*扩展属性*/    &__ext_pro_edit_minute,
/*父窗口_*/     &SET_TIME_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_edit_minute,
/*临时值*/		&__tmp_edit_minute,
/*实际值*/		NULL,
/*最大值*/		59,
/*最小值*/		0,
};

//BUTTON-设置OK
static u8 __ext_pro_button_set_ok;
static u8 __self_pro_button_set_ok;
DX_BUTTON MAIN_WIN_BUTTON_SET_OK={
/*坐标尺寸*/	BUTTON_SET_OK_X,BUTTON_SET_OK_Y,BUTTON_SET_OK_L,BUTTON_SET_OK_H,
/*基础属性*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*扩展属性*/	&__ext_pro_button_set_ok,
/*父窗口_*/		&SET_TIME_WINDOW,
/*响应函数*/    RspButtonSetOk,
/*颜色*/		0,
/*按键属性*/	&__self_pro_button_set_ok,
/*字符串*/		"确认",
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
	0,								//基本属性
	&__ext_pro_set_time_window,		//扩展属性
	NULL,							// 所属父窗口
	SetTimeWin,
	GRAY1,							//窗口背景色
	SET_TIME_WINDOW_CHILD,			//子控件组
	sizeof(SET_TIME_WINDOW_CHILD)/4,//控件数
	&__p_focus_no_set_time_window,	//焦点号

};

/////////////////////////////////////////////////////////////////////
// 		触摸屏测试界面
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
		//更新触摸坐标
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
/*坐标尺寸*/    TOUCH_TEST_EDIT_LOCX_X,TOUCH_TEST_EDIT_LOCX_Y,TOUCH_TEST_EDIT_LOCX_L,TOUCH_TEST_EDIT_LOCX_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_3,
/*扩展属性*/    &__ext_pro_edit_locx,
/*父窗口_*/     &TOUCH_TEST_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_edit_locx,
/*临时值*/		&__tmp_edit_locx,
/*实际值*/		NULL,
/*最大值*/		999,
/*最小值*/		0,
};

//EDIT-LOCY
u8  __ext_pro_edit_locy;
u8  __self_pro_edit_locy;
s32 __tmp_edit_locy;
DX_EDIT TOUCH_TEST_EDIT_LOCY={
/*坐标尺寸*/    TOUCH_TEST_EDIT_LOCY_X,TOUCH_TEST_EDIT_LOCY_Y,TOUCH_TEST_EDIT_LOCY_L,TOUCH_TEST_EDIT_LOCY_H,
/*基础属性*/    BASE_PRO_EDIT|BASE_PRO_FONT16|BASE_PRO_EDIT_STYLE_NULL|BASE_PRO_FORBID_FOCUS|BASE_PRO_EDIT_TYPE_0P|BASE_PRO_EDIT_BCD_3,
/*扩展属性*/    &__ext_pro_edit_locy,
/*父窗口_*/     &TOUCH_TEST_WINDOW,
/*响应函数*/    NULL,
/*颜色*/		0,
/*EDIT属性*/    &__self_pro_edit_locy,
/*临时值*/		&__tmp_edit_locy,
/*实际值*/		NULL,
/*最大值*/		999,
/*最小值*/		0,
};

//按键
//BUTTON-触摸测试
static u8 __ext_pro_button_back;
static u8 __self_pro_button_back;
DX_BUTTON MAIN_WIN_BUTTON_BACK={
/*坐标尺寸*/	TOUCH_TEST_BUTTON_BACK_X,TOUCH_TEST_BUTTON_BACK_Y,TOUCH_TEST_BUTTON_BACK_L,TOUCH_TEST_BUTTON_BACK_H,
/*基础属性*/	BASE_PRO_BUTTON|BASE_PRO_FONT16,
/*扩展属性*/	&__ext_pro_button_back,
/*父窗口_*/		&TOUCH_TEST_WINDOW,
/*响应函数*/    RspButtonTouchButtonBack,
/*颜色*/		0,
/*按键属性*/	&__self_pro_button_back,
/*字符串*/		"返回",
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
	0,								//基本属性
	&__ext_pro_touch_test_window,	//扩展属性
	NULL,							// 所属父窗口
	TouchTestWin,
	WHITE,							//窗口背景色
	TOUCH_TEST_WINDOW_CHILD,			//子控件组
	sizeof(TOUCH_TEST_WINDOW_CHILD)/4,//控件数
	&__p_focus_no_touch_test_window,	//焦点号

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