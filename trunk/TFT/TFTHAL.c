#define TFTHAL_c
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
//#include "hal.h"
#include "TFTHal.h"

//为了加快速度,这里采用直接操作寄存器的方式  
#define RSTTFT_H	GPIOB->BSRR = GPIO_Pin_11
#define RSTTFT_L	GPIOB->BRR=GPIO_Pin_11

#define CSTFT_H		GPIOB->BSRR=GPIO_Pin_8
#define CSTFT_L 	GPIOB->BRR=GPIO_Pin_8

#define A0_H		GPIOC->BSRR=GPIO_Pin_4
#define A0_L        GPIOC->BRR=GPIO_Pin_4

#define WRTFT_H		GPIOC->BSRR=GPIO_Pin_2
#define WRTFT_L     GPIOC->BRR=GPIO_Pin_2

#define RDTFT_H		GPIOC->BSRR=GPIO_Pin_3
#define RDTFT_L     GPIOC->BRR=GPIO_Pin_3



//设置DATA
#define SET_DATATFT(dat)		GPIOE->ODR=(dat)
#define SET_DATA_OUT		GPIOE->CRL=0x33333333	/*推挽输出50MHZ速度?*/

void TFT_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE);

	/* PB8-cs,11-rst输出 */
	GPIO_SetBits(GPIOB, GPIO_Pin_8);		//预置高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	/*PC2,3,4,5,输出*/
	GPIO_SetBits(GPIOC, GPIO_Pin_2|GPIO_Pin_3);		//RD,WR,预置高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*PE0-15 DATA先设置为输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOE, &GPIO_InitStructure);

}



void wr_cmd(u16 cmd)
{
	A0_L;
	CSTFT_L;
	WRTFT_L;
	SET_DATATFT(cmd);
	//__nop();
	WRTFT_H;
	CSTFT_H;
}

void wr_data(u16 dat)
{
	A0_H;
	CSTFT_L;
	WRTFT_L;
	SET_DATATFT(dat);
	//__nop();
	WRTFT_H;
	CSTFT_H;
}
//定义虚拟总线的读写状态,方便复用
void SetBusWrite(void)
{
	SET_BUS_OUT;
	
}
void SetBusRead(void)
{
	SET_BUS_IN;
}


//初始化
void lcd_initialize(void)
{
	SET_DATA_OUT;//设置输出方向

	RSTTFT_L;
	SysTickDelay(80);
	RSTTFT_H;
	SysTickDelay(80);
	SetBusWrite();
#ifdef R6150V	/*R6150V系列的屏*/

	wr_cmd(0x00A4);	wr_data(0x0001);
	
	SysTickDelay(40);
	
	wr_cmd(0x008);	wr_data(0x0808);
	wr_cmd(0x0009);	wr_data(0x0900);
	
	wr_cmd(0x0030);	wr_data(0x0F10);
	wr_cmd(0x0031);	wr_data(0x770F);
	wr_cmd(0x0032);	wr_data(0x0407);
	wr_cmd(0x0033);	wr_data(0x0F0F);
	wr_cmd(0x0034);	wr_data(0x2222);
	wr_cmd(0x0035);	wr_data(0x0F0F);
	wr_cmd(0x0036);	wr_data(0x7704);
	wr_cmd(0x0037);	wr_data(0x0F07);
	wr_cmd(0x0038);	wr_data(0x100F);
	wr_cmd(0x0039);	wr_data(0x2222);
	
	wr_cmd(0x0007);	wr_data(0x0001);
	wr_cmd(0x0010);	wr_data(0x0730);
	wr_cmd(0x0011);	wr_data(0x0237);
	
	SysTickDelay(50);
	
	wr_cmd(0x0012);	wr_data(0x11B6);
	wr_cmd(0x0013);	wr_data(0x1500);
	wr_cmd(0x0029);	wr_data(0x0040);
	
	SysTickDelay(30);
	
	wr_cmd(0x0001);	wr_data(0x0100);
	wr_cmd(0x0002);	wr_data(0x0200);
	wr_cmd(0x0003);	wr_data(0x1030);
	wr_cmd(0x000A);	wr_data(0x0008);
	wr_cmd(0x000C);	wr_data(0x0000);
	wr_cmd(0x000D);	wr_data(0x0000);
	wr_cmd(0x000E);	wr_data(0x0030);
	wr_cmd(0x0020);	wr_data(0x0000);
	wr_cmd(0x0021);	wr_data(0x0000);
	wr_cmd(0x0050);	wr_data(0x0000);
	wr_cmd(0x0051);	wr_data(0x00EF);
	wr_cmd(0x0052);	wr_data(0x0000);
	wr_cmd(0x0053);	wr_data(0x013F);
	
	wr_cmd(0x0060);
	#if TFT_TYPE==1
		wr_data(0x2700);		//一号屏
	#elif TFT_TYPE==2
		wr_data(0xa700);		//二号屏
	#endif
	
	wr_cmd(0x0061);	wr_data(0x0001);
	wr_cmd(0x006A);	wr_data(0x0000);
	wr_cmd(0x0080);	wr_data(0x0000);
	wr_cmd(0x0081);	wr_data(0x0000);
	wr_cmd(0x0082);	wr_data(0x0000);
	
	wr_cmd(0x0090);	wr_data(0x0016);	
	wr_cmd(0x0092);	wr_data(0x0100);	
	wr_cmd(0x0093);	wr_data(0x0201);
	wr_cmd(0x0094);	wr_data(0x0001);
	wr_cmd(0x009C);	wr_data(0x0022);
	wr_cmd(0x0095);	wr_data(0x0A12);
	wr_cmd(0x0097);	wr_data(0x0200);
	wr_cmd(0x0098);	wr_data(0x0101);
	wr_cmd(0x0099);	wr_data(0x0001);
	
	SysTickDelay(10);
	
	wr_cmd(0x0007);	wr_data(0x0100);
	
	wr_cmd(0x0022);
	
	SysTickDelay(200);
#endif 

#ifdef R6150U

	//R61505U控制器
	wr_cmd(0x0007);	wr_data(0x0000);
	wr_cmd(0x0012);	wr_data(0x0000);
	
	SysTickDelay(30);
	
	wr_cmd(0x00A4);	wr_data(0x0001);
	
	SysTickDelay(60);
	
	wr_cmd(0x0060);
	#if TFT_TYPE==1
		wr_data(0x2700);		//一号屏
	#elif TFT_TYPE==2
		wr_data(0xa700);		//二号屏
	#endif

	wr_cmd(0x0030);	wr_data(0x0307);
	wr_cmd(0x0031);	wr_data(0x0303);
	wr_cmd(0x0032);	wr_data(0x0603);
	wr_cmd(0x0033);	wr_data(0x0202);
	wr_cmd(0x0034);	wr_data(0x0202);
	wr_cmd(0x0035);	wr_data(0x0202);
	wr_cmd(0x0036);	wr_data(0x1F1F);
	wr_cmd(0x0037);	wr_data(0x0303);
	wr_cmd(0x0038);	wr_data(0x0303);
	wr_cmd(0x0039);	wr_data(0x0603);
	wr_cmd(0x003A);	wr_data(0x0202);
	wr_cmd(0x003B);	wr_data(0x0102);
	wr_cmd(0x003C);	wr_data(0x0204);
	wr_cmd(0x003D);	wr_data(0x0000);
	
	wr_cmd(0x0007);	wr_data(0x0001);
	wr_cmd(0x0017);	wr_data(0x0001);
	wr_cmd(0x0010);	wr_data(0x16B0);
	wr_cmd(0x0011);	wr_data(0x0111);
	
	SysTickDelay(100);
	
	wr_cmd(0x0012);	wr_data(0x0118);
	wr_cmd(0x0013);	wr_data(0x1400);
	wr_cmd(0x0029);	wr_data(0x000C);
	wr_cmd(0x0012);	wr_data(0x01B8);
	
	SysTickDelay(50);
	
	wr_cmd(0x0001);	wr_data(0x0100);
	wr_cmd(0x0002);	wr_data(0x0300);
	wr_cmd(0x0003);	wr_data(0x5030);
	wr_cmd(0x0004);	wr_data(0x0000);
	wr_cmd(0x0009);	wr_data(0x0000);
	wr_cmd(0x000A);	wr_data(0x0008);
	wr_cmd(0x000C);	wr_data(0x0000);
	wr_cmd(0x000D);	wr_data(0x0000);
	wr_cmd(0x0015);	wr_data(0x8000);
	
	wr_cmd(0x0050);	wr_data(0x0000);
	wr_cmd(0x0051);	wr_data(0x00EF);
	wr_cmd(0x0052);	wr_data(0x0000);
	wr_cmd(0x0053);	wr_data(0x013F);
	
	wr_cmd(0x0061);	wr_data(0x0001);
	wr_cmd(0x0090);	wr_data(0x0010);	//刷新率,越小刷新率越快,但不能小于0x10
	wr_cmd(0x0092);	wr_data(0x0100);
	wr_cmd(0x0093);	wr_data(0x0001);
	
	SysTickDelay(30);
	
	wr_cmd(0x0007);	wr_data(0x0021);
	
	SysTickDelay(30);
	
	wr_cmd(0x0007);	wr_data(0x0061);
	
	SysTickDelay(20);
	
	wr_cmd(0x0007);	wr_data(0x0173);
	
	SysTickDelay(100);
#endif 
	
#ifdef SSD1297
	wr_cmd(0x0028);wr_data(0x0006);
	wr_cmd(0x0000);wr_data(0x0001);
	wr_cmd(0x0010);wr_data(0x0000);
	wr_cmd(0x0007);wr_data(0x0033);
	wr_cmd(0x0011);wr_data(0x6030);//0x6828);//6030
	wr_cmd(0x0001);wr_data(0x2b3f);//2b3f
	wr_cmd(0x000E);wr_data(0x2100);//2c VA电压-设置浅色点
	wr_cmd(0x0003);wr_data(0xaaa8);
	wr_cmd(0x000F);wr_data(0x0000);
	wr_cmd(0x001E);wr_data(0x00a0);//bf vcomh-设置浅色点
	wr_cmd(0x000C);wr_data(0x0005);
	wr_cmd(0x000D);wr_data(0x0004);//08
	wr_cmd(0x000B);wr_data(0x5310);
	wr_cmd(0x0002);wr_data(0x0600);
	wr_cmd(0x0025);wr_data(0x8000);//刷新率
	//windows setting 
	wr_cmd(0x0044);wr_data(0xef00);//ef00
	wr_cmd(0x0045);wr_data(0x0000);
	wr_cmd(0x0046);wr_data(0x013f);
	
	wr_cmd(0x004e);wr_data(0x00ef);//ef
	wr_cmd(0x004f);wr_data(0x0000);
	
	
	wr_cmd(0x0030);wr_data(0x0001); 
	wr_cmd(0x0031);wr_data(0x0007);
	wr_cmd(0x0032);wr_data(0x0F0F); //0f0f
	
	wr_cmd(0x0033);wr_data(0x0000); 
	
	wr_cmd(0x0034);wr_data(0x0000); //0000
	wr_cmd(0x0035);wr_data(0x0002); 
	wr_cmd(0x0036);wr_data(0x0503); 
	
	wr_cmd(0x0037);wr_data(0x0000); 
	
	wr_cmd(0x003a);wr_data(0x1F00); //1F00//0B02
	wr_cmd(0x003b);wr_data(0x000F); //000F//0406
	
	wr_cmd(0x0022);  

#endif

#ifdef SSD1289 

	wr_cmd(0x0025);	wr_data(0xd000); 
	wr_cmd(0x0000);	wr_data(0x0001); 
	 
	SysTickDelay(40); 
	 
	wr_cmd(0x0003);	wr_data(0xEEEE);  
	wr_cmd(0x000C);	wr_data(0x0004); 
	wr_cmd(0x000D);	wr_data(0x000f); 
	wr_cmd(0x000E);	wr_data(0x2100); //3000 VA VL=0.9475vcomh-va
	 
	SysTickDelay(100); 
	 
	wr_cmd(0x001E);	wr_data(0x00a0); //vcomh af
	wr_cmd(0x0001);	wr_data(0x2B3F);  
	wr_cmd(0x0002);	wr_data(0x0600); 
	wr_cmd(0x0010);	wr_data(0x0000); 
	wr_cmd(0x0011);	wr_data(0x6030);  
	wr_cmd(0x0005);	wr_data(0x0000); 
	wr_cmd(0x0006);	wr_data(0x0000); 
	wr_cmd(0x0007);	wr_data(0x0001); 
	wr_cmd(0x0007); wr_data(0x0021); 
	wr_cmd(0x0007); wr_data(0x0023); 
	wr_cmd(0x0007); wr_data(0x0033); 
	wr_cmd(0x000B); wr_data(0x0000);  
	wr_cmd(0x000F); wr_data(0x0000); 
	
	SysTickDelay(100); 
	
	wr_cmd(0x0041); 	wr_data(0x0000); 
	wr_cmd(0x0042); 	wr_data(0x0000); 
	wr_cmd(0x0048); 	wr_data(0x0000); 
	wr_cmd(0x0049); 	wr_data(0x013F); 
	wr_cmd(0x004A); 	wr_data(0x0000); 
	wr_cmd(0x004B); 	wr_data(0x0000); 
	wr_cmd(0x0044); 	wr_data(0xEF00); 
	wr_cmd(0x004E); 	wr_data(0x0000); 
	wr_cmd(0x004F); 	wr_data(0x0000); 
	wr_cmd(0x0045); 	wr_data(0x0000); 
	wr_cmd(0x0046); 	wr_data(0x013F); 
	wr_cmd(0x0023); 	wr_data(0x0000); 
	wr_cmd(0x0024); 	wr_data(0x0000); 
	wr_cmd(0x0021); 	wr_data(0x0000); 
	
	wr_cmd(0x0030); 	wr_data(0x0707); 
	wr_cmd(0x0031); 	wr_data(0x0204); 
	wr_cmd(0x0032); 	wr_data(0x0204); 
	wr_cmd(0x0033); 	wr_data(0x0502); 
	wr_cmd(0x0034); 	wr_data(0x0507); 
	wr_cmd(0x0035); 	wr_data(0x0204); 
	wr_cmd(0x0036); 	wr_data(0x0204); 
	wr_cmd(0x0037); 	wr_data(0x0502); 
	wr_cmd(0x003A); 	wr_data(0x0302); 
	wr_cmd(0x003B); 	wr_data(0x000F); 
	 
	wr_cmd(0x0023); 	wr_data(0x0000); 
	wr_cmd(0x0024); 	wr_data(0x0000); 
	wr_cmd(0x0028); 	wr_data(0x0006); 
	wr_cmd(0x002F); 	wr_data(0x122E); 

	wr_cmd(0x0022); 

	SysTickDelay(200); 
#elif defined HX8347
	// ****************Gamma for CMO 2.8" ***************** 

	wr_cmd(0x0046);wr_data(0x0095);   //  
	wr_cmd(0x0047);wr_data(0x0051);   //  
	wr_cmd(0x0048);wr_data(0x0000);   //  
	wr_cmd(0x0049);wr_data(0x0036);   //  
	wr_cmd(0x004A);wr_data(0x0011);   //  
	wr_cmd(0x004B);wr_data(0x0066);   //  
	wr_cmd(0x004C);wr_data(0x0014);   //  
	wr_cmd(0x004D);wr_data(0x0077);   //  
	wr_cmd(0x004E);wr_data(0x0013);   //  
	wr_cmd(0x004F);wr_data(0x004C);   //  
	wr_cmd(0x0050);wr_data(0x0046);   //  
	wr_cmd(0x0051);wr_data(0x0046);   //             
	//**************240x320 window setting ******************** 
	 
	wr_cmd(0x0002);wr_data(0x0000);   // Column address start2  
	wr_cmd(0x0003);wr_data(0x0000);   // Column address start1  
	 
	wr_cmd(0x0004);wr_data(0x0000);   // Column address end2  
	wr_cmd(0x0005);wr_data(0x00EF);   // Column address end1  
	 
	wr_cmd(0x0006);wr_data(0x0000);   // Row address start2  
	wr_cmd(0x0007);wr_data(0x0000);   // Row address start1  
	 
	wr_cmd(0x0008);wr_data(0x0001);   // Row address end2  
	wr_cmd(0x0009);wr_data(0x003F);   // Row address end1  
	
	wr_cmd(0x0090);wr_data(0x007F);   // SAP=0111 1111          
	// ****************Display Setting *************************         
        
	wr_cmd(0x0001);wr_data(0x0006);      // IDMON=0, INVON=1, NORON=1, PTLON=0  
	wr_cmd(0x0016);wr_data(0x00c8);      //  //MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0   
	
	wr_cmd(0x0023);wr_data(0x0095);   // N_DC=1001 0101  
	wr_cmd(0x0024);wr_data(0x0095);   // P_DC=1001 0101  
	wr_cmd(0x0025);wr_data(0x00FF);   // I_DC=1111 1111          
	
	wr_cmd(0x0027);wr_data(0x0006);   // N_BP=0000 0110  
	wr_cmd(0x0028);wr_data(0x0006);   // N_FP=0000 0110  
	wr_cmd(0x0029);wr_data(0x0006);   // P_BP=0000 0110  
	wr_cmd(0x002A);wr_data(0x0006);   // P_FP=0000 0110  
	wr_cmd(0x002C);wr_data(0x0006);   // I_BP=0000 0110          
	wr_cmd(0x002D);wr_data(0x0006);   // I_FP=0000 0110          
	 
	wr_cmd(0x003A);wr_data(0x0001);   // N_RTN=0000, N_NW=001          
	wr_cmd(0x003B);wr_data(0x0000);   // P_RTN=0000, P_NW=001  
	wr_cmd(0x003C);wr_data(0x00F0);   // I_RTN=1111, I_NW=000   
	wr_cmd(0x003D);wr_data(0x0000);   // DIV=00  
	SysTickDelay(20);  
	wr_cmd(0x0035);wr_data(0x0038);   // EQS=38h          
	wr_cmd(0x0036);wr_data(0x0078);   // EQP=78h  
	 
	wr_cmd(0x003E);wr_data(0x0038);   // SON=38h  
         
	wr_cmd(0x0040);wr_data(0x000F);   // GDON=0Fh  
	wr_cmd(0x0041);wr_data(0x00F0);   // GDOFF   
	
	// **********Power Supply Setting ******************* 
	
	wr_cmd(0x0019);wr_data(0x0049);                // OSCADJ=10 0000,OSD_EN=1 //60Hz   
	wr_cmd(0x0093);wr_data(0x000C);                // RADJ=1100,  
	SysTickDelay(10);  
	
	wr_cmd(0x0020);wr_data(0x0040);                // BT=0100        
	wr_cmd(0x001D);wr_data(0x0007);                // VC1=111  
	wr_cmd(0x001E);wr_data(0x0000);                // VC3=000  
	wr_cmd(0x001F);wr_data(0x0007);                                //  VRH=0100 4.12V        
	wr_cmd(0x0044);wr_data(0x0030);          // VCM=101 0000   3.21V  
	wr_cmd(0x0045);wr_data(0x0009);          // VDV=1 0001  -1.19V  
	SysTickDelay(10);  
	 
	wr_cmd(0x001C);wr_data(0x0004);          // AP=100  
	SysTickDelay(20);  
	wr_cmd(0x001B);wr_data(0x0018);          // GASENB=0, PON=1, DK=1, XDK=0, DDVDH_TRI=0, STB=0   
	SysTickDelay(40);  
	wr_cmd(0x001B);wr_data(0x0010);          // GASENB=0, PON=1, DK=1, XDK=0, DDVDH_TRI=0, STB=0   
	SysTickDelay(40);  
	wr_cmd(0x0043);wr_data(0x0080);          //set VCOMG=1  
	SysTickDelay(10);  
 
//************** Display ON Setting ******************** 
         
	wr_cmd(0x0090);wr_data(0x007f); 
	wr_cmd(0x0026);wr_data(0x0004);   //GON=0, DTE=0, D=01   
	SysTickDelay(40);  
	wr_cmd(0x0026);wr_data(0x0024);   //GON=1, DTE=0, D=01   
	wr_cmd(0x0026);wr_data(0x002C);   //GON=1, DTE=0, D=11  
	SysTickDelay(40);        
	wr_cmd(0x0026);wr_data(0x003C);   //GON=1, DTE=1, D=11                 
//**************set spulse & rpulse*******************   
	wr_cmd(0x0057);wr_data(0x0002);   // Test mode='1'  
	wr_cmd(0x0056);wr_data(0x0084);   // set Rpulse='1000',spulse='0100'  
	wr_cmd(0x0057);wr_data(0x0000);   // Test mode= '0'  
#elif defined ST7781
	//--------------------------------ST7781 Internal Register Initial---------------------------------------//
	wr_cmd(0x00ff);wr_data(0x0001);
	wr_cmd(0x00f3);wr_data(0x0008);
//    wr_cmd(0x00fe);wr_data(0x0008);
	//LCD_CtrlRead_ST7781 (0x00f3); //Read Parameter
	//---------------------------End ST7781 Internal Register Initial--------------------------------------//
	//------------------------------------Display Control Setting----------------------------------------------//
	wr_cmd(0x0001);wr_data(0x0100); //Output Direct
	wr_cmd(0x0002);wr_data(0x0700); //Line Inversion
	wr_cmd(0x0003);wr_data(0x1030); //Entry Mode (65K, BGR)
	wr_cmd(0x0008);wr_data(0x0807); // Porch Setting
	wr_cmd(0x0009);wr_data(0x0000); //Scan Cycle
	wr_cmd(0x000A);wr_data(0x0000); //FMARK off
	//-----------------------------------End Display Control setting-----------------------------------------//
	//-------------------------------- Power Control Registers Initial --------------------------------------//
	wr_cmd(0x0010);wr_data(0x0790); //Power Control1
	wr_cmd(0x0011);wr_data(0x0005); //Power Control2
	wr_cmd(0x0012);wr_data(0x0000); //Power Control3
	wr_cmd(0x0013);wr_data(0x0000); //Power Control4
	//---------------------------------End Power Control Registers Initial -------------------------------//
	SysTickDelay(100); //Delay 100ms
	//--------------------------------- Power Supply Startup 1 Setting------------------------------------//
	wr_cmd(0x0010);wr_data(0x1490); //Power Control1
	SysTickDelay(50); //Delay 50ms
	wr_cmd(0x0011);wr_data(0x0227); //Power Control2
	//--------------------------------- End Power Supply Startup 1 Setting------------------------------//
	SysTickDelay(50); //Delay 50ms
	//--------------------------------- Power Supply Startup 2 Setting------------------------------------//
	wr_cmd(0x0012);wr_data(0x0089); //Power Control3
	wr_cmd(0x0013);wr_data(0x1900); //Power Control4
	wr_cmd(0x0029);wr_data(0x0021); //VCOMH setting 0021
//    wr_cmd(0x002B);wr_data(0x000f);
	//--------------------------------- End Power Supply Startup 2 Setting------------------------------//
	SysTickDelay(50); //Delay 50ms
	//-------------------------------------Gamma Cluster Setting-------------------------------------------//
	wr_cmd(0x0030);wr_data(0x0000);
	wr_cmd(0x0031);wr_data(0x0006);
	wr_cmd(0x0032);wr_data(0x0100);
	wr_cmd(0x0035);wr_data(0x0001);
	wr_cmd(0x0036);wr_data(0x0000);
	wr_cmd(0x0037);wr_data(0x0000);
	wr_cmd(0x0038);wr_data(0x0406);
	wr_cmd(0x0039);wr_data(0x0202);
	wr_cmd(0x003c);wr_data(0x0001);
	wr_cmd(0x003d);wr_data(0x0000);
	//---------------------------------------End Gamma Setting---------------------------------------------//
	//----------------------------------Display Windows 240 X 320----------------------------------------//
	wr_cmd(0x0050);wr_data(0x0000); // Horizontal Address Start Position
	wr_cmd(0x0051);wr_data(0x00ef); // Horizontal Address End Position
	wr_cmd(0x0052);wr_data(0x0000); // Vertical Address Start Position
	wr_cmd(0x0053);wr_data(0x013f); // Vertical Address End Position
	//----------------------------------End Display Windows 240 X 320----------------------------------//
	//-------------------------------------------Frame Rate Setting-------------------------------------------//
	wr_cmd(0x0060);wr_data(0xa700); //Gate scan control
	wr_cmd(0x0061);wr_data(0x0001); //Non-display Area setting
	wr_cmd(0x0090);wr_data(0x0033); //RTNI setting
	//-------------------------------------------END Frame Rate setting------------------------------------//
	wr_cmd(0x0007);wr_data(0x0133); //Display Control1
	SysTickDelay(50); //Delay 50ms
	
	wr_cmd(0x0022);	
#elif defined ILI9328
	//LCD_CtrlRead_ST7781 (0x00f3); //Read Parameter
	//---------------------------End 9328 Internal Register Initial--------------------------------------//
	//------------------------------------Display Control Setting----------------------------------------------//
	wr_cmd(0x0001);wr_data(0x0100); //Output Direct
	wr_cmd(0x0002);wr_data(0x0700); //Line Inversion
	wr_cmd(0x0003);wr_data(0x1030); //Entry Mode (65K, BGR)
	wr_cmd(0x0008);wr_data(0x0807); // Porch Setting
	wr_cmd(0x0009);wr_data(0x0000); //Scan Cycle
	wr_cmd(0x000A);wr_data(0x0000); //FMARK off
	wr_cmd(0x000C);wr_data(0x0000); // RGB interface setting
	wr_cmd(0x000D);wr_data(0x0000); // Frame marker Position
	wr_cmd(0x000F);wr_data(0x0000); // RGB interface polarity
	//-----------------------------------End Display Control setting-----------------------------------------//
	//-------------------------------- Power Control Registers Initial --------------------------------------//
	wr_cmd(0x0010);wr_data(0x1000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	wr_cmd(0x0011);wr_data(0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	wr_cmd(0x0012);wr_data(0x0000); // VREG1OUT voltage
	wr_cmd(0x0013);wr_data(0x0000); // VDV[4:0] for VCOM amplitude
	//---------------------------------End Power Control Registers Initial -------------------------------//
	SysTickDelay(100); //Delay 100ms
	//--------------------------------- Power Supply Startup 1 Setting------------------------------------//
	wr_cmd(0x0010);wr_data(0x1490); //Power Control1
	SysTickDelay(50); //Delay 50ms
	wr_cmd(0x0011);wr_data(0x0227); //Power Control2
	//--------------------------------- End Power Supply Startup 1 Setting------------------------------//
	SysTickDelay(50); //Delay 50ms
	//--------------------------------- Power Supply Startup 2 Setting------------------------------------//
	wr_cmd(0x0012);wr_data(0x001a); // External reference voltage= Vci;//1a
	SysTickDelay(50); // Delay 50ms
	wr_cmd(0x0013);wr_data(0x0c00); // VDV[4:0] for VCOM amplitude//0x1400
	wr_cmd(0x0029);wr_data(0x0010); // VCM[5:0] for VCOMH//0019
	wr_cmd(0x002B);wr_data(0x000b); // Set Frame Rate//0c
	SysTickDelay(50); // Delay 50ms
	wr_cmd(0x0020);wr_data(0x0000); // GRAM horizontal Address
	wr_cmd(0x0021);wr_data(0x0000); // GRAM Vertical Address
	
	// ----------- Adjust the Gamma Curve ----------//
	wr_cmd(0x0030);wr_data(0x0000);
	wr_cmd(0x0031);wr_data(0x0505);//0607
	wr_cmd(0x0032);wr_data(0x0007);//0305
	wr_cmd(0x0035);wr_data(0x0502);//0000
	wr_cmd(0x0036);wr_data(0x0807);//1604
	wr_cmd(0x0037);wr_data(0x0007);//0204
	wr_cmd(0x0038);wr_data(0x0202);//0001
	wr_cmd(0x0039);wr_data(0x0707);
	wr_cmd(0x003C);wr_data(0x0205);//0000
	wr_cmd(0x003D);wr_data(0x0608);//000f
	
	//------------------ Set GRAM area ---------------//
	wr_cmd(0x0050);wr_data(0x0000); // Horizontal Address Start Position
	wr_cmd(0x0051);wr_data(0x00ef); // Horizontal Address End Position
	wr_cmd(0x0052);wr_data(0x0000); // Vertical Address Start Position
	wr_cmd(0x0053);wr_data(0x013f); // Vertical Address End Position
	wr_cmd(0x0060);wr_data(0xa700); // Gate Scan Line a700
	wr_cmd(0x0061);wr_data(0x0001); // NDL,VLE, REV
	wr_cmd(0x006A);wr_data(0x0000); // set scrolling line

	//-------------- Partial Display Control ---------//
	wr_cmd(0x0080);wr_data(0x0000);
	wr_cmd(0x0081);wr_data(0x0000);
	wr_cmd(0x0082);wr_data(0x0000);
	wr_cmd(0x0083);wr_data(0x0000);
	wr_cmd(0x0084);wr_data(0x0000);
	wr_cmd(0x0085);wr_data(0x0000);
	
	//-------------- Panel Control -------------------//
	wr_cmd(0x0090);wr_data(0x0010);
	wr_cmd(0x0092);wr_data(0x0600);
	wr_cmd(0x0093);wr_data(0x0003);
	wr_cmd(0x0095);wr_data(0x0110);
	wr_cmd(0x0097);wr_data(0x0000);
	wr_cmd(0x0098);wr_data(0x0000);
	wr_cmd(0x0007);wr_data(0x0133); // 262K color and display ON			
#endif 

}




