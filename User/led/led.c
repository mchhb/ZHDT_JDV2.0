#include "led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
		/*����GPIOB��GPIOF������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
							                                                                       
		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIOF����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;

		/*���ÿ⺯������ʼ��GPIOF7*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
	  

		
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7);	
 
}
/*********************************************END OF FILE**********************/
