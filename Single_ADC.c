
//Single channel ADC continous conversion

#include <stdint.h>
#include <stm32f4xx.h>

uint16_t val = 0;
/*
 * ADC Steps-
 * 1.Enable ADC peripheral
 * 2.Enable GPIOx
 * 3.Set GPIO_Moder as Analog
 * 4.Set Conversion Length SQR1(L_Bit)
 * 5.Set Channel to Convert (SQRx)
 * 6.Set Continous mode CR2
 * 7.Set (ADC On bit) in CR2
 * 8.Set (SwStartbit) in CR2 to start conversion
 *
 *
 * While Reading Data-
 *
 * Check EOC bit in Status Register
 * When Conversion Ends Read ADC1->DR register
 *
 *
 * */

int main(void) {

	//ADC enable
	RCC->APB2ENR |= (1 << 8);

	//C enable
	RCC->AHB1ENR |= (1 << 2);

	//analog mode
	GPIOC->MODER |= (1 << 4) | (1 << 5);

	//1 conversion
	ADC1->SQR1 = 0;

	//input is 12th channel
	ADC1->SQR3 = (0xC << 0);

	ADC1->CR2 |= (1 << 0);	//adon bit
	ADC1->CR2 |= (1 << 1);	//cont mode
	ADC1->CR2 |= (1 << 30);	//swstart

	while (1) {
		while (!((ADC1->SR) & (1 << 1))) {
		};
		val = ADC1->DR;
	}
}
