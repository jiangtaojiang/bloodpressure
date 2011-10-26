#ifndef COMBO_H
#define COMBO_H

/*************************
**�ṹ����:DX_COMBO
**˵��:combo�ؼ� 
**		��ʾ��ʽΪ�ַ���,��U8����ֵ,����ֻ����0-255����Ŀ,*_*��Ŀ��Ϊconst
** 
*************************/
typedef struct
{
	DX_WIDGET widget;			//��������
	u8* p_self_property;		//��������--����û��Ҫ
	u8* p_tmp;					//��ʱֵ*
	u8* p_data;					//ʵ��ֵ*
	void*const* obj;			//��Ŀ����(��Ŀ��Ϊconst)
	u8  max_data;				//��Ŀ��
	COLOR color2d;				//2Dɫ

}const DX_COMBO;


//COMBO�������� ռ�û�������8,9,10λ(8��״̬)
#define BASE_PRO_COMBO_TYPE_MASK 			0x700
#define BASE_PRO_COMBO_TYPE_STRING			(1<<8)		//�ַ�����
#define BASE_PRO_COMBO_TYPE_BMP				(2<<8)		//BMP��

//COMBO�ķ��-ռ��11,12λ
#define BASE_PRO_COMBO_STYLE_MASK	0x1800		//11,12λ���ڶ�����,��4�ַ��
#define BASE_PRO_COMBO_STYLE_3D		0			//��ͨ3D���(Ĭ��)
#define BASE_PRO_COMBO_STYLE_2D		(1<<11)		//2Dƽ����(����+����)
#define BASE_PRO_COMBO_STYLE_NULL	(2<<11)		//�������



//COMBO��ɫ
//#define COLOR_COMBO_STRING			BLACK	//����ɫ
//#define COLOR_COMBO_BG_UNFOCUS_3D	WHITE	//ûѡ�еı���ɫ 3D
//#define COLOR_COMBO_BG_UNFOCUS_2D	GREEN_5	//ûѡ�еı���ɫ 2D
//#define COLOR_COMBO_BG_FOCUS		RED		//���㱳��ɫ
//#define COLOR_COMBO_BG_FLASH		RED		//��˸����ɫ

extern void ReDrawCombo(DX_COMBO* the_combo);
extern void GuiComboDraw(DX_COMBO *the_combo);
extern void SetComboTmp(DX_COMBO* the_combo,u8 tmp);
extern u8 GetComboTmp(DX_COMBO* the_combo);
extern void UpdateCombo(DX_COMBO* the_combo,bool dir);
extern MESSAGE_WIDGET KeyToChangeCombo(DX_COMBO* the_combo,GUIKEY key);
#endif
