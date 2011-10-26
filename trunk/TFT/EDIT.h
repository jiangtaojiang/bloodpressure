#ifndef _EDIT_H
#define _EDIT_H

/*************************
**�ṹ����:DX_EDIT
**˵��:�༭��ؼ�
*************************/
typedef struct
{
	DX_WIDGET widget;			//��������
	u8* p_self_property;		//��������
	s32* p_tmp;					//��ʱֵ*
	s32* p_data;				//ʵ��ֵ*
	s32 max_data;				//���ֵ
	s32 min_data;				//��Сֵ

}const DX_EDIT;

/**************************************************************
EDIT BASE������
***************************************************************/
//EDIT�Ļ�������-ռ�û������Բ��� 8,9,10λ(8��״̬)
#define BASE_PRO_EDIT_TYPE_MASK		0x700		//8,9,10λ���ڶ���EDIT������,��8������
#define BASE_PRO_EDIT_TYPE_0P		0			//0����С�������ͨEDIT(Ĭ��)
#define BASE_PRO_EDIT_TYPE_1P		(1<<8)		//1����1λС��EDIT
#define BASE_PRO_EDIT_TYPE_2P		(2<<8)		//2����2λС��EDIT
#define BASE_PRO_EDIT_TYPE_PW		(3<<8)		//3����PASSWORD��ʽ��EDIT
#define BASE_PRO_EDIT_TYPE_O8		(4<<8)		//4����8��������ģʽ


//EDIT�ķ��-ռ��16,17λ
#define BASE_PRO_EDIT_STYLE_MASK	0x30000		//16,17λ���ڶ�����,��4�ַ��
#define BASE_PRO_EDIT_STYLE_3D		0			//��ͨ3D���(Ĭ��)
#define BASE_PRO_EDIT_STYLE_2D		(1<<16)		//2Dƽ����(����+����)
#define BASE_PRO_EDIT_STYLE_NULL	(2<<16)		//�޷��(��ɫΪWINDOW������ɫ) NULL���ͼ�β�֧�ֽ�����˸.
#define BASE_PRO_EDIT_STYLE_GLASS	(3<<16)		//�������(���Ե�ɫ) �������֧�ֽ������˸*_*��ʱ��������,���Ҫ�в������,�����ȽϷ�~


//EDIT��λ��,ռ��24,25,26,27 ������������1-16λ
#define BASE_PRO_EDIT_BCD_MASK		0xf000000
#define BASE_PRO_EDIT_BCD_NULL		(0<<24)			//Ĭ��Ϊ�Զ�����
#define BASE_PRO_EDIT_BCD_1			(1<<24)			//1λ-�ƺ�������,�����뱣��,��Ϊ��ȡλ���ĺ���õ�������λ�����ж�λ��
#define BASE_PRO_EDIT_BCD_2			(2<<24)
#define BASE_PRO_EDIT_BCD_3			(3<<24)
#define BASE_PRO_EDIT_BCD_4			(4<<24)
#define BASE_PRO_EDIT_BCD_5			(5<<24)
#define BASE_PRO_EDIT_BCD_6			(6<<24)
#define BASE_PRO_EDIT_BCD_7			(7<<24)
#define BASE_PRO_EDIT_BCD_8			(8<<24)
/*
#define BASE_PRO_EDIT_BCD_9			(9<<24)
#define BASE_PRO_EDIT_BCD_10		(10<<24)
#define BASE_PRO_EDIT_BCD_11		(11<<24)
#define BASE_PRO_EDIT_BCD_12		(12<<24)
#define BASE_PRO_EDIT_BCD_13		(13<<24)
#define BASE_PRO_EDIT_BCD_14		(14<<24)
#define BASE_PRO_EDIT_BCD_15		(15<<24) 
*/ 
//��ȡλ���ĺ�
#define GET_EDIT_BCD_LEN(the_edit)	((the_edit->widget.base_property&BASE_PRO_EDIT_BCD_MASK)>>24)

//SELF����0,1λ
#define SELF_PRO_EDIT_STATE_MASK	0x03		//1,2λ����װ��EDIT��״̬
//1.��EDITΪ��ͨ����,��״̬λ��ʱ����
//...
//2.��EDITΪ��С��λ
#define SELF_PRO_EDIT_STATE_NULL	0			//0����û����С��,������붨���������Ϊ0
#define SELF_PRO_EDIT_STATE_0P		1			//1�������XXX.��״̬
#define SELF_PRO_EDIT_STATE_1P		2			//2�������XXX.X��״̬
#define SELF_PRO_EDIT_STATE_2P		3			//3�������XXX.XX״̬
//3.��EDITΪPASSWORD,1,2,3,4λ���ڱ�ʾ����λ��
#define SELF_PRO_EDIT_PASS_NUM_MASK	0x0f
#define SELF_PRO_EDIT_PASS_NUM0		0
#define SELF_PRO_EDIT_PASS_NUM1		1
#define SELF_PRO_EDIT_PASS_NUM2		2
#define SELF_PRO_EDIT_PASS_NUM3		3
#define SELF_PRO_EDIT_PASS_NUM4		4
#define SELF_PRO_EDIT_PASS_NUM5		5
#define SELF_PRO_EDIT_PASS_NUM6		6
#define SELF_PRO_EDIT_PASS_NUM7		7
#define SELF_PRO_EDIT_PASS_NUM8		8







extern void ReDrawEdit(DX_EDIT* the_edit);
extern void GuiEditDraw(DX_EDIT* the_edit);
extern MESSAGE_WIDGET KeyToChangeEdit(DX_EDIT*,GUIKEY);
extern void SetEditTmp(DX_EDIT* the_edit,s32 tmp);
extern s32 GetEditTmp(DX_EDIT* the_edit);
extern void UpdateEdit(DX_EDIT* the_edit,bool dir);
extern void EditCursorFlash(bool flash);
extern bool EditPassMatch(DX_EDIT* the_edit,u32 num);
#endif
