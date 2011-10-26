/**************************************************************
GUI部分,前部分为基础画函数,后部分为管理函数
***************************************************************/
#define TFTdriver_c
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "TFTdriver.h"
#include "font_data.h"
#include "GuiDef.h"
#include "hal.h"
#include "TFTHal.h"

COLOR BG_COLOR;	//全局的背景色,此变量是为了增加运行速度而设置的

/**************************************************************
** 函数名:TFTFill
** 功能:向TFT填充颜色
** 注意事项:从地址0开始填充
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
** 函数名:TFTPoint
** 功能:画一个点
** 注意事项:暂时没怎么用过~
***************************************************************/
void TFTPoint(LOC_X x,LOC_Y y,COLOR color)
{
	CHECK_LOC(x,y);
	//总线方向
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


	wr_cmd(0x0022);//开始写
	wr_data(color);
}
/**************************************************************
** 函数名:TFTSetXY/_TFTPoint
** 功能:设置XY,并写入一个点
** 注意事项:把TFTPoint函数分为两部分,在某些场合下,合理利用这两个函数,可提高效率 
** 			*_*为了再提高效率这个函数不设置总线方向,应在调用前设置.
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

	wr_cmd(0x0022);//开始写
}
static void _TFTPoint(COLOR color)
{
	wr_data(color);
}

/**************************************************************
** 函数名:SetTFTStyleH/V
** 功能:设置TFT控制芯片的横竖向
** 注意事项:这个是此TFTLCD特有的机制,使用这个机制可以实现更简单的横竖线绘制,缺点是移植性降低
***************************************************************/
static u8 HV_Style=0;	//代表横竖方向0是横,1是竖,开始的时候默认为横
static void SetTFTStyleH(void)
{
	if(HV_Style)
	{
		HV_Style=0;//设置横
	#if (SSD1297==1)||(SSD1289==1)
		wr_cmd(0x0011);
		wr_data(0x6030);
	#elif (R6150V==1)||(R6150U==1)
		wr_cmd(0x03);
		wr_data(0x5030);
	#elif (HX8347==1)
		wr_cmd(0x0016);
		wr_data(0x00c8);
		wr_cmd(0x0004);wr_data(0x0000);   // 行末239
		wr_cmd(0x0005);wr_data(0x00EF);
		wr_cmd(0x0008);wr_data(0x0001);   // 列末319
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
		HV_Style=1;//设置竖
	#if (SSD1297==1)||(SSD1289==1)
		wr_cmd(0x0011);
		wr_data(0x6038);
	#elif (R6150V==1)||(R6150U==1)
		wr_cmd(0x03);
		wr_data(0x5028);
	#elif (HX8347==1)
		wr_cmd(0x0016);
		wr_data(0x00e8);
		wr_cmd(0x0004);wr_data(0x0001);   // 行末319
		wr_cmd(0x0005);wr_data(0x003F);
		wr_cmd(0x0008);wr_data(0x0000);   // 列末239
		wr_cmd(0x0009);wr_data(0x00EF);
	#elif (ST7781==1)||(ILI9328==1)
		wr_cmd(0x03);
		wr_data(0x5038);
	#endif
	}
}
/**************************************************************
** 函数名:TFTHLine
** 功能:画一条横线
** 注意事项:这里利用了控制芯片的便利,通过改写方向的方式画线,只需要设置一次坐标点,则可实现线条的连续写. 
** 			VLINE函数也如此
***************************************************************/
void TFTHLine(LOC_X x,LOC_Y y,u16 len,COLOR color)
{
	u16 i;
	CHECK_LOC(x,y);
	if(len+x>MAX_X)
	{
		len=MAX_X-x;
	}
	//总线方向
	SetBusWrite();
	//换为横向
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
	//开始写
	wr_cmd(0x0022);
	for(i=0;i<len;i++)
	{
		wr_data(color);
	}
}


/**************************************************************
** 函数名:TFTVLine
** 功能:画一条竖线
** 注意事项:参考HLINE
***************************************************************/
void TFTVLine(LOC_X x,LOC_Y y,u16 len,COLOR color)
{
	u16 i;
	CHECK_LOC(x,y);
	if(len+y>MAX_Y)
	{
		len=MAX_Y-y;
	}
	//总线方向
	SetBusWrite();
	//转为竖向
	SetTFTStyleV();
	//坐标
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
	
//这个屏当换方向的时候,XY的意义也会跟着变换
	wr_cmd(0x02);
	wr_data(y>>8);
	wr_cmd(0x03);
	wr_data((u8)y);
	
	wr_cmd(0x06);
	wr_data(0);
	wr_cmd(0x07);
	wr_data(x);
#endif
	
	//开始写
	wr_cmd(0x22);
	for(i=0;i<len;i++)
	{
		wr_data(color);
	}
}

/**************************************************************
** 函数名:TFTRect
** 功能:画一个空矩形
** 注意事项:调用横竖线条的函数,长宽参数不可为0
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
** 函数名:TFTRectFill
** 功能:填充巨型
** 注意事项:调用画线函数,长宽参数不可为0
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
** 函数名:TFTShowHZ16
** 功能:显示汉字.内部调用
** 注意事项:
***************************************************************/
static bool Transparent;//透明允许

#if FONT_HZ_TYPE==FONT_HZ_TYPE_CODE	//CODE内字库
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
	

	SetTFTStyleH();//设置横
	transparent=Transparent;
	for (k = 0;Hz_Code16[k].Hz_Index[0] != 0xff;k++)
	{
		if ( (u8)hz == Hz_Code16[k].Hz_Index[0] )
		{
			if ( (hz>>8) == Hz_Code16[k].Hz_Index[1] )
			{
				buf=(u8*)(&(Hz_Code16[k].Code[0]));

				for (i=0;i<16;i++)//16行
				{
					font_byte=((u16*)buf)[i];

					TFTSetXY(x,y);
					for (j=0;j<16;j++)//每行16个点
					{
						if (font_byte&0x01)
						{
							_TFTPoint(color);	//写有效点
						}
						else
						{
							if (transparent==FALSE)
								_TFTPoint(BG_COLOR);//写底色
							else
								TFTSetXY(x+j+1,y);	//偏移一个点
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

	SetTFTStyleH();//设置横
	
	transparent=Transparent;
	for (k = 0;Hz_Code24[k].Hz_Index[0] != 0xff;k++)
	{
		if ( (u8)hz == Hz_Code24[k].Hz_Index[0] )
		{
			if ( (hz>>8) == Hz_Code24[k].Hz_Index[1] )
			{
				buf=(u8*)(&(Hz_Code24[k].Code[0]));

				for (i=0;i<24;i++)//24行
				{
					font_byte=*buf++;
					font_byte+=(*buf++)<<8;
					font_byte+=(*buf++)<<16;

					TFTSetXY(x,y);
					for (j=0;j<24;j++)//每行24个点
					{
						if (font_byte&0x01)
						{
							_TFTPoint(color);	//写有效点
						}
						else
						{
							if (transparent==FALSE)
								_TFTPoint(BG_COLOR);//写底色
							else
								TFTSetXY(x+j+1,y);	//偏移一个点
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

extern void SST25Read(u32 addr,u8* p_data,u32 no);		//需要使用SST的读FLASH函数
static void TFTShowHZ16(LOC_X x,LOC_Y y,u16 hz,COLOR color)
{
	u16 font_byte;
	u8  i,j;
	u8 c1,c2;
	u32 addr;
	u16 _HZ16_Buf[16];//汉字字库缓冲
	bool transparent;
	
	if (x>MAX_X-16)
		return;
	if (y>MAX_Y-16)
		return;
	SetTFTStyleH();//设置横
	transparent=Transparent;
	c1=(u8)hz;
	c2=hz>>8;
	addr=HZ_DOT16_START_ADDR+((c1-0xb0)*94+(c2-0xa1))*32;
//    addr=HZ_DOT16_START_ADDR+((c1-0xa1)*94+(c2-0xa1))*32;
	SST25Read(addr,(u8*)_HZ16_Buf,32);
	for (i=0;i<16;i++)//16行
	{
		TFTSetXY(x,y);
		font_byte=_HZ16_Buf[i];
		for (j=0;j<16;j++)//每行16个点
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//写有效点
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//写底色
				else
					TFTSetXY(x+j+1,y);	//偏移一个点
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
	u8 HZ24_Buf[72];//汉字字库缓冲(采用半字形式,所以必须两字节对齐)
	u8 *buf=HZ24_Buf;
	bool transparent;
	if (x>MAX_X-16)
		return;
	if (y>MAX_Y-16)
		return;
	SetTFTStyleH();//设置横
	transparent=Transparent;
	c1=(u8)hz;
	c2=hz>>8;
	addr=HZ_DOT24_START_ADDR+((c1-0xb0)*94+(c2-0xa1))*72;
	SST25Read(addr,HZ24_Buf,72);

	for (i=0;i<24;i++)//24行
	{
		font_byte=*buf++;
		font_byte+=(*buf++)<<8;
		font_byte+=(*buf++)<<16;

		TFTSetXY(x,y);
		for (j=0;j<24;j++)//每行24个点
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//写有效点
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//写底色
				else
					TFTSetXY(x+j+1,y);	//偏移一个点
			}
			font_byte>>=1;
		}
		y++;
	}
}

#else
	#error("Must set one hz type!")
#endif
//外部flash字库
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
//	if ( asc> (sizeof(ASCII816)/16) ) //ASC码有效判断
//	{
//		return;
//	}
    addr=HZ_DOT16_START_ADDR+((0xa3-0xa1)*94+(c2-0xa1))*32;
//    addr=HZ_DOT16_START_ADDR+0;//(asc - 32)*16;
	SST25Read(addr,(u8*)ASC16_Buf,32);
    buf = ASC16_Buf;
//	buf=(u8*)(&ASCII816[asc][0]);
	SetTFTStyleH();//设置横

	for (i=0;i<16;i++)//16行
	{
		font_byte=*buf++;
        /***********************************/
        temp1 = (font_byte>>8)&0x00ff;   //横向取模，字节倒序
        temp2 = (font_byte<<8)&0xff00;
        font_byte = temp1|temp2;
        /***********************************/
		TFTSetXY(x,y);
		for (j=0;j<16;j++)//每行8个点
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//写有效点
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//写底色
				else
					TFTSetXY(x+j+1,y);	//偏移一个点
			}
			font_byte>>=1;
		}
		y++;
	}
}
//内部flash字库
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

	if ( asc> (sizeof(ASCII816)/16) ) //ASC码有效判断
	{
		return;
	}

	buf=(u8*)(&ASCII816[asc][0]);
	SetTFTStyleH();//设置横

	for (i=0;i<16;i++)//16行
	{
		font_byte=*buf++;
		TFTSetXY(x,y);
		for (j=0;j<8;j++)//每行8个点
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//写有效点
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//写底色
				else
					TFTSetXY(x+j+1,y);	//偏移一个点
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
	if (asc>(sizeof(ASCII3245)/180))//ASC码有效判断
	{
		return;
	}

	buf=(u8*)(&ASCII3245[asc][0]);
	SetTFTStyleH();//设置横

	for (i=0;i<45;i++)//24行
	{

            font_byte=((u32*)buf)[i];
            TFTSetXY(x,y);
            for (j=0;j<32;j++)//每行12个点
            {
                if (font_byte&0x01)
                {
                    _TFTPoint(color);	//写有效点
                }
                else
                {
                    if (transparent==FALSE)
                        _TFTPoint(BG_COLOR);//写底色
                    else
                        TFTSetXY(x+j+1,y);	//偏移一个点
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
	if (asc>(sizeof(ASCII2432)/96))//ASC码有效判断
	{
		return;
	}

	buf=(u8*)(&ASCII2432[asc][0]);
	SetTFTStyleH();//设置横

	for (i=0;i<32;i++)//32行
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
            for (j=0;j<24;j++)//每行24个点
            {
                if (font_byte&0x01)
                {
                    _TFTPoint(color);	//写有效点
                }
                else
                {
                    if (transparent==FALSE)
                        _TFTPoint(BG_COLOR);//写底色
                    else
                        TFTSetXY(x+j+1,y);	//偏移一个点
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
	if (asc>(sizeof(ASCII1625)/50))//ASC码有效判断
	{
		return;
	}

	buf=(u8*)(&ASCII1625[asc][0]);
	SetTFTStyleH();//设置横

	for (i=0;i<25;i++)//24行
	{

            font_byte=((u16*)buf)[i];
            TFTSetXY(x,y);
            for (j=0;j<16;j++)//每行12个点
            {
                if (font_byte&0x01)
                {
                    _TFTPoint(color);	//写有效点
                }
                else
                {
                    if (transparent==FALSE)
                        _TFTPoint(BG_COLOR);//写底色
                    else
                        TFTSetXY(x+j+1,y);	//偏移一个点
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
	if (asc>(sizeof(ASCII1224)/48))//ASC码有效判断
	{
		return;
	}

	buf=(u8*)(&ASCII2432[asc][0]);
	SetTFTStyleH();//设置横

	for (i=0;i<24;i++)//24行
	{
		font_byte=((u16*)buf)[i];
		TFTSetXY(x,y);
		for (j=0;j<12;j++)//每行12个点
		{
			if (font_byte&0x01)
			{
				_TFTPoint(color);	//写有效点
			}
			else
			{
				if (transparent==FALSE)
					_TFTPoint(BG_COLOR);//写底色
				else
					TFTSetXY(x+j+1,y);	//偏移一个点
			}
			font_byte>>=1;
		}
		y++;
	}
}
/**************************************************************
** 函数名:TFTShowString16/24 /P
** 功能:显示16/24点高度的字符串
** 注意事项:提取16/24点阵汉字的程序有两种,一种是代码字库,一种是外FLASH字库 根据宏定义判断采用哪种
**  		换行等判断以后再加上
**  		op参数:b0:0非透明字,没字的地方写底色. 1透明字,没字的地方忽略(09.8.5)
***************************************************************/
void TFTShowString16(LOC_X x,LOC_Y y,char* str,COLOR color,u8 op)
{
	u16 hz;
	if(!op)
		Transparent=FALSE;
	else
		Transparent=TRUE;
	SetBusWrite();//总线方向
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
			TFTShowASC816(x,y,*str,color);
//            TFTShowASC1616(x,y,*str,color);
			x += 8 ;                                
			str += 1 ;
		}
		else if (*str >= 0xb0)//汉字
		{
			hz=*str++;
			hz+=(*str++)<<8;
			TFTShowHZ16(x,y,hz,color);
			x += 16;
		}
		/*
		else if (*str == '\n')//换行
		{
		   y += 16 ;				
		   str += 1 ;
		}
		else if (*str == '\b')//空格
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
	SetBusWrite();//总线方向
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
//            TFTShowASC1616(x,y,*str,color);
			x += 30 ;                               
			str += 1 ;
		}

		else if (*str >= 0xb0)//汉字
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
* Description    : 显示ASCII 16*25，汉字24*24
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
	SetBusWrite();//总线方向
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
            TFTShowASC1625(x,y,*str,color);
			x += 16 ;                               
			str += 1 ;
		}

		else if (*str >= 0xb0)//汉字
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
* Description    : 显示ASCII 24*32，汉字24*24
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
	SetBusWrite();//总线方向
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
            TFTShowASC2432(x,y,*str,color);
			x += 22 ;                               
			str += 1 ;
		}

		else if (*str >= 0xb0)//汉字
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
* Description    : 显示ASCII 32*45，汉字24*24
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
	SetBusWrite();//总线方向
	while ((*str != '\0'))
	{
		if ((*str <= 0x80) && (*str >= 0x20)) // ASCII	
		{
            TFTShowASC3245(x,y,*str,color);
			x += 30 ;                               
			str += 1 ;
		}

		else if (*str >= 0xb0)//汉字
		{
			hz=*str++;
			hz+=(*str++)<<8;
			TFTShowHZ24(x,y,hz,color);
			x += 24;
		}
	}   
}
/**************************************************************
** 函数名:TFTShowBmp
** 功能:画BMP
** 注意事项:规定点阵BUF是先横后竖的排列
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

	//如果超出屏幕范围,则计算超出的长度
	if(x0+length>=MAX_X)
	{
		length_null=x0+length-MAX_X;
		length=MAX_X-x0;
 	}
	
	//如果超出屏幕范围,则计算可用的高度
	if(y0+height>=MAX_Y)
	{
		height=MAX_Y-y0;
	}

	SetTFTStyleH();		//设置横
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

//初始化LCD硬件
void Lcd_Init(void)
{
	lcd_initialize();


	//测试*_*
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
