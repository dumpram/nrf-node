#include <stm32f10x_adc.h>
#include <stm32f10x_gpio.h>

void adc_init();
char is_wet();
u16 readADC1(uint8_t ch);
