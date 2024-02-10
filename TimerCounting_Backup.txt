#include <stdint.h>
#include <stm32f4xx.h>

uint32_t i=0;

int main(void){

	RCC->APB1ENR |= (1<<1);

	TIM3->PSC = 50000;
	TIM3->ARR = 2000;

	TIM3->CR1 |= (2<<5);
	TIM3->CR1 |= (1<<4); //dir
	TIM3->CR1 |= (1<<0); //cen

	while (1) {

		i=TIM3->CNT;

	}
}
