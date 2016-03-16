#include "adc.h"
#include "compatibility.h"
#include <stdio.h>

u16 readADC1(u8 channel)
{
  ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_1Cycles5);
  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // Wait until conversion completion
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  // Get the conversion value
  return (int)((float)ADC_GetConversionValue(ADC1)/4095*330);
}



void adc_init(){
	//Initialization of ADC1
	//////////////////////////////////////////////////////
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Initialization of charge and discharge pin
	//////////////////////////////////////////////////////
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		//charge pin
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	ADC_InitStructure.ADC_NbrOfChannel = 1;
  	ADC_Init(ADC1, &ADC_InitStructure);
	
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5);

  	/* Enable ADC1 DMA */
  	ADC_DMACmd(ADC1, ENABLE);
  
  	/* Enable ADC1 */
  	ADC_Cmd(ADC1, ENABLE);

  	/* Enable ADC1 reset calibration register */   
  	ADC_ResetCalibration(ADC1);
  	/* Check the end of ADC1 reset calibration register */
  	while(ADC_GetResetCalibrationStatus(ADC1));

  	/* Start ADC1 calibration */
  	ADC_StartCalibration(ADC1);
  	/* Check the end of ADC1 calibration */
  	while(ADC_GetCalibrationStatus(ADC1));
     
  	/* Start ADC1 Software Conversion */ 
  	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//////////////////////////////////////////////////////
}

char is_wet(){
//	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//	GPIO_SetBits(GPIOA, GPIO_Pin_7);
//	USART_puts(USART1, "Helou vrld !!!\n\r");
//	USART_puts(USART1, dec(1234, &time[29]));
//	USART_puts(USART1, "\n\r");

	uint16_t average = 0;
	uint16_t ADC_values[50];
	int i;
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);	//Charge pin ON
	for( i = 0; i < 10; i++){
		ADC_values[i] = readADC1(ADC_Channel_9);
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	__msleep(100);
		
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	for( i = 10; i < 20; i++){
		ADC_values[i] = readADC1(ADC_Channel_9);
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	__msleep(100);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	for( i = 20; i < 30; i++){
		ADC_values[i] = readADC1(ADC_Channel_9);
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	__msleep(100);
		
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	for( i = 30; i < 40; i++){
		ADC_values[i] = readADC1(ADC_Channel_9);
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	__msleep(100);
		
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	for( i = 40; i < 50; i++){
		ADC_values[i] = readADC1(ADC_Channel_9);
	}
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	__msleep(100);
		
	for( i = 0 ; i < 50 ; i++){
		average = average + ADC_values[i];
	}
		
	average = average / 50;
	printf("Average: %d\r\n", average);
	if(average < 100)return '1';
	else return '0';
}

