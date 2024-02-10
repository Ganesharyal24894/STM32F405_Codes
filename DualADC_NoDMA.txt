/*
 * 		DualADC_NoDMA.c
 *
 *  	Created on: Aug 3, 2023
 *      Author: Ganesh Sharma
 *
 *      This Program is for Reading Values from both POT1(ch12) and POT2(ch11)
 *      without enabling DMA to do this.
 *
 */

#include <stdint.h>
#include <stm32f4xx.h>

uint16_t val1 = 0, val2 = 0;

void ADC_12(void);
void ADC_11(void);
void ADC_Init(void);

int main(void) {

	ADC_Init();

	while (1) {
		//for POT 1
		ADC_12();
		//EOC bit Check
		while (!((ADC1->SR) & (1 << 1))) {
		}
		val1 = ADC1->DR;

		//for POT 2
		ADC_11();
		//EOC bit Check
		while (!((ADC1->SR) & (1 << 1))) {
		}
		val2 = ADC1->DR;
	}
}

///////////////FUNCTION DEFINITIONS/////////////////////

void ADC_12(void) {
	//ADON bit OFF
	ADC1->CR2 &= ~(1 << 0);
	//Set Channel to (0x0C) for POT 1
	ADC1->SQR3 = (0x0C << 0);
	//Set (ADC On bit)
	ADC1->CR2 |= (1 << 0);
	//Set (SwStartbit) to start conversion
	ADC1->CR2 |= (1 << 30);
}

void ADC_11(void) {
	//ADON bit OFF
	ADC1->CR2 &= ~(1 << 0);
	//Set Channel to (0x0B) for POT 2
	ADC1->SQR3 = (0x0B << 0);
	//Set (ADC On bit)
	ADC1->CR2 |= (1 << 0);
	//Set (SwStartbit) to start conversion
	ADC1->CR2 |= (1 << 30);
}

void ADC_Init(void) {
	RCC->APB2ENR |= (1 << 8);
	RCC->AHB1ENR |= (1 << 2);
	//for both POT1 & POT2
	GPIOC->MODER |= (1 << 2) | (1 << 3);
	GPIOC->MODER |= (1 << 4) | (1 << 5);
	//conversion length = 1
	ADC1->SQR1 = 0;
	//initial channel Input
	ADC1->SQR3 = (0x0C << 0);
	//continous conversion
	ADC1->CR2 |= (1 << 1);
	//(ADC On bit)
	ADC1->CR2 |= (1 << 0);
	//(SwStartbit)
	ADC1->CR2 |= (1 << 30);
}
