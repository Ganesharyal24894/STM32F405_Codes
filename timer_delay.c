#include <stdint.h>
#include <stm32f4xx.h>

void TIM5_IRQHandler(void);
void create_msdelay(uint16_t delay_t);

volatile int count=0;
int x=0;

int main(void)
{

	RCC->APB1ENR |= (1<<3);	//timer 5 enabled

	TIM5->ARR = 8000;
	TIM5->PSC = 2;

	TIM5->CR1 |= (1<<2);	//update source is underflow/overflow
	TIM5->DIER |= (1<<0);	//update interrupt enabled

	TIM5->EGR |= (1<<0); //re-intialize counter values

	NVIC_EnableIRQ(TIM5_IRQn);

    /* Loop forever */
	while(1){
		create_msdelay(1000);
		x++;

	}

}


void create_msdelay(uint16_t delay_t){
	count=0;
	//turn ON timer
	TIM5->CR1 |= TIM_CR1_CEN;

	while(count<delay_t){}
	//turn OFF timer
	TIM5->CR1 &= ~TIM_CR1_CEN;
}

void TIM5_IRQHandler(void){

	count++;

    TIM5->SR &= ~(1<<0);	//clearing UIF bit

}



