#ifndef FONT_DATA_H
#define FONT_DATA_H

#define FONT_HZ_TYPE_CODE		0		/*�����ֿ��ڴ�����*/
#define FONT_HZ_TYPE_FLASH		1		/*�����ֿ�����ΧFLASH��*/

//�����ֿ��λ��
#define FONT_HZ_TYPE			FONT_HZ_TYPE_FLASH/*FONT_HZ_TYPE_CODE*/



typedef struct
{
	u8 Hz_Index[3];		//����
	u8 Code[32];		//�ֿ�
}HZ_CODE;

extern const u8 ASCII816[96][16];
extern const u8 ASCII1224[96][48];
extern const u8 ASCII2432[96][96];
extern const u8 ASCII3245[96][180];
extern const u8 ASCII1625[96][50];
#if FONT_HZ_TYPE==FONT_HZ_TYPE_CODE		//�����ֿ�

extern const HZ_CODE Hz_Code[];

#elif FONT_HZ_TYPE==FONT_HZ_TYPE_FLASH	//FLASH�ֿ�

extern u8 Gob_Font_Buf[72];
//�ֿ���FLASH�е���ʼλ��
//#define HZ_DOT16_START_ADDR		0
//#define HZ_DOT24_START_ADDR		0x34e00

#endif



#endif
