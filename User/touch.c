/***********************************************
**ģ��:��������
**����:��������,֧�ְ���,ͣ���͵���ķ�Ӧ
**����:���Ӱײ�
**�汾:V1.0
**�汾��Ϣ:��������ͼ��̵Ļ�����ͬ,��ͬ���Ǵ���������ݷŽ����ݻ�����,����ѭ�����Լ�����.��Ϊ����,��֮���Ӧ����,�ɸ���������� 
**			�Ľ�. 
***********************************************/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "usr.h"
#include "touch.h"
//#include "hal.h"

#define NULL	0
//����SPI�ӿڵ�״̬,���㸴��
u8 Spi1_Mode=0;
u8 Spi2_Mode=0;

//Ӳ������
/**************************************************************
SPI�ӿڴ��ڵ�״̬�Ķ��巽�㸴��SPI�ӿ�
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
	GPIO_SetBits(GPIOB, GPIO_Pin_0);//Ԥ��Ϊ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*PB12-INT,����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* SPI2 configuration */
	SPI_Cmd(SPI2, DISABLE); 												//�����Ƚ���,���ܸı�MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;								//CPOL=1 ʱ������x
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=1 ���ݲ����x
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//���NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//32M/16=2M
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE); 

}

static u8 SPIByte(u8 byte)
{
	/*�ȴ����ͼĴ�����*/
	while((SPI2->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*����һ���ֽ�*/
	SPI2->DR = byte;
	/* �ȴ����ռĴ�����Ч*/
	while((SPI2->SR & SPI_I2S_FLAG_RXNE)==RESET);
	return(SPI2->DR);
}

/********************************************************
**SPI2ת������
********************************************************/
void SPI2TurnToTouch(void)
{
	if(Spi2_Mode!=SPI2_MODE_TOUCH)
	{
		SPI_Cmd(SPI2, DISABLE); 
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;								//CPOL=1 ʱ�����ո�
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=1 ���ݲ����2��
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//32M/16=2M
		SPI_Init(SPI2, &SPI_InitStructure);
		SPI_Cmd(SPI2, ENABLE); 
		TOUCH_DESELECT();
		SPIByte(0xff);	//��λ����
		Spi2_Mode=SPI2_MODE_TOUCH;
	}
}



u16 X_Buf[10],Y_Buf[10];

/**************************************************************
** ������:GetTouchCoordinate
** ����:��ȡ������������
** ע������:����ǻ�ȡ��������ĺ���,����������ӿڶ�ȡ2046,����X,Y��12BIT����. 
**  		��������Ϊָ������X,Yָ�� ,�����ȡ�Ĵ�������Ч,�򷵻�TRUE
**  		��������Ч����PEN����Ч������ű�����,����������.
**			*_*���˴�������ϵͳ��,���������û����
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
	//�˲�,��0xfff������Ϊ����������Ӳ������
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
**������:GetOneCoordinate
**����:��ȡһ�����겢�ŵ�ָ���Ļ�����
**ע������:ʹ�ô��������,Ψһ��Ҫ�����ײ�ľ����������,ע��Ҫ��SPI״̬ת����
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


//�˲�
void TouchFilter(u16* p_x,u16* p_y)
{
	//�˲�,��0xfff������Ϊ����������Ӳ������
	#if TOUCH_TYPE==1
	*p_x=0xfff-DigitFilter(X_Buf,10);
	*p_y=0xfff-DigitFilter(Y_Buf,10);
	#elif TOUCH_TYPE==2
	*p_x=DigitFilter(X_Buf,10);
	*p_y=DigitFilter(Y_Buf,10);
	#endif
}
/**************************************************************
 
 �������ϲ��жϺ���,��GUI�޹�,CountingTouch,ManageTouch��ͬ��1MS��ʱ���е���,�û����ü����ⲿ������ɶ�ȡ����ֵ
 
***************************************************************/

static u8 Touch_Flag=0;	//������־
static u8 Filter_No=0;	//�˲�������
static u8 Touch_Cnt_Time=0;	//����ʱ��

/**************************************************************
���������������,���д����õ������궼��װ�ؽ����б�����,��ѱ���ʹ�ô����ײ���Ӧ�ò㳹�׷���
***************************************************************/
static u16 T_Push_X=0,T_Push_Y=0;				//���µ�XY(ADֵ)
static u16 T_Hold_X=0,T_Hold_Y=0;				//���ֵ�XY(ADֵ)
static u16 T_Release_X=0,T_Release_Y=0;			//�ͷŵ�XY(ADֵ)

/**************************************************************
** ������:ManageTouch
** ����:��������
** ע������:�о���...���ж���Чʱ��ʼ����(�������ӳ�),->������Ч��TOUCH,ÿ��ѭ����һ�β������ж�,��10��ѭ�����˲���ֵ.
***************************************************************/
void ManageTouch(void)
{
	Touch_Cnt_Time++;
	/**************************************************************
		PENΪ��ЧЧ
	***************************************************************/
	if(GET_PEN_INT)
	{
		/**************************************************************
			��������ҲҪʹ������-�ͷ�����
		***************************************************************/

		if(!(Touch_Flag&TOUCH_F_RELEASE))
		{
			//���֮ǰ��HOLD��־,������ͷ�
			if(Touch_Flag&TOUCH_F_HOLD)
			{
				Touch_Flag=TOUCH_F_NULL;
				Touch_Flag|=TOUCH_F_RELEASE|TOUCH_F_HOLD;
				T_Release_X=T_Hold_X;
				T_Release_Y=T_Hold_Y;
			}
			//����֮ǰֻ��PUSH��־,Ҳ�����ͷ�
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
		PENΪ��Ч
	***************************************************************/
	//���֮ǰ���ͷ�״̬,����¿�ʼ�жϴ���
	if(Touch_Flag&TOUCH_F_RELEASE)
	{
		goto new_touch;
	}
	
	//-1.TOUCH_F_WAIT_HOLD��־,����Ҫ��ʼ����ȴ�����ʱ��
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
	//-1.HOLD���ҷ��˲���־
	else if((Touch_Flag&TOUCH_F_HOLD)&&(!(Touch_Flag&TOUCH_F_FILTERING)))
	{
		Touch_Flag|=TOUCH_F_FILTERING;		//��ʼ�˲�
		Filter_No=0;
	}
	
	//-3.PUSH��־���ҷ��˲���־*_*
	else if((Touch_Flag&TOUCH_F_PUSH)&&(!(Touch_Flag&TOUCH_F_FILTERING)))
	{
		if(Touch_Cnt_Time>=TOUCH_CNT_HOLD_TIME)	//��ʼ����BPK,ʱ�䵽���ֽ����˲�
		{
			Touch_Flag|=TOUCH_F_FILTERING;
			Filter_No=0;
		}
	}
	
	//-3.FLITERING-��ûPUSH��PUSH��ʱ����������һ��,HOLD����������
	else if(Touch_Flag&TOUCH_F_FILTERING)
	{
		GetOneCoordinate(Filter_No);
		if((++Filter_No)==MAX_TOUCH_FILTER_NO)
		{
			if(!(Touch_Flag&TOUCH_F_PUSH))		//֮ǰû��PUSH,�����������PUSH���˲�
			{
				TouchFilter(&T_Push_X,&T_Push_Y);
				Touch_Flag=TOUCH_F_PUSH;
				Touch_Cnt_Time=0;
			}
			else								//����HOLD���˲�
			{
				TouchFilter(&T_Release_X,&T_Release_Y);//�ŵ�Release����Ϊ�˽�ʡ��ʱ����
				Touch_Flag&=~TOUCH_F_FILTERING;
				Touch_Flag|=TOUCH_F_WAIT_HOLD;
				Touch_Cnt_Time=0;
			}
		}
	}
	//-4.BCK-����ʱ�䵽,��ת���˲�
	else if(Touch_Flag&TOUCH_F_BCK_TIME)
	{
		if(Touch_Cnt_Time>=TOUCH_CNT_FLITER_TIME)
		{
			Touch_Flag=TOUCH_F_FILTERING;
			Filter_No=0;
		}
	}
	//-5.PUCH_NULL,��������־
	else
	{
		new_touch:
		Touch_Flag=TOUCH_F_BCK_TIME;
		Touch_Cnt_Time=0;
	}

}


/**************************************************************
** ������:GetTouchPush/Hold/Release
** ����:��ȡ����PUSH������
** ע������:�����ȡ��Ч,�򷵻�TRUE,������NULL������--*_*�����ǲ������벻ΪNULL,ʵ��������NULL����,�������ڻ�ȡ��־״̬Ҳ�ǿ��е�
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
//��ȡRELEASE��,��־�Զ����
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
** ������:ClearTouchFlag
** ����:�������Touch_Flag��־
** ע������:������PUSH��HOLD�ǲ��������,��RELEASE��־�������
***************************************************************/
void ClearTouchFlag(void)
{
	if(Touch_Flag&TOUCH_F_RELEASE)
	{
		Touch_Flag=TOUCH_F_NULL;
	}
}

