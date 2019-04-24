#ifndef _USART3_H_
#define _USART3_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "types.h"

#define USART3_REC_LEN  		200  							// ���崮�ڽ��ջ����С
#define EN_USART3_RX 			1									// ʹ�ܴ���3���գ�0Ϊ��ֹ
extern volatile unsigned char rxd_buffer_locked3;
extern volatile unsigned int usart_buf3[26];
extern volatile unsigned char usart_flg3;
extern volatile unsigned char ptr_rxd_buffer3;
extern unsigned char  start3;
extern unsigned char  crc3;  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; 		// �������ڽ��ջ��� 
extern u16 USART3_RX_STA;         						// �������ڽ���״̬��־λ

void USART3_Config(void);
//int fputc(int ch, FILE *f);
//void USART_NVIC_Configuration(void);
void USART3_Putc(unsigned char c);
void USART3_Puts(char * str);
void USART3_IRQHandler(void);
void USART3_NVIC_Configuration(void);
#endif 

