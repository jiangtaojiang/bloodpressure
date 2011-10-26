/**************************************************************
GUI����,ǰ����Ϊ����������,�󲿷�Ϊ������
***************************************************************/
#define TFTdriver_c
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "TFTdriver.h"
#include "font_data.h"
#include "GuiDef.h"
#include "hal.h"
#include "TFTHal.h"

COLOR BG_COLOR;	//ȫ�ֵı���ɫ,�˱�����Ϊ�����������ٶȶ����õ�

/**************************************************************
** ������:TFTFill
** ����:��TFT�����ɫ
** ע������:�ӵ�ַ0��ʼ���
***************************************************************/
void TFTFill(u16 color)
{
	u32 i,j;
	SetBusWrite();
#if (SSD1297==1)||(SSD1289==1)
	wr_cmd(0x004e);
	wr_data(0);
	wr_cmd(0x004f);
	wr_data(0);
#elif (R6150V==1)||(R6150U==1)||(ST7781==1)||(ILI9328==1)
	wr_cmd(0x0020);
	wr_data(0);
	wr_cmd(0x0021);
	wr_data(0);
#elif (HX8347==1)
	wr_cmd(0x02);
	wr_data(0);
	wr_cmd(0x03);
	wr_data(0);
	
	wr_cmd(0x06);
	wr_data(0);
	wr_cmd(0x07);
	wr_data(0);
#endif

	wr_cmd(0x0022);
	for(i=0;i<320;i++)
	for(j=0;j<240;j++)
	{
		wr_data(color);
	}
}

/**************************************************************
** ������:TFTPoint
** ����:��һ����
** ע������:��ʱû��ô�ù�~
***************************************************************/
void TFTPoint(LOC_X x,LOC_Y y,COLOR color)
{
	CHECK_LOC(x,y);
	//���߷���
	SetBusWrite();
#if (SSD1297==1)||(SSD1289==1)
	wr_cmd(0x004e);
	wr_data(x);
	wr_cmd(0x004f);
	wr_data(y);
#elif (R6150V==1)||(R6150U==1)||(ST7781==1)||(ILI9328==1)
	wr_cmd(0x0020);//x
	wr_data(x);
	wr_cmd(0x0021);//y
	wr_data(y);
#elif (HX8347==1)
	wr_cmd(0x02);
	wr_data(0);
	wr_cmd(0x03);
	wr_data(x);

	wr_cmd(0x06);
	wr_data(y>>8);
	wr_cmd(0x07);
	wr_data((u8)y);
#endif


	wr_cmd(0x0022);//��ʼд
	wr_data(color);
}
/**************************************************************
** ������:TFTSetXY/_TFTPoint
** ����:����XY,��д��һ����
** ע������:��TFTPoint������Ϊ������,��ĳЩ������,������������������,�����Ч�� 
** 			*_*Ϊ�������Ч������������������߷���,Ӧ�ڵ���ǰ����.
***************************************************************/
static void TFTSetXY(LOC_X x,LOC_Y y)
{
	CHECK_LOC(x,y);

#if (SSD1297==1)||(SSD1289==1)
	wr_cmd(0x004e);
	wr_data(x);
	wr_cmd(0x004f);
	wr_data(y);
#elif (R6150V==1)||(R6150U==1)||(ST7781==1)||(ILI9328==1)
	wr_cmd(0x0020);//x
	wr_data(x);
	wr_cmd(0x0021);//y
	wr_data(y);
#elif (HX8347==1)
	wr_cmd(0x02);
	wr_data(0);
	wr_cmd(0x03);
	wr_data(x);
	
	wr_cmd(0x06);
	wr_data(y>>8);
	wr_cmd(0x07);
	wr_data((u8)y);
#endif

	wr_cmd(0x0022);//��ʼд
}
static void _TFTPoint(COLOR color)
{
	wr_data(color);
}

/**************************************************************
** ������:SetTFTStyleH/V
** ����:����TFT����оƬ�ĺ�����
** ע������:����Ǵ�TFTLCD���еĻ���,ʹ��������ƿ���ʵ�ָ��򵥵ĺ����߻���,ȱ������ֲ�Խ���
***************************************************************/
static u8 HV_Style=0;	//�����������0�Ǻ�,1����,��ʼ��ʱ��Ĭ��Ϊ��
static void SetTFTStyleH(void)
{
	if(HV_Style)
	{
		HV_Style=0;//���ú�
	#if (SSD1297==1)||(SSD1289==1)
		wr_cmd(0x0011);
		wr_data(0x6030);
	#elif (R6150V==1)||(R6150U==1)
		wr_cmd(0x03);
		wr_data(0x5030);
	#elif (HX8347==1)
		wr_cmd(0x0016);
		wr_data(0x00c8);
		wr_cmd(0x0004);wr_data(0x0000);   // ��ĩ239
		wr_cmd(0x0005);wr_data(0x00EF);
		wr_cmd(0x0008);wr_data(0x0001);   // ��ĩ319
		wr_cmd(0x0009);wr_data(0x003F);
	#elif (ST7781==1)||(ILI9328==1)
		wr_cmd(0x03);
		wr_data(0x5030);
	#endif
	}
}
static void SetTFTStyleV(void)
{
	if(!HV_Style)
	{
		HV_Style=1;//������
	#if (SSD1297==1)||(SSD1289==1)
		wr_cmd(0x0011);
		wr_data(0x6038);
	#elif (R6150V==1)||(R6150U==1)
		wr_cmd(0x03);
		wr_data(0x5028);
	#elif (HX8347==1)
		wr_cmd(0x0016);
		wr_data(0x00e8);
		wr_cmd(0x0004);wr_data(0x0001);   // ��ĩ319
		wr_cmd(0x0005);wr_data(0x003F);
		wr_cmd(0x0008);wr_data(0x0000);   // ��ĩ239
		wr_cmd(0x0009);wr_data(0x00EF);
	#elif (ST7781==1)||(ILI9328==1)
		wr_cmd(0x03);
		wr_data(0x5038);
	#endif
	}
}
/**************************************************************
** ������:TFTHLine
** ����:��һ������
** ע������:���������˿���оƬ�ı���,ͨ����д����ķ�ʽ����,ֻ��Ҫ����һ�������,���ʵ������������д. 
** 			VLINE����Ҳ���
***************************************************************/
void TFTHLine(LOC_X x,LOC_Y y,u16 len,COLOR color)
{
	u16 i;
	CHECK_LOC(x,y);
	if(len+x>MAX_X)
	{
		len=MAX_X-x;
	}
	//���߷���
	SetBusWrite();
	//��Ϊ����
	SetTFTStyleH();
#if (SSD1297==1)||(SSD1289==1)
	wr_cmd(0x004e);
	wr_data(x);
	wr_cmd(0x004f);
	wr_data(y);
#elif (R6150V==1)||(R6150U==1)||(ST7781==1)||(ILI9328==1)
	wr_cmd(0x0020);//x
	wr_data(x);
	wr_cmd(0x0021);//y
	wr_data(y);
#elif (HX8347==1)
	wr_cmd(0x02);
	wr_data(0);
	wr_cmd(0x03);
	wr_data(x);
	
	wr_cmd(0x06);
	wr_data(y>>8);
	wr_cmd(0x07);
	wr_data((u8)y);
#endif
	//��ʼд
	wr_cmd(0x0022);
	for(i=0;i<len;i++)
	{
		wr_data(color);
	}
}


/**************************************************************
** ������:TFTVLine
** ����:��һ������
** ע������:�ο�HLINE
***************************************************************/
void TFTVLine(LOC_X x,LOC_Y y,u16 len,COLOR color)
{
	u16 i;
	CHECK_LOC(x,y);
	if(len+y>MAX_Y)
	{
		len=MAX_Y-y;
	}
	//���߷���
	SetBusWrite();
	//תΪ����
	SetTFTStyleV();
	//����
#if (SSD1297==1)||(SSD1289==1)
	wr_cmd(0x004e);
	wr_data(x);
	wr_cmd(0x004f);
	wr_data(y);
#elif (R6150V==1)||(R6150U==1)||(ST7781==1)||(ILI9328==1)
	wr_cmd(0x0020);//x
	wr_data(x);
	wr_cmd(0x0021);//y
	wr_data(y);
#elif (HX8347==1)
	
//��������������ʱ��,XY������Ҳ����ű任
	wr_cmd(0x02);
	wr_data(y>>8);
	wr_cmd(0x03);
	wr_data((u8)y);
	
	wr_cmd(0x06);
	wr_data(0);
	wr_cmd(0x07);
	wr_data(x);
#endif
	
	//��ʼд
	wr_cmd(0x22);
	for(i=0;i<len;i++)
	{
		wr_data(color);
	}
}

/**************************************************************
** ������:TFTRect
** ����:��һ���վ���
** ע������:���ú��������ĺ���,�����������Ϊ0
***************************************************************/
void TFTRect(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height,COLOR color)
{
	if((length==0)||(height==0))	
		return;

	TFTHLine(x0,y0,length,color);
	TFTHLine(x0,y0+height-1,length,color);    
	TFTVLine(x0,y0+1,height-1,color);
	TFTVLine(x0+length-1,y0+1,height-1,color);
	
}

/**************************************************************
** ������:TFTRectFill
** ����:������
** ע������:���û��ߺ���,�����������Ϊ0
***************************************************************/
void TFTRectFill(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height,COLOR color)
{
	u32 i;

	for (i=0;i<height;i++)
	{
		TFTHLine(x0,y0+i,length,color);
	}
}
/**************************************************************
** ������:TFTShowHZ16
** ����:��ʾ����.�ڲ�����
** ע������:
***************************************************************/
static bool Transparent;//͸������

#if FONT_HZ_TYPE==FONT_HZ_TYPE_CODE	//CODE���ֿ�
static void TFTShowHZ16(LOC_X x,LOC_Y y,u16 hz,COLOR color)
{
	u16 font_byte;
	u16 k;
	u8  i,j;
	u8* buf;
	bool transparent;
	
	if (x>MAX_X-16)
		return;
	if (y>MAX_Y-16)
		return;
	

	SetTFTStyleH();//���ú�
	transparent=Transparent;
	for (k = 0;Hz_Code16[k].Hz_Index[0] != 0xff;k++)
	{
		if ( (u8)hz == Hz_Code16[k].Hz_Index[0] )
		{
			if ( (hz>>8) == Hz_Code16[k].Hz_Index[1] )
			{
				buf=(u8*)(&(Hz_Code16[k].Code[0]));

				for (i=0;i<16;i++)//16��
				{
					font_byte=((u16*)buf)[i];

					TFTSetXY(x,y);
					for (j=0;j<16;j++)//ÿ��16����
					{
						if (font_byte&0x01)
						{
							_TFTPoint(color);	//д��Ч��
						}
						else
						{
							if (transparent==FALSE)
								_TFTPoint(BG_COLOR);//д��ɫ
							else
								TFTSetXY(x+j+1,y);	//ƫ��һ����
						}
						font_byte>>=1;
					}
					y++;
				}
				return;
			}
		}

	}
}

static void TFTShowHZ24(LOC_X x,LOC_Y y,u16 hz,COLOR color)
{
	u32 font_byte;
	u16 k;
	u8  i,j;
	u8* buf;
	bool transparent;
	
	if (x>MAX_X-24)
		return;
	if (y>MAX_Y-24)
		return;

	SetTFTStyleH();//���ú�
	
	transparent=Transparent;
	for (k = 0;Hz_Code24[k].Hz_Index[0] != 0xff;k++)
	{
		if ( (u8)hz == Hz_Code24[k].Hz_Index[0] )
		{
			if ( (hz>>8) == Hz_Code24[k].Hz_Index[1] )
			{
				buf=(u8*)(&(Hz_Code24[k].Code[0]));

				for (i=0;i<24;i++)//24��
				{
					font_byte=*buf++;
					font_byte+=(*buf++)<<8;
					font_byte+=(*buf++)<<16;

					TFTSetXY(x,y);
					for (j=0;j<24;j++)//ÿ��24����
					{
						if (font_byte&0x01)
						{
							_TFTPoint(color);	//д��Ч��
						}
						else
						{
							if (transparent==FALSE)
								_TFTPoint(BG_COLOR);//д��ɫ
							else
								TFTSetXY(x+j+1,y);	//ƫ��һ����
						}
						font_byte>>=1;
					}
					y++;
				}
				return;
			}
		}

	}
}

#elif FONT_HZ_TYPE==FONT_HZ_TYPE_FLASH

extern void SST25Read(u32 addr,u8* p_data,u32 no);		//��Ҫʹ��SST�Ķ�FLASH����
static void TFTShowHZ16(LOC_X x,LOC_Y y,u16 hz,COLOR color)
{
	u16 font_byte;
	u8  i,j;
	u8 c1,c2;
	u32 addr;
	u16 _HZ16_Buf[16];//�����ֿ⻺��
	bool transparent;
	
	if (x>MAX_X-16)
		return;
	if (y>MAX_Y-16)
		return;
	SetTFTStyleH();//���ú�
	transparent=Transparent;
	c1=(u8)hz;
	c2=hz>>8;
	addr=HZ_DOT16_START_ADDR+((c1-0xb0)*94+(c2-0xa1))*32;
//    addr=HZ_DOT16_START_ADDR+((c1-0xa1)*94+(c2-0xa1))*32;
	SST25Read(addr,(u8*)_HZ16_Buf,32);
	for (i=0;i<16;i++)//16��
	{
		TFTSetXY(x,y);
		font_byte=_HZ16_Buf[i];
		for (j=0;j<16;j++)//ÿ��16����
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//д��Ч��
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//д��ɫ
				else
					TFTSetXY(x+j+1,y);	//ƫ��һ����
			}
			font_byte>>=1;
		}
		y++;
	}
}

static void TFTShowHZ24(LOC_X x,LOC_Y y,u16 hz,COLOR color)
{
	u32 font_byte;
	u8  i,j;
	u8 c1,c2;
	u32 addr;
	u8 HZ24_Buf[72];//�����ֿ⻺��(���ð�����ʽ,���Ա������ֽڶ���)
	u8 *buf=HZ24_Buf;
	bool transparent;
	if (x>MAX_X-16)
		return;
	if (y>MAX_Y-16)
		return;
	SetTFTStyleH();//���ú�
	transparent=Transparent;
	c1=(u8)hz;
	c2=hz>>8;
	addr=HZ_DOT24_START_ADDR+((c1-0xb0)*94+(c2-0xa1))*72;
	SST25Read(addr,HZ24_Buf,72);

	for (i=0;i<24;i++)//24��
	{
		font_byte=*buf++;
		font_byte+=(*buf++)<<8;
		font_byte+=(*buf++)<<16;

		TFTSetXY(x,y);
		for (j=0;j<24;j++)//ÿ��24����
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//д��Ч��
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//д��ɫ
				else
					TFTSetXY(x+j+1,y);	//ƫ��һ����
			}
			font_byte>>=1;
		}
		y++;
	}
}

#else
	#error("Must set one hz type!")
#endif
//�ⲿflash�ֿ�
static void TFTShowASC1616(LOC_X x,LOC_Y y,u8 asc,COLOR color)
{
    u32 addr;
	u16 font_byte;
	u8  i,j;
    u8 c2;
    u16 temp1,temp2;
    u16 ASC16_Buf[16];
	u16* buf;
	bool transparent;
	if (x>MAX_X-16)
		return;
	if (y>MAX_Y-16)
		return;
	transparent=Transparent;
//	asc-=0x20;
    c2 = asc-0x20+0xa0;
//	if ( asc> (sizeof(ASCII816)/16) ) //ASC����Ч�ж�
//	{
//		return;
//	}
    addr=HZ_DOT16_START_ADDR+((0xa3-0xa1)*94+(c2-0xa1))*32;
//    addr=HZ_DOT16_START_ADDR+0;//(asc - 32)*16;
	SST25Read(addr,(u8*)ASC16_Buf,32);
    buf = ASC16_Buf;
//	buf=(u8*)(&ASCII816[asc][0]);
	SetTFTStyleH();//���ú�

	for (i=0;i<16;i++)//16��
	{
		font_byte=*buf++;
        /***********************************/
        temp1 = (font_byte>>8)&0x00ff;   //����ȡģ���ֽڵ���
        temp2 = (font_byte<<8)&0xff00;
        font_byte = temp1|temp2;
        /***********************************/
		TFTSetXY(x,y);
		for (j=0;j<16;j++)//ÿ��8����
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//д��Ч��
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//д��ɫ
				else
					TFTSetXY(x+j+1,y);	//ƫ��һ����
			}
			font_byte>>=1;
		}
		y++;
	}
}
//�ڲ�flash�ֿ�
static void TFTShowASC816(LOC_X x,LOC_Y y,u8 asc,COLOR color)
{
	u16 font_byte;
	u8  i,j;
	u8* buf;
	bool transparent;
	if (x>MAX_X-8)
		return;
	if (y>MAX_Y-8)
		return;
	transparent=Transparent;
	asc-=0x20;

	if ( asc> (sizeof(ASCII816)/16) ) //ASC����Ч�ж�
	{
		return;
	}

	buf=(u8*)(&ASCII816[asc][0]);
	SetTFTStyleH();//���ú�

	for (i=0;i<16;i++)//16��
	{
		font_byte=*buf++;
		TFTSetXY(x,y);
		for (j=0;j<8;j++)//ÿ��8����
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//д��Ч��
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//д��ɫ
				else
					TFTSetXY(x+j+1,y);	//ƫ��һ����
			}
			font_byte>>=1;
		}
		y++;
	}
}

static void TFTShowASC3245(LOC_X x,LOC_Y y,u8 asc,COLOR color)
{
	u32 font_byte;
	u8  i,j;
	u8* buf;
	bool transparent;
	if (x>MAX_X-32)
		return;
	if (y>MAX_Y-45)
		return;
    asc-=0x30;//ASCII2432
	transparent=Transparent;
	if (asc>(sizeof(ASCII3245)/180))//ASC����Ч�ж�
	{
		return;
	}

	buf=(u8*)(&ASCII3245[asc][0]);
	SetTFTStyleH();//���ú�

	for (i=0;i<45;i++)//24��
	{

            font_byte=((u32*)buf)[i];
            TFTSetXY(x,y);
            for (j=0;j<32;j++)//ÿ��12����
            {
                if (font_byte&0x01)
                {
                    _TFTPoint(color);	//д��Ч��
                }
                else
                {
                    if (transparent==FALSE)
                        _TFTPoint(BG_COLOR);//д��ɫ
                    else
                        TFTSetXY(x+j+1,y);	//ƫ��һ����
                }
                font_byte>>=1;
            }
            y++;
	}
}
static void TFTShowASC2432(LOC_X x,LOC_Y y,u8 asc,COLOR color)
{
	u32 font_byte,temp;
    u8 tem[3];
	u8  i,j,k;
	u8* buf;
	bool transparent;
	if (x>MAX_X-24)
		return;
	if (y>MAX_Y-32)
		return;
    asc-=0x30;//ASCII2432
	transparent=Transparent;
	if (asc>(sizeof(ASCII2432)/96))//ASC����Ч�ж�
	{
		return;
	}

	buf=(u8*)(&ASCII2432[asc][0]);
	SetTFTStyleH();//���ú�

	for (i=0;i<32;i++)//32��
	{
      for(k=0;k<3;k++){
            tem[k] =((u8*) buf)[i*3+k];
      }
            temp = 0;font_byte = 0;
            temp = (((temp|tem[2])&0x000000ff)<<16);            
            font_byte=font_byte|temp;
            temp = (((temp|tem[1])&0x000000ff)<<8);
            font_byte=font_byte|temp;
            font_byte=(font_byte|tem[0])&0x00ffffff;

            TFTSetXY(x,y);
            for (j=0;j<24;j++)//ÿ��24����
            {
                if (font_byte&0x01)
                {
                    _TFTPoint(color);	//д��Ч��
                }
                else
                {
                    if (transparent==FALSE)
                        _TFTPoint(BG_COLOR);//д��ɫ
                    else
                        TFTSetXY(x+j+1,y);	//ƫ��һ����
                }
                font_byte>>=1;
            }
            y++;
	}
}

static void TFTShowASC1625(LOC_X x,LOC_Y y,u8 asc,COLOR color)
{
	u16 font_byte;
	u8  i,j;
	u8* buf;
	bool transparent;
	if (x>MAX_X-16)
		return;
	if (y>MAX_Y-16)
		return;
    asc-=0x30;//ASCII2432
	transparent=Transparent;
	if (asc>(sizeof(ASCII1625)/50))//ASC����Ч�ж�
	{
		return;
	}

	buf=(u8*)(&ASCII1625[asc][0]);
	SetTFTStyleH();//���ú�

	for (i=0;i<25;i++)//24��
	{

            font_byte=((u16*)buf)[i];
            TFTSetXY(x,y);
            for (j=0;j<16;j++)//ÿ��12����
            {
                if (font_byte&0x01)
                {
                    _TFTPoint(color);	//д��Ч��
                }
                else
                {
                    if (transparent==FALSE)
                        _TFTPoint(BG_COLOR);//д��ɫ
                    else
                        TFTSetXY(x+j+1,y);	//ƫ��һ����
                }
                font_byte>>=1;
            }
            y++;
	}
}
static void TFTShowASC1224(LOC_X x,LOC_Y y,u8 asc,COLOR color)
{
	u16 font_byte;
	u8  i,j;
	u8* buf;
	bool transparent;
	if (x>MAX_X-12)
		return;
	if (y>MAX_Y-12)
		return;
	asc-=0x20;//ASCII1224
//    asc-=0x30;//ASCII2432
	transparent=Transparent;
	if (asc>(sizeof(ASCII1224)/48))//ASC����Ч�ж�
	{
		return;
	}

	buf=(u8*)(&ASCII2432[asc][0]);
	SetTFTStyleH();//���ú�

	for (i=0;i<24;i++)//24��
	{
		font_byte=((u16*)buf)[i];
		TFTSetXY(x,y);
		for (j=0;j<12;j++)//ÿ��12����
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//д��Ч��
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//д��ɫ
				else
					TFTSetXY(x+j+1,y);	//ƫ��һ����
			}
			font_byte>>=1;
		}
		y++;
	}
}
/**************************************************************
** ������:TFTShowString16/24 /P
** ����:��ʾ16/24��߶ȵ��ַ���
** ע������:��ȡ16/24�����ֵĳ���������,һ���Ǵ����ֿ�,һ������FLASH�ֿ� ���ݺ궨���жϲ�������
**  		���е��ж��Ժ��ټ���
**  		op����:b0:0��͸����,û�ֵĵط�д��ɫ. 1͸����,û�ֵĵط�����(09.8.5)
***************************************************************/
void TFTShowString16(LOC_X x,LOC_Y y,char* str,COLOR color,u8 op)
{
	u16 hz;
	if(!op)
		Transparent=FALSE;
	else
		Transparent=TRUE;
	SetBusWrite();//���߷���
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
			TFTShowASC816(x,y,*str,color);
//            TFTShowASC1616(x,y,*str,color);
			x += 8 ;                                
			str += 1 ;
		}
		else if (*str >= 0xb0)//����
		{
			hz=*str++;
			hz+=(*str++)<<8;
			TFTShowHZ16(x,y,hz,color);
			x += 16;
		}
		/*
		else if (*str == '\n')//����
		{
		   y += 16 ;				
		   str += 1 ;
		}
		else if (*str == '\b')//�ո�
		{	
			x += 8 ;
			str += 1 ;		
		}
		*/
	}       
}
/*
void TFTShowString24(LOC_X x,LOC_Y y,char* str,COLOR color,u8 op)
{
	u16 hz;
	if(!op)
		Transparent=FALSE;
	else
		Transparent=TRUE;
	SetBusWrite();//���߷���
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
//            TFTShowASC1616(x,y,*str,color);
			x += 30 ;                               
			str += 1 ;
		}

		else if (*str >= 0xb0)//����
		{
			hz=*str++;
			hz+=(*str++)<<8;
			TFTShowHZ24(x,y,hz,color);
			x += 24;
		}

	}   
}*/
/*******************************************************************************
* Function Name  : TFTShowString24
* Description    : ��ʾASCII 16*25������24*24
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TFTShowString1624(LOC_X x,LOC_Y y,char* str,COLOR color,u8 op)
{
	u16 hz;
	if(!op)
		Transparent=FALSE;
	else
		Transparent=TRUE;
	SetBusWrite();//���߷���
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
            TFTShowASC1625(x,y,*str,color);
			x += 16 ;                               
			str += 1 ;
		}

		else if (*str >= 0xb0)//����
		{
			hz=*str++;
			hz+=(*str++)<<8;
			TFTShowHZ24(x,y,hz,color);
			x += 24;
		}
	}   
}
/*******************************************************************************
* Function Name  : TFTShowString24
* Description    : ��ʾASCII 24*32������24*24
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TFTShowString24(LOC_X x,LOC_Y y,char* str,COLOR color,u8 op)
{
	u16 hz;
	if(!op)
		Transparent=FALSE;
	else
		Transparent=TRUE;
	SetBusWrite();//���߷���
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
            TFTShowASC2432(x,y,*str,color);
			x += 22 ;                               
			str += 1 ;
		}

		else if (*str >= 0xb0)//����
		{
			hz=*str++;
			hz+=(*str++)<<8;
			TFTShowHZ24(x,y,hz,color);
			x += 24;
		}
	}   
}
/*******************************************************************************
* Function Name  : TFTShowString32
* Description    : ��ʾASCII 32*45������24*24
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TFTShowString32(LOC_X x,LOC_Y y,char* str,COLOR color,u8 op)
{
	u16 hz;
	if(!op)
		Transparent=FALSE;
	else
		Transparent=TRUE;
	SetBusWrite();//���߷���
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
            TFTShowASC3245(x,y,*str,color);
			x += 30 ;                               
			str += 1 ;
		}

		else if (*str >= 0xb0)//����
		{
			hz=*str++;
			hz+=(*str++)<<8;
			TFTShowHZ24(x,y,hz,color);
			x += 24;
		}
	}   
}
/**************************************************************
** ������:TFTShowBmp
** ����:��BMP
** ע������:�涨����BUF���Ⱥ����������
***************************************************************/
void TFTShowBmp(LOC_X x0,LOC_Y y0,LOC_X length,LOC_Y height,COLOR* buf)
{
	LOC_X i,length_null=0;
	LOC_Y j;

	if((!length)||(!height))
	{
		return;
	}
	CHECK_LOC(x0,y0);

	//���������Ļ��Χ,����㳬���ĳ���
	if(x0+length>=MAX_X)
	{
		length_null=x0+length-MAX_X;
		length=MAX_X-x0;
 	}
	
	//���������Ļ��Χ,�������õĸ߶�
	if(y0+height>=MAX_Y)
	{
		height=MAX_Y-y0;
	}

	SetTFTStyleH();		//���ú�
	for(j=0;j<height;j++)
	{
		TFTSetXY(x0,y0);
		for(i=0;i<length;i++)
		{
			_TFTPoint(*buf++);
		}
		y0++;
		buf+=length_null;
	}
}

//��ʼ��LCDӲ��
void Lcd_Init(void)
{
	lcd_initialize();


	//����*_*
/*	TFTFill(GREEN);
	SysTickDelay(200);
	TFTFill(BLUE);
	SysTickDelay(200);
	TFTFill(YELLOW);
	SysTickDelay(200);
	TFTFill(CYAN);
	SysTickDelay(200);
	TFTFill(GRAY1);
	SysTickDelay(200);
*/

}
