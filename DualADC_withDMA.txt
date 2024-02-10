/*
 * 		DualADC_withDMA.c
 *
 *  	Created on: Aug 3, 2023
 *      Author: Ganesh Sharma
 *
 */

#include <stdint.h>
#include <stm32f4xx.h>

uint16_t val[2]={0,0};

void DMA_Init(void);
void ADC_Init(void);

int main(void) {

	ADC_Init();
	DMA_Init();
	ADC1->CR2 |= (1 << 0);
	//(SwStartbit)
		ADC1->CR2 |= (1 << 30);

	while (1) {

	}
}

///////////////FUNCTION DEFINITIONS/////////////////////
void ADC_Init(void) {

	RCC->APB2ENR |= (1 << 8);
	RCC->AHB1ENR |= (1 << 2);

	//for both POT1 & POT2
	GPIOC->MODER |= (1 << 2) | (1 << 3);
	GPIOC->MODER |= (1 << 4) | (1 << 5);

	//conversion length = 2
	ADC1->SQR1 |= (1 << 20);

	//initial channel Input
	ADC1->SQR3 = 0;
	ADC1->SQR3 |= (0x0C << 0);
	ADC1->SQR3 |= (0x0B << 5);

	//EOC bit set after each conversion
	ADC1->CR2 |= (1 << 10);
	//enable DMA
	ADC1->CR2 |= (1 << 8);
	//generate new DMA request as long as new conversions are happening
	ADC1->CR2 |= (1 << 9);
	//SCAN BIT
	ADC1->CR1 |= (1 << 8);


	//continous conversion
	ADC1->CR2 |= (1 << 1);
	//(ADC On bit)
//	ADC1->CR2 |= (1 << 0);
//	//(SwStartbit)
//	ADC1->CR2 |= (1 << 30);
}

void DMA_Init(void) {
	//dma enable off
	DMA2_Stream0->CR &= ~(1 << 0);
	//DMA 2 enable (from request mapping in ref.)
	RCC->AHB1ENR |= (1 << 22);
	//DMA2 stream 0 data direction(PtoM)
	DMA2_Stream0->CR &= ~(3 << 6);
	//Minc bit
	DMA2_Stream0->CR |= (1 << 10);
	//P data size(half word)
	DMA2_Stream0->CR |= (1 << 11);
	//M data size(half word)
	DMA2_Stream0->CR |= (1 << 13);
	//circular mode
	DMA2_Stream0->CR |= (1 << 8);
	//channel 0 selection
	DMA2_Stream0->CR &= ~(7 << 25);

	//total number of values to be transfered
	DMA2_Stream0->NDTR = 2;
	//memory address to transfer
	DMA2_Stream0->M0AR = (uint32_t)&val;

	//peripheral address to transfer
	DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;

	//dma en
	DMA2_Stream0->CR |= (1 << 0);
}
