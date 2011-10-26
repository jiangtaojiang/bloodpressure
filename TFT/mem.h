#ifndef MEM_H
#define MEM_H

#define MAX_MEM_PTR		10		//最多允许的内存分配块数


//错误代码
#define MEM_ERR_NONE			0	//无错
#define MEM_ERR_BYTE_ZERO		1	//开辟的字节数为0
#define MEM_ERR_PTR_DOWN		2	//MEM队列索引错误
#define MEM_ERR_OVER			3	//内存空间不够
#define MEM_ERR_BLOCK_OVER		4	//预定义的内存块数不过



extern void BCMemInit(void);
extern void *BCNew(u32 nbyte);
extern u8 BCDelete(void *pt);

#ifndef NULL
#define NULL 0
#endif

#endif
