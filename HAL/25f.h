/******************************************************************************
* file Name     : 25f.h                                                                          
* Data          : 2011-3-22                                              
* Version       : Ver 1.0.0.0                                                                   
* Programmer    : chen                                                                           
* Updata        : chen   
* brief         : 
*******************************************************************************/
#ifndef _25F_H
#define _25F_H

#ifdef  SST25f_c
void FLASH_SPI_Config(void);

#else
extern void FLASH_SPI_Config(void);

#endif

#endif