/**************************************************************
**ģ��:mem.c
**����:��̬�ڴ濪�ٺ���,��Ҫ�ⲿ���������������,һ���ǳ����ڴ�Ķ���,һ���ǿ����ڴ�Ķ��� 
	��̬���ٵ��ڴ�ؼ����ڳ����ڴ�Ķ��˺Ϳ����ڴ�Ķ���֮�� 
**����:���Ӱײ�
**�汾:V1.1(09.11.5) ���ֽڶ����ϸ����������,�Ľ��˷���.
**�汾��Ϣ:	V1.0���� �����ù���44B0X,LPC2XXXϵ����,����ֲ��STM32.�����������˱�׼���÷�(���˻���ϲ���Ǳ��÷�),
			��ʱֻ֧��MDK������.
***************************************************************/

#include "stm32f10x.h"
#include "mem.h"


/*ʹ�������ڴ涥�˵Ĳ�����Ϊ������(�Ǳ�׼�÷�,���ռ�Ŀ��ǱȽ�ʡ��)*/
extern u8 __initial_sp[];
 void *HEAP_BASE=__initial_sp;	//����STM32�ٷ���������,��ջ�Ķ��˾��ǳ����ڴ�ĩ��
#define HEAP_LIMIT (0x20000000+0x5000)		//�������������ڴ澡ͷ,V8ϵ�е�STM32��20K��RAM,VC����48K.����

/*ʹ���û���HEAP����Ϊ������(��׼�÷�,HEAP������ν�Ķ�,Ҫ�ڻ�������ļ��϶���HEAP�Ĵ�С*/
/*
extern u8 __heap_base[],u8 __heap_limit[];
void *HEAP_BASE=__heap_base;
void *HEAP_LIMIT=__heap_limit;

*/

static u8 *Mem_Ptr[MAX_MEM_PTR+1];		//��Դָ��--����һ������DELETE��ʱ���ƶ�����
static u32 Mem_Len[MAX_MEM_PTR+1];		//�ڴ�鳤�ȸ���Ԫ�ض�ӦMem_Ptr,���Ⱥ�ָ�볤�����
static u32 Mem_Err_No=MEM_ERR_NONE;		//������Ϣ

/*static*/ u32 Mem_No=0;			//Ϊ�˷���ָ�����,�����ڴ��ĸ���

/**************************
**������:BCNew
**����:����Ҫ���С�����ڴ棬����ָ��(ע�����Ϊ�ֽ���)
**ע������:���ʧ�ܣ��򷵻�NULL
**************************/
void *BCNew(u32 nbyte)
{
	u32 i,j;
	u8 * new_ptr=NULL,*now_ptr;
	
	if(!nbyte)
	{
		Mem_Err_No=MEM_ERR_BYTE_ZERO;
		return(NULL);
	}
	
	//1.Ѱ�ҿ����ڴ�
	
	for(i=0;i<MAX_MEM_PTR-1;i++)	//ʹ��MAX_MEM_PTR-1��ȷ�������ܲ���
	{
		if(Mem_Len[i]!=0)		//��ǰ��0
		{
			if(Mem_Len[i+1]!=NULL)	//��һ����0
			{
				if(Mem_Ptr[i]+Mem_Len[i]<=Mem_Ptr[i+1] )	//�߼��ϱ��������������,
				{
					if( (u32)(Mem_Ptr[i+1]-Mem_Ptr[i]-Mem_Len[i])>=(nbyte+4))	//Ϊ��ȷ��4�ֽڶ���,����ȷ���ĳ��ȶ�4�ֽ�(������������)
					{
						now_ptr=Mem_Ptr[i]+Mem_Len[i];			//�µ�ַ
					/*	
						if((((u32)now_ptr)%4)==0)	//�µ�ַ�Ѿ�����
						{
							new_ptr=now_ptr;
						}
						else
						{
							new_ptr=(u8*)((u32)now_ptr+(4- ((u32)now_ptr)%4))   ;	//���ֽڶ���
						}
					*/
						//�����Ƽ����뷽��:
						new_ptr=(u8*)(((u32)now_ptr+3)&0xfffffffc);	
						break;
					}
					else
					{
						continue;
					}
					
				}
				else	//��������MEMϵͳ������
				{
					Mem_Err_No=MEM_ERR_PTR_DOWN;					//MEM�����Ŵ�
					while(1);//debug
				//	return(NULL);
				}
			}
			else//��һ��Ϊ0,����Է���ռ�
			{
				if(((u32)(Mem_Ptr[i]+Mem_Len[i]+nbyte+4))<HEAP_LIMIT)	//������,��ɷ���*ע��4�ֽڶ���
				{
					now_ptr=Mem_Ptr[i]+Mem_Len[i];			//�µ�ַ 
					/* *_*
					if((((u32)now_ptr)%4)==0)	//�µ�ַ�Ѿ�����
					{
						new_ptr=now_ptr;
					}
					else
					{
						new_ptr=(u8*)((u32)now_ptr+(4- ((u32)now_ptr)%4))   ;	//���ֽڶ���
					}
					*/
					//�����Ƽ����뷽��:
					new_ptr=(u8*)(((u32)now_ptr+3)&0xfffffffc);	
					break;
				}//������
				else
				{
					Mem_Err_No=MEM_ERR_OVER;						//�ռ䲻��
					return(NULL);
				}
			}
			
		}
		else//��ǰ��Ϊ0,���뵽�����ȻMem_No==0,i==0
		{
			if((i)||(Mem_No))	//debug-��̫���ܳ���
			{
				while(1);
			}
			if(((u32)((u32)HEAP_BASE+nbyte+4))<HEAP_LIMIT)	//������,��ɷ���*ע��4�ֽڶ���
			{
				now_ptr=(u8*)HEAP_BASE;			//�µ�ַ
				/*
				if((((u32)now_ptr)%4)==0)	//�µ�ַ�Ѿ�����
				{
					new_ptr=now_ptr;
				}
				else
				{
					new_ptr=(u8*)((u32)now_ptr+(4- ((u32)now_ptr)%4))   ;	//���ֽڶ���
				}
				*/
				//�����Ƽ����뷽��:
				new_ptr=(u8*)(((u32)now_ptr+3)&0xfffffffc);	
				break;
			}//������
			else
			{
				Mem_Err_No=MEM_ERR_OVER;						//�ռ䲻��
				return(NULL);
			}
		} 
	
	
	}//end for
	
	if(new_ptr==NULL)	//�ҵ���ͷ��û���䵽��ַ,���������������
	{
		Mem_Err_No=MEM_ERR_BLOCK_OVER;						//��������
		return(NULL);
	}
	
	//��������Mem_Ptr Mem_Len
	if(Mem_No)
	{
		
		for(j=Mem_No-1;j>i;j--)
		{
			Mem_Ptr[j+1]=Mem_Ptr[j];
			Mem_Len[j+1]=Mem_Len[j];
		}
		
		Mem_Ptr[i+1]=new_ptr;	//�������ڴ��
		Mem_Len[i+1]=nbyte;
	}
	else
	{
		Mem_Ptr[0]=new_ptr;
		Mem_Len[0]=nbyte;
	}
	Mem_No++;
	return(new_ptr);
}



/**************************
**������:BCDelete
**����:�ͷ���Դ,�ͷ�ʵ�����ǲ�ͣѯ���Ƿ��д���Դ������ͷ�ʧ�ܣ��򷵻�FALSE
**ע������:�ͷ�ʵ��Ҳ����������MEM_PTR
**************************/
u8 BCDelete(void * pt)
{
	u32 i;
	for(i=0;i<Mem_No;i++)
	{
		if(Mem_Ptr[i]==(u8*)pt)
		{
			for(;i<Mem_No-1;i++)
			{
				Mem_Ptr[i]=Mem_Ptr[i+1];
				Mem_Len[i]=Mem_Len[i+1];
			}
			Mem_Ptr[Mem_No-1]=0;	//���һ���ճ�
			Mem_Len[Mem_No-1]=0;
			Mem_No--;
			return(TRUE);
		}
	}
	return(FALSE);
}

/***************************
**������:BCMemInit
**����:��ʼ��MEM�����й���
**ע������:
***************************/
void BCMemInit(void) 
{
	u32 i;
	u32 *pt;
	
	for(i=0;i<MAX_MEM_PTR;i++)
	{
		Mem_Ptr[i]=NULL;		//�ڴ��ָ��ΪNULL
		Mem_Len[i]=0;			//�ڴ�鳤��Ϊ0
	}
	Mem_No=0;
	
	pt=(u32*)BCNew(0x04);	//��ʱ�ײ�����ȿ���һ���ڴ���� 
	*pt=0x01020304;				//д���Զ����־�������
	
}

/***************************
**������:BCGetMemErr
**����:��ʼ��MEM�����й���
**ע������:
***************************/
u32 BCGetMemErr(void)
{
	u32 tmp;
	tmp=Mem_Err_No;
	Mem_Err_No=MEM_ERR_NONE;
	return(tmp);
}

