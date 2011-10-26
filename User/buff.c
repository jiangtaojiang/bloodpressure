/******************************************************************************
* file Name     : buff.c                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    :                                                                            
* Updata        : chen   
* brief         : 
*******************************************************************************/
/* 注意： 该buffer处理函数必须将数据区大小设置为2^n
*/

#include "buff.h"


CBUFF xtcp_buf;
BYTE  xtcp_data[XTCP_DATA_SIZE];  //xtcp的data区大小（必须为2^n）

CBUFF  com_buf;
BYTE   com_data[COM_DATA_SIZE];	  //com的data区大小（必须为2^n）

void buff_cm_init(void)
{	CBUFF *bp;

	bp = &com_buf;
	bp->out = bp->in = bp->trial = 0;
	bp->data = com_data;
	bp->len = COM_DATA_SIZE;	
}

void buff_xtcp_init(void)
{	CBUFF *bp;

	bp = &xtcp_buf;
	bp->out = bp->in = bp->trial = 0;
	bp->data = xtcp_data;
	bp->len = XTCP_DATA_SIZE;	
}

/* Set all the buffer pointers to a starting value */
void buff_init(void)
{
	buff_cm_init();
	buff_xtcp_init();
}

/* 使用该函数，不使用宏定义的原因：避免系统崩溃
   中断时对BUFF指针进行操作
	 宏定义是直接提取变量
	 函数是保存引用值的副本 */
WORD minw(WORD a, WORD b)
{	return (a<b ? a : b);
}

/* Return total length of data in buffer */
WORD buff_dlen(CBUFF *bp)
{	return ((WORD)((bp->in -bp->out) & (bp->len-1)));
}

/* Return length of untried (i.e. unsent) data in buffer */
WORD buff_untriedlen(CBUFF *bp)
{	return((WORD)((bp->in - bp->trial) & (bp->len-1)));
}

/* Return length of trial data in buffer (i.e. data send but unacked) */
WORD buff_trylen(CBUFF *bp)
{	return ((WORD)((bp->trial - bp->out) & (bp->len -1)));
}

/* Return length of free space in buffer */
WORD buff_freelen(CBUFF *bp)
{	return (bp->len ? bp->len-1 - buff_dlen(bp) : 0);
}


/* Load data into buffer, return num of bytes that could be accepted
** If data pointer is null. adjust pointers but don't transfer data */
/* 若数据过大就截去多余的数据,这样做的原因是假定程序员在调用该函数前已检查过可用空间 */
WORD buff_in(CBUFF *bp, BYTE *data, WORD len)
{	WORD in, n, n1, n2;
	in = (WORD)bp->in & (bp->len-1);		 /* Mask len to buffer area */
	n  = minw(len, buff_freelen(bp));		 /* Get max allowable length */
	n1 = minw(n,(WORD)(bp->len - in));	 /* Length up to end of buff */
	n2 = n - n1;												 /* Length from start of buff */
	if(n1 && data)											 /* If anything to copy ... */
		memcpy (&bp->data[in], data, n1);	 /* ..copy up to end of buffer ..*/
	if(n2 && data)											 /* and maybe also.. */
		memcpy (&bp->data, &data[n1], n2); /* ..copy into start of buffer */
	bp->in += n;												 /* Bump point */
	return (n);
}

/* Load string into buffer, return num of chars that could be accepted */
WORD buff_instr(CBUFF *bp, char *str)
{	return(buff_in(bp, (BYTE *)str, (WORD)strlen(str)));
}

/* Remove waiting data from buffer, return number of bytes 
** If data pointer is null, adjust pointers but don't transfer data */
WORD buff_try(CBUFF *bp, BYTE *data, WORD maxlen)
{	WORD trial, n, n1, n2;
	trial = (WORD)bp->trial & (bp->len-1);			 /* Mask trial len to buffer area */
	n  = minw(maxlen, buff_untriedlen(bp));			 /* Get max allowable length */
	n1 = minw(n,(WORD)(bp->len -trial));				 /* Length up to end of buff */
	n2 = n - n1;																 /* Length from start of buff */
	if(n1 && data)															 /* If anything to copy.. */
		memcpy(data, &bp->data[trial], n1);				 /* ..copy up to end of buffer.. */
	if(n2 && data)															 /* ..and maybe also.. */
		memcpy(&data[n1], bp->data, n2);					 /* ..copy from start of buffer */
	bp->trial += n;															 /* Bump trial pointer */
	return(n);
}

/* Remove data from buffer, return number of bytes
** If data pointer is null, adjust pointers but don't transfer data */
WORD buff_out(CBUFF *bp, BYTE *data, WORD maxlen)
{
//	unsigned int cpu_sr;
	
	WORD out, n, n1, n2;
	out = (WORD)bp->out & (bp->len-1);			/* Mask len to buffer area */
	n  = minw(maxlen, buff_dlen(bp));				/* Get max allowable length */
	n1 = minw(n, (WORD)(bp->len - out));		/* Length up to end of buff */
	n2 = n - n1;														/* Length from start of buff */
	if(n1 && data)													/* If anything to copy.. */
		memcpy(data, &bp->data[out], n1);			/* ..copy up to end of buffer.. */
	if(n2 && data)													/* ..and maybe also.. */
		memcpy(&data[n1], bp->data, n2);			/* ..copy from start of buffer */
	bp->out +=n;														/* Bump O/P pointer */
	if(buff_untriedlen(bp))									/* ..and maybe trial pointer */
		bp->trial = bp->out;
	return(n);
}

/* Rewind the trial pointer by the given byte count, return actual count */
WORD buff_retry(CBUFF *bp, WORD len)
{	len = minw(len, buff_trylen(bp));
	bp->trial -= len;
	return(len);
}
