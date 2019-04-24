#ifndef _USART1_H_
#define _USART1_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "types.h"

#define USART1_REC_LEN  		200  							// ���崮�ڽ��ջ����С
#define EN_USART1_RX 			1									// ʹ�ܴ���1���գ�0Ϊ��ֹ
extern volatile unsigned char rxd_buffer_locked1;
extern volatile unsigned int usart_buf1[26];  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; 		// �������ڽ��ջ��� 
extern u16 USART1_RX_STA;         						// �������ڽ���״̬��־λ
void USART1_Config(void);
void USART1_Putc(unsigned char c);
void USART1_Puts(char * str);
void USART1_IRQHandler(void);
void USART1_NVIC_Configuration(void);
#endif 

