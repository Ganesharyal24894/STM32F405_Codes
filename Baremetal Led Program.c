/*
 * This is full Baremetal approach which means not using any header files
 * and writing address of peripherals manually
 *
 * Baremetal Led Program
 *
 * */

//this library is only for using standard data types like uint32_t,uint8_t etc.
#include <stdint.h>

//for PC6 LED1

/*
 *	Steps for turning ON an output --
 *
 * ->First enable GPIO port to which your LED is Connected
 *  	(GPIOC in our case since LED is connected to PC6 which means PortC_6th pin)
 *
 * ->To enable this port go to AHB1ENR register since GPIOC port is connected to AHB bus
 *  	(AHB1ENR stands for AHB1 Enable Register all the peripherals connected to this bus will be enabled from this register)
 *
 * ->Next set this pin to be an Output mode by default it will be in reset state which is Input mode
 *	(we can do this by setting MODER register inside GPOIC section)
 *
 * ->Now to simply turn ON the LED we can use ODR register which is Output Data register
 *	(since our LEDs are Active Low we need to write '0' to turn them ON)
 *
 * */

/*
 *
 * Functionality of different registers present in GPIO section-
 *
 * MODER -> used define input/output/af mode/ analog mode
 * OTYPER ->push-pull/open drain
 * OSPEEDR ->used set max freq. at which output can work
 * PUPDR -> used to enable pullup/pull down resistors
 * IDR -> to read input value
 * ODR -> to set output data
 * BSSR -> bit addressable output register
 * LCKR -> used to lock configuration of GPIO
 * AFRL/AFRH-> used to select the Alternate function number
 *
 *
 * */

#define GPIOC_BASE 0x40020800
#define GPIOB_BASE 0x40020400
#define RCC_BASE 0x40023800

#define MODER_OFFSET 0x00
#define ODR_OFFSET 0x14
#define AHB1ENR_OFFSET 0x30
#define BSRR_OFFSET 0x18

#define delay_time 1000000

int main(void) {

	//gpioc enable
	*((uint32_t*) (RCC_BASE + AHB1ENR_OFFSET)) |= (1 << 2);

	//PC6 moder set as output

	//pc6
	*((uint32_t*) (GPIOC_BASE + MODER_OFFSET)) |= (1 << 12);

	//writing 0 in GPIOC pin 6 to turn ON LED
	*((uint32_t*) (GPIOC_BASE + ODR_OFFSET)) &= ~(1 << 6);

	while (1) {

	}

}
