/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��adc.c
 * ����    ��adcӦ�ú�����        
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�----------------------------------------------------
 *          |                                                    |
 *          | PC1 - ADC1 �����ⲿ��ѹ(ͨ��һ��������������ѹ����)|
 *          |                                                    |
 *           ----------------------------------------------------
 * ��汾  ��ST3.5.0 
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "adc.h"

#define DMA_MAX_ADC_CH			1
#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t ADC_ConvertedValue;
__IO uint16_t ADC_Result[DMA_MAX_ADC_CH];
//__IO u16 ADC_ConvertedValueLocal;


/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PA.01
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
	
	/* Configure PA.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
}


/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC��ַ
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//�ڴ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_Result;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = DMA_MAX_ADC_CH;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 //��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	//ADC_InitStructure.ADC_ScanConvMode = ENABLE; 	 //��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //DISABLE;	// ��ֹ����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //��ʹ���ⲿ����ת��
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC2; //ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = DMA_MAX_ADC_CH;	 	//Ҫת����ͨ����Ŀ1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*����ADC1��ͨ��11Ϊ55.	5���������ڣ�����Ϊ1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_28Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_28Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_28Cycles5);
//#define ADC_SampleTime_1Cycles5                    ((uint8_t)0x00)
//#define ADC_SampleTime_7Cycles5                    ((uint8_t)0x01)
//#define ADC_SampleTime_13Cycles5                   ((uint8_t)0x02)
//#define ADC_SampleTime_28Cycles5                   ((uint8_t)0x03)
//#define ADC_SampleTime_41Cycles5                   ((uint8_t)0x04)
//#define ADC_SampleTime_55Cycles5                   ((uint8_t)0x05)
//#define ADC_SampleTime_71Cycles5                   ((uint8_t)0x06)
//#define ADC_SampleTime_239Cycles5                  ((uint8_t)0x07)
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
//	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//ADC_ExternalTrigConvCmd(ADC1, ENABLE);//��ʱ������ADCת��

}

/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}


u16 Get_Adc(u8 ch)   
{
	u16 adr = 0xFFFF;
	
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )) //�ȴ�ת������
	{
		adr = ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����

		ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	}
	return adr;
}



#define ADC_SHIFT_BITS	2
#define ADC_MAX_NUM			((1 << ADC_SHIFT_BITS) + 2)
uint16_t adc_buf[ADC_MAX_NUM];
uint16_t adc_save_idx = 0;
uint16_t adc_filter(uint16_t dat)	{
	uint16_t min, max;
	uint32_t i, sum = 0;

	adc_buf[adc_save_idx] = dat;
	if(++adc_save_idx >= ADC_MAX_NUM)	adc_save_idx = 0;
	
	min = 0xFFFF; max = 0x0000;
	for(i = 0; i < ADC_MAX_NUM; i++)	{
		if(adc_buf[i] > max)	max = adc_buf[i];
		if(adc_buf[i] < min)	min = adc_buf[i];
		sum += adc_buf[i];
	}
	sum -= min; sum -= max;
	
	return (sum >> ADC_SHIFT_BITS);
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

