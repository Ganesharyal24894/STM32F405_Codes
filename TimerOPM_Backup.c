#include <stdint.h>
#include <stm32f4xx.h>

void opm_func(void);

void TIM3_IRQHandler(void);


uint32_t i=0;
int main(void)
{
	RCC->APB1ENR |= (1<<1);		//timer 3 enable
	RCC->AHB1ENR |= (1<<2);		//GPIOC enable

	GPIOC->MODER |= (1<<12);
	GPIOC->MODER &= ~(1<<13);

	GPIOC->OTYPER &= ~(1<<6);

	GPIOC->OSPEEDR |= (1<<12);
	GPIOC->OSPEEDR |= (1<<13);

	GPIOC->ODR |= (1<<6);

	TIM3->ARR = 5000;
	TIM3->PSC = 50000;

	TIM3->CR1 |= (1<<4);	//DIR Bit

	TIM3->CR1 |= (1<<3);	//OPM Bit

	TIM3->DIER |= (1<<0);	//UIE Bit

	TIM3->EGR |= (1<<0);	//UG Bit
	NVIC_EnableIRQ(TIM3_IRQn);


	opm_func();

	while(1){
		i=TIM3->CNT;
	}
}


void opm_func(void){

	/*
	 * You can take different ARR values also as an argument
	 * and change the length of output signal.
	 *
	 * */
	GPIOC->ODR &= ~(1<<6); //turn ON the LED

	TIM3->CR1 |= (1<<0);	//CEN Bit

}

void TIM3_IRQHandler(void){
	GPIOC->ODR |= (1<<6); //turn OFF the LED

	TIM3->SR &= ~(1<<0);
}
