/**************************************************************
	��ɫ����
***************************************************************/

#ifndef COLOR_H
#define COLOR_H

//#pragma diag_suppress 1             //ȥ����1�ű�����ʾ
//#pragma diag_suppress 47            //ȥ����47�ű�����ʾ

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

//ȡ��SAMPOO��ɫ��

#define GRAY_0    0x18c3   //��10%
#define GRAY_1    0x31a6   //��20%
#define GRAY_2    0x528a   //��30%
#define GRAY_3    0x6b6d   //��40%
#define GRAY_4    0x8c51   //��50%
#define GRAY_5    0xa534   //��60%
#define GRAY_6    0xbdd7   //��70%
#define GRAY_7    0xce79   //��80%
#define GRAY_8    0xe71c   //��90%
#define GRAY_9    0xf79e   //��100%

//----------------------------------------------
#define RED_0   0x1800   //��ɫ10%
#define RED_1   0x3000   //��ɫ20%
#define RED_2   0x4800   //��ɫ30%
#define RED_3   0x6000   //��ɫ40%
#define RED_4   0x7800   //��ɫ50%
#define RED_5   0x9000   //��ɫ60%
#define RED_6   0xa800   //��ɫ70%
#define RED_7   0xc800   //��ɫ80%
#define RED_8   0xe000   //��ɫ90%
#define RED_9   0xf800   //��ɫ100%
//-------------------------------------------
#define GREEN_0 0x00c0   //��ɫ10%
#define GREEN_1 0x0180   //��ɫ20%
#define GREEN_2 0x0240   //��ɫ30%
#define GREEN_3 0x0320   //��ɫ40%
#define GREEN_4 0x03e0   //��ɫ50%
#define GREEN_5 0x04a0   //��ɫ60%
#define GREEN_6 0x0560   //��ɫ70%
#define GREEN_7 0x0640   //��ɫ80%
#define GREEN_8 0x0700   //��ɫ90%
#define GREEN_9 0x07e0   //��ɫ100%
//-----------------------------------------
#define BLUE_0  0x0003   //��ɫ10%
#define BLUE_1  0x0006   //��ɫ20%
#define BLUE_2  0x0009   //��ɫ30%
#define BLUE_3  0x000c   //��ɫ40%
#define BLUE_4  0x000f   //��ɫ50%
#define BLUE_5  0x0012   //��ɫ60%
#define BLUE_6  0x0016   //��ɫ70%
#define BLUE_7  0x0019   //��ɫ80%
#define BLUE_8  0x001c   //��ɫ90%
#define BLUE_9  0x001f   //��ɫ100%
//-------------------------------------------
#define Green_kb  0x05ed    //kaba
//-------------------------------------------
#define REDBLUE_1 0x284e  //��ɫ1
#define REDBLUE_2 0x3892  //��ɫ2
#define REDBLUE_3 0x4915  //��ɫ3
#define REDBLUE_4 0x5157  //��ɫ4
#define REDBLUE_5 0x6199  //��ɫ5
#define REDBLUE_6 0x725b  //��ɫ6
#define REDBLUE_7 0x8b1d  //��ɫ7
#define REDBLUE_8 0x935d  //��ɫ8
#define REDBLUE_9 0xac3e  //��ɫ9
//-------------------------------------------
#define PING_0    0x2804  //�Ϻ�ɫ10%
#define PING_1    0x4006  //�Ϻ�ɫ20%
#define PING_2    0x5808  //�Ϻ�ɫ30%
#define PING_3    0x700a  //�Ϻ�ɫ40%
#define PING_4    0x880c  //�Ϻ�ɫ50%
#define PING_5    0xa02f  //�Ϻ�ɫ60%
#define PING_6    0xb811  //�Ϻ�ɫ70%
#define PING_7    0xd033  //�Ϻ�ɫ80%
#define PING_8    0xe855  //�Ϻ�ɫ90%
#define PING_9    0xf837  //�Ϻ�ɫ100%
//-------------------------------------------
#define YELLOW_1  0x41e0  //��ɫ1
#define YELLOW_2  0x6ae0  //��ɫ2
#define YELLOW_3  0x8be1  //��ɫ3
#define YELLOW_4  0xa4a1  //��ɫ4
#define YELLOW_5  0xbd42  //��ɫ5
#define YELLOW_6  0xcdc4  //��ɫ6
#define YELLOW_7  0xde47  //��ɫ7
#define YELLOW_8  0xeeea  //��ɫ8
#define YELLOW_9  0xf752  //��ɫ9
//-----------------------------------------ST���е��
#define BLUE_ST_0 0x00a2   //����ɫ10%
#define BLUE_ST_1 0x0145   //����ɫ20%
#define BLUE_ST_2 0x0229   //����ɫ30%
#define BLUE_ST_3 0x02cc   //����ɫ40%
#define BLUE_ST_4 0x03b0   //����ɫ50%
#define BLUE_ST_5 0x0473   //����ɫ60%
#define BLUE_ST_6 0x0536   //����ɫ70%
#define BLUE_ST_7 0x05d9   //����ɫ80%
#define BLUE_ST_8 0x06bc   //����ɫ90%
#define BLUE_ST_9 0x075f   //����ɫ100%
//-----------------------------------------
#define BLUE_AT_0 0x00e3   //����ɫ10%
#define BLUE_AT_1 0x01a6   //����ɫ20%
#define BLUE_AT_2 0x0249   //����ɫ30%
#define BLUE_AT_3 0x02eb   //����ɫ40%
#define BLUE_AT_4 0x03ae   //����ɫ50%
#define BLUE_AT_5 0x0491   //����ɫ60%
#define BLUE_AT_6 0x0534   //����ɫ70%
#define BLUE_AT_7 0x05d7   //����ɫ80%
#define BLUE_AT_8 0x073c   //����ɫ90%
#define BLUE_AT_9 0x07be   //����ɫ100%
//-----------------------------------------
#define BLUE_BT_0 0x00c5   //����ɫ10%
#define BLUE_BT_1 0x0147   //����ɫ20%
#define BLUE_BT_2 0x01aa   //����ɫ30%
#define BLUE_BT_3 0x022d   //����ɫ40%
#define BLUE_BT_4 0x0290   //����ɫ50%
#define BLUE_BT_5 0x02f2   //����ɫ60%
#define BLUE_BT_6 0x0375   //����ɫ70%
#define BLUE_BT_7 0x03f8   //����ɫ80%
#define BLUE_BT_8 0x047b   //����ɫ90%
#define BLUE_BT_9 0x04fe   //����ɫ100%
//-----------------------------------------
#define YELLOWGREEN_0  0x2180   //����ɫ10%
#define YELLOWGREEN_1  0x2a20   //����ɫ20%
#define YELLOWGREEN_2  0x3aa0   //����ɫ30%
#define YELLOWGREEN_3  0x4340   //����ɫ40%
#define YELLOWGREEN_4  0x53e0   //����ɫ50%
#define YELLOWGREEN_5  0x5c60   //����ɫ60%
#define YELLOWGREEN_6  0x6d00   //����ɫ70%
#define YELLOWGREEN_7  0x7dc0   //����ɫ80%
#define YELLOWGREEN_8  0x8e80   //����ɫ90%
#define YELLOWGREEN_9  0xa780   //����ɫ100%



/****************************************************************
GUIϵͳ�����ȫ����ɫ-�û��ɰ��Լ������޸�,��������޸�
*****************************************************************/

#define COLOR_FOCUS					RED		/*����ɫ*/
#define COLOR_FLASH					GREEN	/*��˸ɫ*/
#define COLOR_DISABLE				GRAY1	/*��Чɫ*/
#define COLOR_FRAME_2D				BLACK	/*2Dͼ���Ŀ�ɫ*/
#define COLOR_SYS_FILL_RECT			WHITE	/*ϵͳ�Ŀؼ����ɫ,WINDOW�ķ��,���ǰ�ɫ,Ӧ����EDIT,CHECK�ȿ���*/

//A�Ϳ����ɫ
#define COLOR_FRAME_A_OU			GRAY2	//����
#define COLOR_FRAME_A_OD			WHITE	//����
#define COLOR_FRAME_A_IU			BLACK	//����
#define COLOR_FRAME_A_ID			GRAY1	//����

//B�Ϳ����ɫ
#define COLOR_FRAME_B				BLACK	//��߿�
#define COLOR_FRAME_B_U				WHITE	//�п���
#define COLOR_FRAME_B_D				BLACK	//�п���(��ʱ�ú�ɫ)
#define COLOR_FRAME_B_I				GRAY2	//�ڿ���

//��Ч�ַ���ɫ
#define COLOR_STRING_DISABLE1		WHITE	//��Ч�������ַ�ɫ1
#define COLOR_STRING_DISABLE2		GRAY2	//��Ч�������ַ�ɫ2


#endif
