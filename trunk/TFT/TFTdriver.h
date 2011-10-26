#ifndef TFTDRIVER_H
#define TFTDRIVER_H

//����ɫ
#define RED			0xf800
#define GREEN		0x7e0
#define BLUE		0x001f
#define BLACK		0x0000
#define YELLOW		0xffe0
#define WHITE		0xffff

//����ɫ��
#define CYAN		0x07ff	//��ɫ
#define RED_BRIGHT	0xf810	//����
#define GRAY1		0x8410//0xD699	//ǳ��,����WIN����
#define GRAY2		0x4208//0x8410	//�л�,���ڿ��в�
#define GRAY3		0x00  //0x4208	//���,���ڿ���Χ��ʱֻ���Ǻ�ɫ


//������λ
#define COLOR	u16		//��ɫ����
#define LOC_X	u8		//X����
#define LOC_Y	u16		//Y����


//Ӳ���ߴ�
#define MIN_X	0
#define MIN_Y	0
#define MAX_X	240
#define MAX_Y	320

#define CHECK_LOC(x,y) if(x>MAX_X)return;if(y>MAX_Y)return

#ifdef TFTdriver_c
void SetTFTStyleH(void);
void SetTFTStyleV(void);
void TFTFill(COLOR color);
void TFTHLine(LOC_X x,LOC_Y y,u16 len,COLOR);
void TFTVLine(LOC_X x,LOC_Y y,u16 len,COLOR);
void TFTRect(LOC_X,LOC_Y,LOC_X,LOC_Y,COLOR);
void TFTRectFill(LOC_X,LOC_Y,LOC_X,LOC_Y,COLOR);
void TFTPoint(LOC_X,LOC_Y,COLOR);
void TFTSetXY(LOC_X x,LOC_Y y);
void _TFTPoint(COLOR color);

void TFTShowString16(LOC_X x,LOC_Y y,char* str,COLOR color,u8);
void TFTShowString24(LOC_X x,LOC_Y y,char* str,COLOR color,u8);
void TFTShowString32(LOC_X x,LOC_Y y,char* str,COLOR color,u8 op);
void TFTShowBmp(LOC_X x0,LOC_Y y0,LOC_X x1,LOC_Y y1,COLOR* buf);
void Lcd_Init(void);
#else

extern void SetTFTStyleH(void);
extern void SetTFTStyleV(void);
extern void TFTFill(COLOR color);
extern void TFTHLine(LOC_X x,LOC_Y y,u16 len,COLOR);
extern void TFTVLine(LOC_X x,LOC_Y y,u16 len,COLOR);
extern void TFTRect(LOC_X,LOC_Y,LOC_X,LOC_Y,COLOR);
extern void TFTRectFill(LOC_X,LOC_Y,LOC_X,LOC_Y,COLOR);
extern void TFTPoint(LOC_X,LOC_Y,COLOR);
extern void TFTSetXY(LOC_X x,LOC_Y y);
extern void _TFTPoint(COLOR color);

extern void TFTShowString16(LOC_X x,LOC_Y y,char* str,COLOR color,u8);
extern void TFTShowString24(LOC_X x,LOC_Y y,char* str,COLOR color,u8);
extern void TFTShowString32(LOC_X x,LOC_Y y,char* str,COLOR color,u8 op);
extern void TFTShowBmp(LOC_X x0,LOC_Y y0,LOC_X x1,LOC_Y y1,COLOR* buf);
//extern void TFTShowCursor(LOC_X x,LOC_Y y,u8 len,bool flash);
extern COLOR BG_COLOR;	
extern void Lcd_Init(void);

#endif
#endif
