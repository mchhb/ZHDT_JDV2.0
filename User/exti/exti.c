/********************************************************
 * �ļ���  ��exti.c
*********************************************************/
#include "exti.h"
/*  ���� PA0 PA15 Ϊ���жϿڣ��������ж����ȼ� */
void EXTI_PA0_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE); //ʹ�ܶ˿ں͸���IOʱ�� 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);// �������ȼ�����
  
  /* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel 										= EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;	//��ռ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 6;  //�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd 								= ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_0;       
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IPD;	 					// ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //ѡ��PA�ڵ�0�ſ�Ϊ�ж���·
  EXTI_InitStructure.EXTI_Line 		= EXTI_Line0; 							//�ⲿ�ж���1
  EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt; 			//����Ϊ�ж�����
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 		//�½����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

void EXTI_PA1_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE); //ʹ�ܶ˿ں͸���IOʱ�� 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);// �������ȼ�����
  
  /* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel 										= EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;		//��ռ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 7;  	//�����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd 								= ENABLE;
  NVIC_Init(&NVIC_InitStructure);
			
  GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_1;       
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IPD;	 						// ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);	//�ж���0����GPIOA.15
  EXTI_InitStructure.EXTI_Line    = EXTI_Line1;								//���ð������е��ⲿ��·
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  		//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);															// ��ʼ���ⲿ�ж�
}
void EXTI_ENABLE(void)
{
		EXTI_PA0_Config();
		EXTI_PA1_Config();
}
