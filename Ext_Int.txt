#include <stdint.h>
#include <stm32f4xx.h>

uint8_t val=0;

int main(void) {

	//	GPIOB enable
	RCC->AHB1ENR |= (1 << 1);
	//	set as input
	GPIOC->MODER &= ~(1 << 14);
	GPIOC->MODER &= ~(1 << 15);

	//syscfg en
	RCC->APB2ENR |= (1<<14);

	//gpioB interrupt source
	SYSCFG->EXTICR[1] |= (1<<12);

	//mask removed
	EXTI->IMR |= (1<<7);
	//falling edge trigger enabled
	EXTI->FTSR |= (1<<7);

	
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	while (1) {

	}
}


void EXTI9_5_IRQHandler(){
	val++;
	//pending register bit cleared
	EXTI->PR |= (1<<7);
}
