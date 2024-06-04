#include <stdint.h>
#include <stm32f4xx.h>


volatile uint32_t count = 0;
volatile uint32_t main_count = 0;
volatile uint32_t count_1 = 0;

/*

in this program Timer 2 and Timer 3 are generating different amount of delay and
increasing count variable at different rate.

*/


int main(void) {


///////////////////////////DELAY/////////////////////////////////

	//enabling TIMER and GPIO port
	RCC->APB1ENR |= (1 << 0);
	RCC->APB1ENR |= (1 << 1);
	RCC->AHB1ENR |= (1 << 2);
	RCC->AHB1ENR |= (1 << 1);

	GPIOC->MODER |= (1 << 12);
	GPIOB->MODER |= (1 << 30);



	//set ARR, PSC		
	TIM2->ARR = 16000000;


	TIM3->PSC = 16000;
	TIM3->ARR = 500;

	//set UG bit in EGR to reinitialize values
	TIM2->EGR |= (1 << 0);
	TIM3->EGR |= (1 << 0);


	TIM2->DIER |= (1 << 0);
	TIM3->DIER |= (1 << 0);

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);

	TIM2->CR1 |= (1 << 0);
	TIM3->CR1 |= (1 << 0);



	while (1) {
		main_count++;
		

	}
}

void TIM2_IRQHandler(void) {

	count++;
	GPIOC->ODR ^= (1 << 6);

	TIM2->SR &= ~(1 << 0); //clear flag
}

void TIM3_IRQHandler(void) {
	count_1++;
	GPIOB->ODR ^= (1 << 15);

	TIM3->SR &= ~(1 << 0); //clear flag
}
