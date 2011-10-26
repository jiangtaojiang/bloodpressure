#ifndef __BUFF_H__
#define __BUFF_H__


#include <string.h>

#define XTCP_DATA_SIZE 512
#define COM_DATA_SIZE  512


typedef unsigned short WORD;
typedef unsigned long  LWORD;
typedef unsigned char  BYTE;

/* Circular buffer structure */
typedef struct
{	WORD len;		/* length of data  (must be first) */
	LWORD in;		/* Incoming data */
	LWORD out;		/* Outgoing data */
	LWORD trial;	/* Outgoing data 'on trial' */
	BYTE *data;		/* buffer */
}CBUFF;



void buff_init(void);
void buff_cm_init(void);
void buff_xtcp_init(void);

WORD buff_dlen(CBUFF *bp);
WORD buff_freelen(CBUFF *bp);
WORD buff_trylen(CBUFF *bp);
WORD buff_untriedlen(CBUFF *bp);

WORD buff_in(CBUFF *bp, BYTE *data, WORD len);
WORD buff_out(CBUFF *bp, BYTE *data, WORD maxlen);
WORD buff_try(CBUFF *bp, BYTE *data, WORD maxlen);

WORD buff_instr(CBUFF *bp, char *str);

WORD buff_retry(CBUFF *bp, WORD len);

extern CBUFF xtcp_buf;
extern CBUFF  com_buf;


//#define debug(x) printf(x)

#endif

