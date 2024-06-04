//ST32f405rgt6 TIMER PWM - 

#include <stdint.h>
#include <stm32f4xx.h>
uint32_t count = 0;
/*
 * PWM steps--
 *
 * Enable the timerx
 * Enable the GPIOx Port
 * Set GPIO_MODER as Alternate function
 * Set PushPull and Speed
 * Set Alternate function number (ex-AF02[0010])
 * Enable Timer Channel (CCER capture compare enable register )
 * Select PWM Mode CCMR1/CCMR2
 * Set PSC ARR CCR1(duty cycle)
 * Reintialize with Update generation Bit in EGR
 * Enable Counter from CR1 (CEN bit).
 *
 * */



int main(void) {
	//	freq. calculation-> clock/req freq = psc*arr

	RCC->APB1ENR |= (1 << 1);	//tim3 enable

	RCC->AHB1ENR |= (1 << 2);	//GPIOC enable

	GPIOC->MODER |= (1 << 13);	//pc6
	GPIOC->MODER &= ~(1 << 12);	//pc6

	GPIOC->MODER |= (1 << 19);	//pc9
	GPIOC->MODER &= ~(1 << 18);	//pc9

	GPIOC->OTYPER &= ~(1 << 6);	//pc6
	GPIOC->OTYPER &= ~(1 << 9); //pc9

	GPIOC->OSPEEDR |= (3 << 12); //pc6
	GPIOC->OSPEEDR |= (3 << 18); //pc9

	//AF02 for pc6
	GPIOC->AFR[0] &= ~(1 << 24);
	GPIOC->AFR[0] |= (1 << 25);
	GPIOC->AFR[0] &= ~(1 << 26);
	GPIOC->AFR[0] &= ~(1 << 27);

	//AF02 for pc9
	GPIOC->AFR[1] &= ~(1 << 4);
	GPIOC->AFR[1] |= (1 << 5);
	GPIOC->AFR[1] &= ~(1 << 6);
	GPIOC->AFR[1] &= ~(1 << 7);

	TIM3->CCER |= (1 << 0);		//CC1E Channel 1 Enable
	TIM3->CCER |= (1 << 12);	//CC4E Channel 4 Enable

	//TIM3->CCER &= ~(1<<1);	//CC1P	//default acitve state (HIGH/LOW) for channel 1

	TIM3->CCMR1 |= (7 << 4);	//PWM mode 2 active LOW by default

	TIM3->CCMR2 |= (3 << 13);	//PWM mode 1 active HIGH by default
	TIM3->CCMR2 &= ~(1 << 12);	//PWM mode 1 active HIGH by default

	TIM3->PSC = 320;	//psc value
	TIM3->ARR = 200;	//total val

	TIM3->CCR1 = 1;		//duty cycle
	TIM3->CCR4 = 1;		//duty cycle

	TIM3->EGR |= (1 << 0);	//re-intialize the counter
	TIM3->CR1 |= (1 << 0);	//counter enable

	while (1) {
//		count = TIM3->CNT;
		//count = SystemCoreClock;
//		TIM3->CCR1 = 1000;
//		for(int i=0;i<1000000;i++);
//		TIM3->CCR1 = 5000;
//		for(int i=0;i<1000000;i++);
//		TIM3->CCR1 = 9000;
//		for(int i=0;i<1000000;i++);
//		TIM3->CCR1 = 12000;
//		for(int i=0;i<1000000;i++);
		for (int k = 1; k <= 100; k+=2) {
			TIM3->CCR1 = k;
			TIM3->CCR4 = k;
			for (int i = 0; i < 5000; i++) {
			};
		}

		for (int k = 100; k > 0; k-=1) {
			TIM3->CCR1 = k;
			TIM3->CCR4 = k;
			for (int i = 0; i < 5000; i++) {
			};
		}

	}
}
