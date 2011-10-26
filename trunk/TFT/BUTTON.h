#ifndef BUTTON_H
#define BUTTON_H

/***********************
**�ṹ����:BC_BUTTON
**˵��:�����ؼ�
***********************/
typedef struct
{
	DX_WIDGET widget;			//��������
	u8* p_self_property;		//��������*
	char* str;					//�ַ�
//	void (*button_on)(u8);		//��Ӧ����
	//����ΪBMP��������Ҫ���õ�����
	COLOR* bmp_normal;			//ƽʱ��ͼ��
	COLOR* bmp_focus;			//����ʱ��ͼ��
	COLOR* bmp_push;			//���º��ͼ��
}const DX_BUTTON;

//BUTTON��������-ռ��8-9-10λ
#define BASE_PRO_BUTTON_TYPE_MASK	0x700
#define BASE_PRO_BUTTON_TYPE_STRING	(0<<8)		//Ĭ��
#define BASE_PRO_BUTTON_TYPE_BMP	(1<<8)


//BUTTON���-ռ��16,17λ
#define BASE_PRO_BUTTON_STYLE_MASK	0x30000		//16,17λ���ڶ�����,��4�ַ��
#define BASE_PRO_BUTTON_STYLE_3D	0			//��ͨ3D���(Ĭ��)
#define BASE_PRO_BUTTON_STYLE_2D	(1<<16)		//2Dƽ����(����+����)
#define BASE_PRO_BUTTON_STYLE_NULL	(2<<16)		//��ɫ���(��ɫΪWINDOW������ɫ)

//BUTTON��������


#define SELF_PRO_PUSH_DOWN			0x01		//SELF���Ե�һλ�����µ�״̬,1Ϊ����,0Ϊ����


/**************************************************************
	�ؼ���ɫԤ����
***************************************************************/

//������ɫ
#define COLOR_BUTTON_STRING			BLACK	//�ַ�ɫ
#define COLOR_BUTTON_BG_UNFOCUS		GRAY1	//ûѡ�еı���ɫ--Ĭ�ϵı���ɫ


extern MESSAGE_WIDGET KeyToChangeButton(DX_BUTTON*,GUIKEY);
extern void ReDrawButton(DX_BUTTON* the_button);
extern void GuiButtonDraw(DX_BUTTON *the_button);

#endif
