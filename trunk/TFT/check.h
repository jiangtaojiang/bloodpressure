#ifndef CHECK_H
#define CHECK_H

/*************************
**�ṹ����:DX_EDIT
**˵��:�༭��ؼ� 
**ע������:*_*���ǲ���Ҫp_tmpֵ,��ֵ��self_pro��һ��λ��ʾ,������ʡ��ռ�
**			p_data����, 
*************************/
typedef struct
{
	DX_WIDGET widget;			//��������
	u8* p_self_property;		//��������
	bool * p_tmp;				//��ʱֵ* 
	bool * p_data;				//ʵ��ֵ*
	void*const* obj;			//��Ŀ��,һ����BMP���,STRING�����
	COLOR color2d;				// �ڶ��ŵ�ɫ
}const DX_CHECK;

/**************************************************************
CHECK BASE������
***************************************************************/
//CHECK�Ļ�������-ռ�û������Բ��� 8,9,10λ(8��״̬)
#define BASE_PRO_CHECK_TYPE_MASK	0x700		//8,9,10λ���ڶ���CHECK������,��8������
#define BASE_PRO_CHECK_TYPE_HOOK	0			//0����ͨ�Ĺ���ģʽ,��С�������С����
#define BASE_PRO_CHECK_TYPE_BMP		(1<<8)		//1��BMPģʽ
#define BASE_PRO_CHECK_TYPE_STRING	(2<<8)		//2���ַ�ģʽ
#define BASE_PRO_CHECK_TYPE_BUTTON	(3<<8)		//3�ǰ���ģʽ(���º͵����������״̬)

//CHECK���-ռ��16,17λ*_*�ر�˵��,��ʱCHECK���ֻ����STRINGģʽ��ʱ����Ч(09.11.19)
#define BASE_PRO_CHECK_STYLE_MASK	0x30000		//16,17λ���ڶ�����,��4�ַ��
#define BASE_PRO_CHECK_STYLE_3D		0			//��ͨ3D���
#define BASE_PRO_CHECK_STYLE_2D		(1<<16)		//2Dƽ����(����),Ĭ��
#define BASE_PRO_CHECK_STYLE_NULL	(2<<16)		//��ɫ���(��ɫΪWINDOW������ɫ)


extern void ReDrawCheck(DX_CHECK*);
extern void GuiCheckDraw(DX_CHECK *the_check);
extern void SetCheckTmp(DX_CHECK*,bool);
extern bool GetCheckTmp(DX_CHECK*);
extern void UpdateCheck(DX_CHECK*,bool);
extern MESSAGE_WIDGET KeyToChangeCheck(DX_CHECK* the_check,GUIKEY key);

#endif
