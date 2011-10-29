#ifndef __BT_H__
#define __BT_H__

#define BT_WAIT_LINK_TIMEOUT 5	//等待连接超时 单位:s

#define BT_PWR_PIN	GPIO_Pin_1	//pa1 低有效
#define BT_LINK_PIN	GPIO_Pin_6	//pa6 高有效
#define BT_AT_PIN	GPIO_Pin_5	//pa5
#define BT_RST_PIN	GPIO_Pin_4	//pa4
			
#define BT_PWR_ON()   		GPIO_ResetBits(GPIOA, BT_PWR_PIN)
#define BT_PWR_OFF()  		GPIO_SetBits(GPIOA, BT_PWR_PIN)
#define BT_LINK_STATE()		GPIO_ReadInputDataBit(GPIOA, BT_LINK_PIN)
#define BT_ENTER_ATMODE() 	GPIO_SetBits(GPIOA,BT_AT_PIN)
#define BT_EXIT_ATMODE() 	GPIO_ResetBits(GPIOA,BT_AT_PIN)

#define BT_ENTER_RST() 		GPIO_ResetBits(GPIOA,BT_RST_PIN)
#define BT_EXIT_RST() 		GPIO_SetBits(GPIOA,BT_RST_PIN)

#ifdef bt_c

void BT_Init(void);
char bt_wait_connect(unsigned long t);
void at(char *ptr, int timeout);
void bt_addr(void);
void bt_link(void);
void bt_disc(void);
unsigned char link_test(void);

#else
extern void BT_Init(void);
extern char bt_wait_connect(unsigned long t);
extern void at(char *ptr, int timeout);
extern void bt_addr(void);
extern void bt_link(void);
extern void bt_disc(void);
extern unsigned char link_test(void);
#endif
#endif
