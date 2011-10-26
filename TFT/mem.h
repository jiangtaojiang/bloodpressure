#ifndef MEM_H
#define MEM_H

#define MAX_MEM_PTR		10		//���������ڴ�������


//�������
#define MEM_ERR_NONE			0	//�޴�
#define MEM_ERR_BYTE_ZERO		1	//���ٵ��ֽ���Ϊ0
#define MEM_ERR_PTR_DOWN		2	//MEM������������
#define MEM_ERR_OVER			3	//�ڴ�ռ䲻��
#define MEM_ERR_BLOCK_OVER		4	//Ԥ������ڴ��������



extern void BCMemInit(void);
extern void *BCNew(u32 nbyte);
extern u8 BCDelete(void *pt);

#ifndef NULL
#define NULL 0
#endif

#endif
