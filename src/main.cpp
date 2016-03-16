#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stdio.h>
#include "retarget.h"
#include "compatibility.h"
#include "RF24.h"
#include "adc.h"


#define LED_GPIO (GPIO_TypeDef *) GPIOC
#define LED_PIN GPIO_Pin_13

/** GPIO PIN PA2 PA3 **/
#define CE_NRF 2
#define CSN_NRF 3

/** Host nRF24 address **/
static const uint64_t extAddress = 0xF0F0F0F0E2LL;

/** Test message **/
static char message[] = "F0F0F0F0D3:TestNRF!\n";

/**
 * Function setups RF24 radio. Additionally prints out registers.
 */
void setupRF(RF24 &radio) {
    radio.begin();
    radio.setChannel(0x4C);
    radio.setPayloadSize(32);
    radio.setPALevel(RF24_PA_LOW);
    radio.openWritingPipe(extAddress);
    radio.printDetails();
}

void LED_init() {
	GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}

int main() {
    LED_init();
    retarget_init();
	adc_init();
    RF24 rf24(CE_NRF, CSN_NRF);
    setupRF(rf24);
    printf("Setup complete...\n\r");
	char wet;
	u16 test;
    while(1367) {
        if(GPIO_ReadInputDataBit(LED_GPIO, LED_PIN)) {
            GPIO_ResetBits(LED_GPIO, LED_PIN);
        } else {
            GPIO_SetBits(LED_GPIO, LED_PIN);
        }
        __msleep(500);
		
		//test = readADC1(ADC_Channel_9);
		wet = is_wet();	    
		printf("Value is %c \r\n", wet);    
		//rf24.write(&wet, sizeof(char));
    }
}
