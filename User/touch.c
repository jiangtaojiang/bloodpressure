/***********************************************
**模块:触摸管理
**功能:管理触摸屏,支持按下,停留和弹起的反应
**作者:电子白菜
**版本:V1.0
**版本信息:消抖管理和键盘的基本相同,不同的是触摸后的数据放进数据缓冲中,让主循环中自己调用.此为尝试,在之后的应用中,可根据情况再作 
**			改进. 
***********************************************/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usr.h"
#include "touch.h"
//#include "hal.h"

#define NULL	0
//定义SPI接口的状态,方便复用
u8 Spi1_Mode=0;
u8 Spi2_Mode=0;

//硬件定义
/**************************************************************
SPI接口处于的状态的定义方便复用SPI接口
***************************************************************/
#define SPI1_MODE_SST25		1
#define SPI1_MODE_SD		2

#define SPI2_MODE_NET		1
#define SPI2_MODE_TOUCH		2
#define SPI2_MODE_LCD		3
#define TOUCH_DESELECT()	GPIOB->BSRR=GPIO_Pin_0
#define TOUCH_SELECT()  	GPIOB->BRR=GPIO_Pin_0 

#define GET_PEN_INT		(GPIOB->IDR&GPIO_Pin_12)

static SPI_InitTypeDef  SPI_InitStructure;
void Touch_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* PB15-MOSI2,PB13-SCK2,PB14-MISO2*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13  | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PB-14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*  PB0-CS*/
	GPIO_SetBits(GPIOB, GPIO_Pin_0);//预置为高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*PB12-INT,输入*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* SPI2 configuration */
	SPI_Cmd(SPI2, DISABLE); 												//必须先禁能,才能改变MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;								//CPOL=1 时钟悬空x
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=1 数据捕获第x
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//32M/16=2M
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE); 

}

static u8 SPIByte(u8 byte)
{
	/*等待发送寄存器空*/
	while((SPI2->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*发送一个字节*/
	SPI2->DR = byte;
	/* 等待接收寄存器有效*/
	while((SPI2->SR & SPI_I2S_FLAG_RXNE)==RESET);
	return(SPI2->DR);
}

/********************************************************
**SPI2转到触摸
********************************************************/
void SPI2TurnToTouch(void)
{
	if(Spi2_Mode!=SPI2_MODE_TOUCH)
	{
		SPI_Cmd(SPI2, DISABLE); 
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;								//CPOL=1 时钟悬空高
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=1 数据捕获第2沿
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//32M/16=2M
		SPI_Init(SPI2, &SPI_InitStructure);
		SPI_Cmd(SPI2, ENABLE); 
		TOUCH_DESELECT();
		SPIByte(0xff);	//复位总线
		Spi2_Mode=SPI2_MODE_TOUCH;
	}
}



u16 X_Buf[10],Y_Buf[10];

/**************************************************************
** 函数名:GetTouchCoordinate
** 功能:获取触摸屏的坐标
** 注意事项:这个是获取物理坐标的函数,调用了物理接口读取2046,返回X,Y的12BIT坐标. 
**  		输入坐标为指向结果的X,Y指针 ,如果获取的触摸点有效,则返回TRUE
**  		触摸点有效并且PEN脚有效的坐标才被采用,否则当作抖动.
**			*_*有了触摸管理系统后,这个函数就没用了
***************************************************************/
/*
bool GetTouchCoordinate(u16* p_x,u16* p_y)
{
	u8 i;
	u16 tmp;
	if(GET_PEN_INT) 
		return FALSE;

	TOUCH_SELECT();
	__nop();
	SPIByte(0xd0);

	for(i=0;i<10;i++) 
	{
		tmp=SPIByte(0);
		tmp<<=8;
		tmp|=SPIByte(0);
		tmp>>=3;
		SPIByte(0x90);
		X_Buf[i]=tmp;
		tmp=SPIByte(0);
		tmp<<=8;
		tmp|=SPIByte(0);
		tmp>>=3;
		SPIByte(0xd0);
		Y_Buf[i]=tmp;

		if(GET_PEN_INT)
		{
			TOUCH_DESELECT();
			return FALSE;
		}
	}
	TOUCH_DESELECT();
	//滤波,用0xfff减是因为修正触摸的硬件方向
	*p_x=0xfff-DigitFilter(X_Buf,10);
	*p_y=0xfff-DigitFilter(Y_Buf,10);
	return TRUE;
}
*/
bool GetPenTouch(void)
{
	if(GET_PEN_INT)
		return TRUE;
	else 
		return FALSE;
}

/***********************************************
**函数名:GetOneCoordinate
**功能:获取一个坐标并放到指定的缓冲上
**注意事项:使用触摸管理后,唯一需要调动底层的就是这个函数,注意要把SPI状态转向触摸
***********************************************/
void GetOneCoordinate(u8 no)
{
	u16 tmp;
	
	SPI2TurnToTouch();
	
	TOUCH_SELECT();

	__nop();
	SPIByte(0xd0);
	__nop();
	tmp=SPIByte(0);
	tmp<<=8;
	tmp|=SPIByte(0);
	tmp>>=3;
	SPIByte(0x90);
	X_Buf[no]=tmp;
	tmp=SPIByte(0);
	tmp<<=8;
	tmp|=SPIByte(0);
	tmp>>=3;
	SPIByte(0xd0);
	Y_Buf[no]=tmp;

	TOUCH_DESELECT();
}


//滤波
void TouchFilter(u16* p_x,u16* p_y)
{
	//滤波,用0xfff减是因为修正触摸的硬件方向
	#if TOUCH_TYPE==1
	*p_x=0xfff-DigitFilter(X_Buf,10);
	*p_y=0xfff-DigitFilter(Y_Buf,10);
	#elif TOUCH_TYPE==2
	*p_x=DigitFilter(X_Buf,10);
	*p_y=DigitFilter(Y_Buf,10);
	#endif
}
/**************************************************************
 
 触摸的上层判断函数,与GUI无关,CountingTouch,ManageTouch共同于1MS定时器中调用,用户引用几个外部函数则可读取触摸值
 
***************************************************************/

static u8 Touch_Flag=0;	//按键标志
static u8 Filter_No=0;	//滤波计数器
static u8 Touch_Cnt_Time=0;	//消抖时间

/**************************************************************
触摸的输出缓冲区,所有触摸得到的坐标都会装载进下列变量中,这堆变量使得触摸底层与应用层彻底分离
***************************************************************/
static u16 T_Push_X=0,T_Push_Y=0;				//触下的XY(AD值)
static u16 T_Hold_X=0,T_Hold_Y=0;				//保持的XY(AD值)
static u16 T_Release_X=0,T_Release_Y=0;			//释放的XY(AD值)

/**************************************************************
** 函数名:ManageTouch
** 功能:触摸管理
** 注意事项:研究中...笔中断有效时开始消抖(非阻塞延迟),->进入有效读TOUCH,每个循环读一次并检测笔中断,够10个循环则滤波出值.
***************************************************************/
void ManageTouch(void)
{
	Touch_Cnt_Time++;
	/**************************************************************
		PEN为无效效
	***************************************************************/
	if(GET_PEN_INT)
	{
		/**************************************************************
			考虑这里也要使用消抖-释放消抖
		***************************************************************/

		if(!(Touch_Flag&TOUCH_F_RELEASE))
		{
			//如果之前有HOLD标志,则代表释放
			if(Touch_Flag&TOUCH_F_HOLD)
			{
				Touch_Flag=TOUCH_F_NULL;
				Touch_Flag|=TOUCH_F_RELEASE|TOUCH_F_HOLD;
				T_Release_X=T_Hold_X;
				T_Release_Y=T_Hold_Y;
			}
			//否则之前只有PUSH标志,也代表释放
			else if(Touch_Flag&TOUCH_F_PUSH)
			{
				Touch_Flag=TOUCH_F_NULL;
				Touch_Flag|=TOUCH_F_RELEASE|TOUCH_F_PUSH;
				T_Release_X=T_Push_X;
				T_Release_Y=T_Push_Y;
			}
		}
		return;
	}

	/**************************************************************
		PEN为有效
	***************************************************************/
	//如果之前有释放状态,则从新开始判断触摸
	if(Touch_Flag&TOUCH_F_RELEASE)
	{
		goto new_touch;
	}
	
	//-1.TOUCH_F_WAIT_HOLD标志,代表要开始计算等待保持时间
	if(Touch_Flag&TOUCH_F_WAIT_HOLD)
	{
		if(Touch_Cnt_Time>=TOUCH_CNT_WAIT_HOLD_TIME)
		{
			Touch_Flag|=TOUCH_F_HOLD;
			Touch_Flag&=~TOUCH_F_WAIT_HOLD;
			T_Hold_X=T_Release_X;
			T_Hold_Y=T_Release_Y;
		}
	}
	//-1.HOLD并且非滤波标志
	else if((Touch_Flag&TOUCH_F_HOLD)&&(!(Touch_Flag&TOUCH_F_FILTERING)))
	{
		Touch_Flag|=TOUCH_F_FILTERING;		//则开始滤波
		Filter_No=0;
	}
	
	//-3.PUSH标志并且非滤波标志*_*
	else if((Touch_Flag&TOUCH_F_PUSH)&&(!(Touch_Flag&TOUCH_F_FILTERING)))
	{
		if(Touch_Cnt_Time>=TOUCH_CNT_HOLD_TIME)	//则开始计算BPK,时间到则又进入滤波
		{
			Touch_Flag|=TOUCH_F_FILTERING;
			Filter_No=0;
		}
	}
	
	//-3.FLITERING-当没PUSH到PUSH的时候会进入这里一次,HOLD则连续进入
	else if(Touch_Flag&TOUCH_F_FILTERING)
	{
		GetOneCoordinate(Filter_No);
		if((++Filter_No)==MAX_TOUCH_FILTER_NO)
		{
			if(!(Touch_Flag&TOUCH_F_PUSH))		//之前没有PUSH,则这个是属于PUSH的滤波
			{
				TouchFilter(&T_Push_X,&T_Push_Y);
				Touch_Flag=TOUCH_F_PUSH;
				Touch_Cnt_Time=0;
			}
			else								//属于HOLD的滤波
			{
				TouchFilter(&T_Release_X,&T_Release_Y);//放到Release纯粹为了节省临时变量
				Touch_Flag&=~TOUCH_F_FILTERING;
				Touch_Flag|=TOUCH_F_WAIT_HOLD;
				Touch_Cnt_Time=0;
			}
		}
	}
	//-4.BCK-消抖时间到,则转入滤波
	else if(Touch_Flag&TOUCH_F_BCK_TIME)
	{
		if(Touch_Cnt_Time>=TOUCH_CNT_FLITER_TIME)
		{
			Touch_Flag=TOUCH_F_FILTERING;
			Filter_No=0;
		}
	}
	//-5.PUCH_NULL,置消抖标志
	else
	{
		new_touch:
		Touch_Flag=TOUCH_F_BCK_TIME;
		Touch_Cnt_Time=0;
	}

}


/**************************************************************
** 函数名:GetTouchPush/Hold/Release
** 功能:获取触摸PUSH的坐标
** 注意事项:如果获取有效,则返回TRUE,参数带NULL保护的--*_*但并非参数必须不为NULL,实际上输入NULL参数,仅仅用于获取标志状态也是可行的
***************************************************************/
bool GetTouchPush(u16* x,u16* y)
{
	if( (Touch_Flag&TOUCH_F_PUSH)&&(!(Touch_Flag&TOUCH_F_RELEASE)) )
	{
		if(x!=NULL)
			*x=T_Push_X;
		if(y!=NULL)
			*y=T_Push_Y;
		return TRUE;
	}
	else
		return FALSE;
}
bool GetTouchHold(u16* x,u16* y)
{
	if( (Touch_Flag&TOUCH_F_HOLD)&&(!(Touch_Flag&TOUCH_F_RELEASE)) )
	{
		if(x!=NULL)
			*x=T_Hold_X;
		if(y!=NULL)
			*y=T_Hold_Y;
		return TRUE;
	}
	else
		return FALSE;
}
//获取RELEASE后,标志自动清除
bool GetTouchRelease(u16* x,u16* y)
{

	if(Touch_Flag&TOUCH_F_RELEASE)
	{
		if(x!=NULL)
			*x=T_Release_X;
		if(y!=NULL)
			*y=T_Release_Y;

		Touch_Flag=TOUCH_F_NULL;
		return TRUE;
	}
	else
		return FALSE;
}

/**************************************************************
** 函数名:ClearTouchFlag
** 功能:清除触摸Touch_Flag标志
** 注意事项:单独的PUSH和HOLD是不能清除的,有RELEASE标志才能清除
***************************************************************/
void ClearTouchFlag(void)
{
	if(Touch_Flag&TOUCH_F_RELEASE)
	{
		Touch_Flag=TOUCH_F_NULL;
	}
}

