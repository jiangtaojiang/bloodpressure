/**************************************************************
	配色方案
***************************************************************/

#ifndef COLOR_H
#define COLOR_H

//#pragma diag_suppress 1             //去除第1号报警提示
//#pragma diag_suppress 47            //去除第47号报警提示

//基础色
#define RED			0xf800
#define GREEN		0x7e0
#define BLUE		0x001f
#define BLACK		0x0000
#define YELLOW		0xffe0
#define WHITE		0xffff

//其他色种
#define CYAN		0x07ff	//青色
#define RED_BRIGHT	0xf810	//亮红
#define GRAY1		0x8410//0xD699	//浅灰,用于WIN表面
#define GRAY2		0x4208//0x8410	//中灰,用于框中层
#define GRAY3		0x00  //0x4208	//深灰,用于框外围暂时只能是黑色

//取自SAMPOO的色素

#define GRAY_0    0x18c3   //灰10%
#define GRAY_1    0x31a6   //灰20%
#define GRAY_2    0x528a   //灰30%
#define GRAY_3    0x6b6d   //灰40%
#define GRAY_4    0x8c51   //灰50%
#define GRAY_5    0xa534   //灰60%
#define GRAY_6    0xbdd7   //灰70%
#define GRAY_7    0xce79   //灰80%
#define GRAY_8    0xe71c   //灰90%
#define GRAY_9    0xf79e   //灰100%

//----------------------------------------------
#define RED_0   0x1800   //红色10%
#define RED_1   0x3000   //红色20%
#define RED_2   0x4800   //红色30%
#define RED_3   0x6000   //红色40%
#define RED_4   0x7800   //红色50%
#define RED_5   0x9000   //红色60%
#define RED_6   0xa800   //红色70%
#define RED_7   0xc800   //红色80%
#define RED_8   0xe000   //红色90%
#define RED_9   0xf800   //红色100%
//-------------------------------------------
#define GREEN_0 0x00c0   //绿色10%
#define GREEN_1 0x0180   //绿色20%
#define GREEN_2 0x0240   //绿色30%
#define GREEN_3 0x0320   //绿色40%
#define GREEN_4 0x03e0   //绿色50%
#define GREEN_5 0x04a0   //绿色60%
#define GREEN_6 0x0560   //绿色70%
#define GREEN_7 0x0640   //绿色80%
#define GREEN_8 0x0700   //绿色90%
#define GREEN_9 0x07e0   //绿色100%
//-----------------------------------------
#define BLUE_0  0x0003   //蓝色10%
#define BLUE_1  0x0006   //蓝色20%
#define BLUE_2  0x0009   //蓝色30%
#define BLUE_3  0x000c   //蓝色40%
#define BLUE_4  0x000f   //蓝色50%
#define BLUE_5  0x0012   //蓝色60%
#define BLUE_6  0x0016   //蓝色70%
#define BLUE_7  0x0019   //蓝色80%
#define BLUE_8  0x001c   //蓝色90%
#define BLUE_9  0x001f   //蓝色100%
//-------------------------------------------
#define Green_kb  0x05ed    //kaba
//-------------------------------------------
#define REDBLUE_1 0x284e  //紫色1
#define REDBLUE_2 0x3892  //紫色2
#define REDBLUE_3 0x4915  //紫色3
#define REDBLUE_4 0x5157  //紫色4
#define REDBLUE_5 0x6199  //紫色5
#define REDBLUE_6 0x725b  //紫色6
#define REDBLUE_7 0x8b1d  //紫色7
#define REDBLUE_8 0x935d  //紫色8
#define REDBLUE_9 0xac3e  //紫色9
//-------------------------------------------
#define PING_0    0x2804  //紫红色10%
#define PING_1    0x4006  //紫红色20%
#define PING_2    0x5808  //紫红色30%
#define PING_3    0x700a  //紫红色40%
#define PING_4    0x880c  //紫红色50%
#define PING_5    0xa02f  //紫红色60%
#define PING_6    0xb811  //紫红色70%
#define PING_7    0xd033  //紫红色80%
#define PING_8    0xe855  //紫红色90%
#define PING_9    0xf837  //紫红色100%
//-------------------------------------------
#define YELLOW_1  0x41e0  //黄色1
#define YELLOW_2  0x6ae0  //黄色2
#define YELLOW_3  0x8be1  //黄色3
#define YELLOW_4  0xa4a1  //黄色4
#define YELLOW_5  0xbd42  //黄色5
#define YELLOW_6  0xcdc4  //黄色6
#define YELLOW_7  0xde47  //黄色7
#define YELLOW_8  0xeeea  //黄色8
#define YELLOW_9  0xf752  //黄色9
//-----------------------------------------ST蓝有点灰
#define BLUE_ST_0 0x00a2   //亮蓝色10%
#define BLUE_ST_1 0x0145   //亮蓝色20%
#define BLUE_ST_2 0x0229   //亮蓝色30%
#define BLUE_ST_3 0x02cc   //亮蓝色40%
#define BLUE_ST_4 0x03b0   //亮蓝色50%
#define BLUE_ST_5 0x0473   //亮蓝色60%
#define BLUE_ST_6 0x0536   //亮蓝色70%
#define BLUE_ST_7 0x05d9   //亮蓝色80%
#define BLUE_ST_8 0x06bc   //亮蓝色90%
#define BLUE_ST_9 0x075f   //亮蓝色100%
//-----------------------------------------
#define BLUE_AT_0 0x00e3   //亮蓝色10%
#define BLUE_AT_1 0x01a6   //亮蓝色20%
#define BLUE_AT_2 0x0249   //亮蓝色30%
#define BLUE_AT_3 0x02eb   //亮蓝色40%
#define BLUE_AT_4 0x03ae   //亮蓝色50%
#define BLUE_AT_5 0x0491   //亮蓝色60%
#define BLUE_AT_6 0x0534   //亮蓝色70%
#define BLUE_AT_7 0x05d7   //亮蓝色80%
#define BLUE_AT_8 0x073c   //亮蓝色90%
#define BLUE_AT_9 0x07be   //亮蓝色100%
//-----------------------------------------
#define BLUE_BT_0 0x00c5   //亮蓝色10%
#define BLUE_BT_1 0x0147   //亮蓝色20%
#define BLUE_BT_2 0x01aa   //亮蓝色30%
#define BLUE_BT_3 0x022d   //亮蓝色40%
#define BLUE_BT_4 0x0290   //亮蓝色50%
#define BLUE_BT_5 0x02f2   //亮蓝色60%
#define BLUE_BT_6 0x0375   //亮蓝色70%
#define BLUE_BT_7 0x03f8   //亮蓝色80%
#define BLUE_BT_8 0x047b   //亮蓝色90%
#define BLUE_BT_9 0x04fe   //亮蓝色100%
//-----------------------------------------
#define YELLOWGREEN_0  0x2180   //黄绿色10%
#define YELLOWGREEN_1  0x2a20   //黄绿色20%
#define YELLOWGREEN_2  0x3aa0   //黄绿色30%
#define YELLOWGREEN_3  0x4340   //黄绿色40%
#define YELLOWGREEN_4  0x53e0   //黄绿色50%
#define YELLOWGREEN_5  0x5c60   //黄绿色60%
#define YELLOWGREEN_6  0x6d00   //黄绿色70%
#define YELLOWGREEN_7  0x7dc0   //黄绿色80%
#define YELLOWGREEN_8  0x8e80   //黄绿色90%
#define YELLOWGREEN_9  0xa780   //黄绿色100%



/****************************************************************
GUI系统定义的全局用色-用户可按自己需求修改,但请合理修改
*****************************************************************/

#define COLOR_FOCUS					RED		/*焦点色*/
#define COLOR_FLASH					GREEN	/*闪烁色*/
#define COLOR_DISABLE				GRAY1	/*无效色*/
#define COLOR_FRAME_2D				BLACK	/*2D图案的框色*/
#define COLOR_SYS_FILL_RECT			WHITE	/*系统的控件填充色,WINDOW的风格,就是白色,应用于EDIT,CHECK等框体*/

//A型框的颜色
#define COLOR_FRAME_A_OU			GRAY2	//外上
#define COLOR_FRAME_A_OD			WHITE	//外下
#define COLOR_FRAME_A_IU			BLACK	//内上
#define COLOR_FRAME_A_ID			GRAY1	//内下

//B型框的颜色
#define COLOR_FRAME_B				BLACK	//外边框
#define COLOR_FRAME_B_U				WHITE	//中框上
#define COLOR_FRAME_B_D				BLACK	//中框下(暂时用黑色)
#define COLOR_FRAME_B_I				GRAY2	//内框下

//无效字符颜色
#define COLOR_STRING_DISABLE1		WHITE	//无效按键的字符色1
#define COLOR_STRING_DISABLE2		GRAY2	//无效按键的字符色2


#endif
