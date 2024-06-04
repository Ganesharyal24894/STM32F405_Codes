#include <stdint.h>
#include <stm32f4xx.h>

void UART_Init(void);

void UART_DMA_RX_Init(void);
void UART_DMA_TX_Init(void);

void UART_Tx(uint8_t ch);
uint8_t UART_Rx(void);


uint8_t val[2] = {0,0};
uint8_t TXval[2] = {'Q','W'};


int main(void) {

	UART_Init();
	UART_DMA_RX_Init();
	UART_DMA_TX_Init();
	
	
	
	while (1) {


	}
}

void UART_Init(void) {
	//uart pins tx = pc10
	//uart pins rx = pc11

	RCC->AHB1ENR |= (1 << 2);		//gpioc enable

	RCC->APB1ENR |= (1 << 18);	//usart3 enable

	//af
	GPIOC->MODER &= ~(1 << 20);
	GPIOC->MODER |= (1 << 21);
	GPIOC->MODER &= ~(1 << 22);
	GPIOC->MODER |= (1 << 23);

	GPIOC->OTYPER &= ~(1 << 10);
	GPIOC->OTYPER &= ~(1 << 11);

	GPIOC->OSPEEDR |= (3 << 20);
	GPIOC->OSPEEDR |= (3 << 22);

	//af7
	GPIOC->AFR[1] |= (7 << 8);
	GPIOC->AFR[1] |= (7 << 12);

	USART3->CR1 &= ~(1 << 13);	//disable UART

	USART3->CR1 |= (1 << 2) | (1 << 3);	//Te Re enable

	USART3->BRR = 0x683;	//Setting baudrate

	USART3->CR3 |= (1 << 6)|(1<<7) ;	//DMA en Rx Tx

	USART3->CR1 |= (1 << 13);	//enable UART
}
///////////////////////////////////////////////////////////////
void UART_Tx(uint8_t ch) {
	while (!((USART3->SR) & (1 << 7))) {
	}	//wait till DR is empty
	USART3->DR = ch;
	while (!((USART3->SR) & (1 << 6))) {
	}	//wait until transmitted
}

uint8_t UART_Rx(void) {
	while (!((USART3->SR) & (1 << 5))) {
	}
	return (USART3->DR);
}

///////////////////////////////////////////////////
void UART_DMA_RX_Init(void) {
	//dma enable off
	DMA1_Stream1->CR &= ~(1 << 0);

	//DMA 1 enable (from request mapping in ref.)
	RCC->AHB1ENR |= (1 << 21);

	//DMA2 stream 0 data direction(PtoM)
	DMA1_Stream1->CR &= ~(3 << 6);

	//Minc bit
	DMA1_Stream1->CR |= (1 << 10);
	//P data size(half word)
//	DMA1_Stream1->CR |= (1 << 11);
	//M data size(half word)
//	DMA1_Stream1->CR |= (1 << 13);
	//circular mode
	DMA1_Stream1->CR |= (1 << 8);
	//channel 4 selection
	DMA1_Stream1->CR |= (4 << 25);

	//total number of values to be transfered
	DMA1_Stream1->NDTR = 2;
	//memory address to transfer
	DMA1_Stream1->M0AR = (uint32_t) &val;

	//peripheral address to transfer
	DMA1_Stream1->PAR = (uint32_t) &USART3->DR;

	//dma en
	DMA1_Stream1->CR |= (1 << 0);

}

void UART_DMA_TX_Init(void){
	//dma enable off
	DMA1_Stream4->CR &= ~(1 << 0);

	//DMA 1 enable (from request mapping in ref.)
	RCC->AHB1ENR |= (1 << 21);

	//DMA1 stream 0 data direction(MtoP)
	DMA1_Stream4->CR |= (1 << 6);

	//Minc bit
	DMA1_Stream4->CR |= (1 << 10);
	//P data size(half word)
//	DMA1_Stream1->CR |= (1 << 11);
	//M data size(half word)
//	DMA1_Stream1->CR |= (1 << 13);
	//circular mode
	DMA1_Stream4->CR |= (1 << 8);
	//channel 7 selection
	DMA1_Stream4->CR |= (7 << 25);

	//total number of values to be transfered
	DMA1_Stream4->NDTR = 2;
	//memory address to transfer
	DMA1_Stream4->M0AR = (uint32_t) &TXval;

	//peripheral address to transfer
	DMA1_Stream4->PAR = (uint32_t) &USART3->DR;
	//dma en
	DMA1_Stream4->CR |= (1 << 0);

}

