#include "usart.h"

/**
* Function initialises USART1 with given baudrate.
* @param baudrate given baudrate
* */
void init_USART1(int baudrate) {
	/* GPIO structure for initialisation */
	GPIO_InitTypeDef GPIO_InitStruct;
	/* USART structure for initalisation */
	USART_InitTypeDef USART_InitStruct;
	/* NVIC structure for initalisation */
	//NVIC_InitTypeDef NVIC_InitStruct;
	/* For USART1 enable clock on APB2 bus*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
	/* For GPIOA enable clock on APB2 bus  */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Forming GPIO structure */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //pin 6(TX), pin7(RX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	/* Initalization of GPIO */
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Give USART1 control over GPIOA pins 9 and 10*/
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	/* Forming USART initalisation structure */
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	/* Initialisation of USART1 */
	USART_Init(USART1, &USART_InitStruct);

	/* Enabling interrupt for USART1 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Forming NVIC structure */
	//NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	//NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	//NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* NVIC initialisation */
	NVIC_EnableIRQ(USART1_IRQn);
	/* Enabling USART1 */
	USART_Cmd(USART1, ENABLE);
}

void USART_puts(USART_TypeDef* USARTx, char* bytes) {
	while(*bytes) {
		//waits until USARTx is ready
		while(!(USARTx->SR & 0x40));
		USART_SendData(USARTx, *bytes);
		bytes++;
	}
}

extern "C" void USART1_IRQHandler(void) {
  //checks if interrupt is set by USART1
  if(USART_GetITStatus(USART1, USART_IT_RXNE)) {
    char byteReceived = USART1->DR;
    USART_puts(USART1, &byteReceived);
  }
}
