/**************************************************************
**模块:mem.c
**功能:动态内存开辟函数,需要外部引入的数据有两个,一个是程序内存的顶端,一个是可用内存的顶端 
	动态开辟的内存控件就在程序内存的顶端和可用内存的顶端之间 
**作者:电子白菜
**版本:V1.1(09.11.5) 四字节对齐上根据网友意见,改进了方法.
**版本信息:	V1.0初版 曾经用过在44B0X,LPC2XXX系列上,现移植到STM32.开辟区加上了标准的用法(个人还是喜欢非标用法),
			暂时只支持MDK编译器.
***************************************************************/

#include "stm32f10x.h"
#include "mem.h"


/*使用物理内存顶端的部分作为开辟区(非标准用法,但空间的考虑比较省心)*/
extern u8 __initial_sp[];
 void *HEAP_BASE=__initial_sp;	//根据STM32官方启动代码,堆栈的顶端就是程序内存末端
#define HEAP_LIMIT (0x20000000+0x5000)		//定义允许分配的内存尽头,V8系列的STM32有20K的RAM,VC则有48K.这里

/*使用用户的HEAP区作为开辟区(标准用法,HEAP就是所谓的堆,要在汇编启动文件上定义HEAP的大小*/
/*
extern u8 __heap_base[],u8 __heap_limit[];
void *HEAP_BASE=__heap_base;
void *HEAP_LIMIT=__heap_limit;

*/

static u8 *Mem_Ptr[MAX_MEM_PTR+1];		//资源指针--留多一个用于DELETE的时候移动数据
static u32 Mem_Len[MAX_MEM_PTR+1];		//内存块长度各个元素对应Mem_Ptr,长度和指针长度相符
static u32 Mem_Err_No=MEM_ERR_NONE;		//错误信息

/*static*/ u32 Mem_No=0;			//为了方便指针插入,定义内存块的个数

/**************************
**函数名:BCNew
**功能:根据要求大小分配内存，返回指针(注意参数为字节数)
**注意事项:如果失败，则返回NULL
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
	
	//1.寻找空闲内存
	
	for(i=0;i<MAX_MEM_PTR-1;i++)	//使用MAX_MEM_PTR-1，确保至少能插入
	{
		if(Mem_Len[i]!=0)		//当前非0
		{
			if(Mem_Len[i+1]!=NULL)	//下一个非0
			{
				if(Mem_Ptr[i]+Mem_Len[i]<=Mem_Ptr[i+1] )	//逻辑上必须会符合这个条件,
				{
					if( (u32)(Mem_Ptr[i+1]-Mem_Ptr[i]-Mem_Len[i])>=(nbyte+4))	//为了确保4字节对齐,必须比分配的长度多4字节(这里懒的运算)
					{
						now_ptr=Mem_Ptr[i]+Mem_Len[i];			//新地址
					/*	
						if((((u32)now_ptr)%4)==0)	//新地址已经对齐
						{
							new_ptr=now_ptr;
						}
						else
						{
							new_ptr=(u8*)((u32)now_ptr+(4- ((u32)now_ptr)%4))   ;	//四字节对齐
						}
					*/
						//网友推荐对齐方法:
						new_ptr=(u8*)(((u32)now_ptr+3)&0xfffffffc);	
						break;
					}
					else
					{
						continue;
					}
					
				}
				else	//否则整个MEM系统将崩溃
				{
					Mem_Err_No=MEM_ERR_PTR_DOWN;					//MEM队列排错
					while(1);//debug
				//	return(NULL);
				}
			}
			else//下一个为0,则可以分配空间
			{
				if(((u32)(Mem_Ptr[i]+Mem_Len[i]+nbyte+4))<HEAP_LIMIT)	//不超顶,则可分配*注意4字节对齐
				{
					now_ptr=Mem_Ptr[i]+Mem_Len[i];			//新地址 
					/* *_*
					if((((u32)now_ptr)%4)==0)	//新地址已经对齐
					{
						new_ptr=now_ptr;
					}
					else
					{
						new_ptr=(u8*)((u32)now_ptr+(4- ((u32)now_ptr)%4))   ;	//四字节对齐
					}
					*/
					//网友推荐对齐方法:
					new_ptr=(u8*)(((u32)now_ptr+3)&0xfffffffc);	
					break;
				}//不超顶
				else
				{
					Mem_Err_No=MEM_ERR_OVER;						//空间不够
					return(NULL);
				}
			}
			
		}
		else//当前块为0,进入到这里必然Mem_No==0,i==0
		{
			if((i)||(Mem_No))	//debug-不太可能成立
			{
				while(1);
			}
			if(((u32)((u32)HEAP_BASE+nbyte+4))<HEAP_LIMIT)	//不超顶,则可分配*注意4字节对齐
			{
				now_ptr=(u8*)HEAP_BASE;			//新地址
				/*
				if((((u32)now_ptr)%4)==0)	//新地址已经对齐
				{
					new_ptr=now_ptr;
				}
				else
				{
					new_ptr=(u8*)((u32)now_ptr+(4- ((u32)now_ptr)%4))   ;	//四字节对齐
				}
				*/
				//网友推荐对齐方法:
				new_ptr=(u8*)(((u32)now_ptr+3)&0xfffffffc);	
				break;
			}//不超顶
			else
			{
				Mem_Err_No=MEM_ERR_OVER;						//空间不够
				return(NULL);
			}
		} 
	
	
	}//end for
	
	if(new_ptr==NULL)	//找到尽头都没分配到地址,则代表最大块数不够
	{
		Mem_Err_No=MEM_ERR_BLOCK_OVER;						//块数不够
		return(NULL);
	}
	
	//重新排列Mem_Ptr Mem_Len
	if(Mem_No)
	{
		
		for(j=Mem_No-1;j>i;j--)
		{
			Mem_Ptr[j+1]=Mem_Ptr[j];
			Mem_Len[j+1]=Mem_Len[j];
		}
		
		Mem_Ptr[i+1]=new_ptr;	//插入新内存块
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
**函数名:BCDelete
**功能:释放资源,释放实际上是不停询问是否有此资源，如果释放失败，则返回FALSE
**注意事项:释放实际也是重新排列MEM_PTR
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
			Mem_Ptr[Mem_No-1]=0;	//最后一个空出
			Mem_Len[Mem_No-1]=0;
			Mem_No--;
			return(TRUE);
		}
	}
	return(FALSE);
}

/***************************
**函数名:BCMemInit
**功能:初始化MEM的所有功能
**注意事项:
***************************/
void BCMemInit(void) 
{
	u32 i;
	u32 *pt;
	
	for(i=0;i<MAX_MEM_PTR;i++)
	{
		Mem_Ptr[i]=NULL;		//内存块指针为NULL
		Mem_Len[i]=0;			//内存块长度为0
	}
	Mem_No=0;
	
	pt=(u32*)BCNew(0x04);	//暂时底层必须先开辟一个内存块垫底 
	*pt=0x01020304;				//写入自定义标志方便调试
	
}

/***************************
**函数名:BCGetMemErr
**功能:初始化MEM的所有功能
**注意事项:
***************************/
u32 BCGetMemErr(void)
{
	u32 tmp;
	tmp=Mem_Err_No;
	Mem_Err_No=MEM_ERR_NONE;
	return(tmp);
}

