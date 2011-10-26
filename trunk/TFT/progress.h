#ifndef PROGRESS_H
#define PROGRESS_H

/*************************
**�ṹ����:DX_PROGRESS
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
	COLOR bg_color;				// ��ɫ
	COLOR fg_color;				// ��ɫ
}const DX_PROGRESS;

/**************************************************************
	PROGRESS BASE������
***************************************************************/
//PROGRESS�Ļ�������-ռ�û������Բ��� 8,9,λ(4��״̬)
#define BASE_PRO_PROGRESS_TYPE_MASK		0x300		//8,9,10λ���ڶ���PROGRESS������,
#define BASE_PRO_PROGRESS_TYPE_PERCENT	0			//0�ǰٷֱ�
#define BASE_PRO_PROGRESS_TYPE_NUM		(1<<8)		//1����ʵ����ֵ

//PROGRESS�ķ�������-ռ�û������Բ��ֵĵ�10λ (����״̬)
#define BASE_PRO_PROGRESS_DIR_MASK		0x400
#define BASE_PRO_PROGRESS_DIR_H			(0<<10)		//��
#define BASE_PRO_PROGRESS_DIR_V			(1<<10)		//��

//PROGRESS����ʼ����-ռ�û������Բ��ֵĵ�11λ(����״̬)
#define BASE_PRO_PROGRESS_BEGIN_MASK	0x800
#define BASE_PRO_PROGRESS_BEGIN_LD		(0<<11)		//������¿�ʼ
#define BASE_PRO_PROGRESS_BEGIN_RU		(1<<11)		//���һ��Ͽ�ʼ

//PROGRESS�ķ��
#define BASE_PRO_PROGRESS_STYLE_MASK	0x30000		//16,17λ���ڶ�����,��4�ַ��
#define BASE_PRO_PROGRESS_STYLE_3D		0			//��ͨ3D���(Ĭ��)
#define BASE_PRO_PROGRESS_STYLE_2D		(1<<16)		//2Dƽ����


/**************************************************************
	��ɫ����
***************************************************************/
//#define COLOR_PROGRESS_STRING	BLACK	//���������ַ���ɫ



/**************************************************************
	�ⲿ���ú���
***************************************************************/
extern void ReDrawProgress(DX_PROGRESS *the_progress);
extern void GuiProgressDraw(DX_PROGRESS *the_progress);
extern void SetProgressTmp(DX_PROGRESS *the_progress,s32 tmp);
extern s32 GetProgressTmp(DX_PROGRESS *the_progress);
extern void UpdateProgress(DX_PROGRESS *the_progress,bool dir);
#endif
