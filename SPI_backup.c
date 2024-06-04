#include <stdint.h>
#include <stm32f4xx.h>

/*
 * connections -
 * 21->34 clk lines
 * 23->36 MOSI lines
 *
 * SPI 1 pins -
 *
 * PA4-> cs
 * PA5-> clk
 * PA6-> MISO
 * PA7-> MOSI
 *
 *
 * SPI 2 pins -
 *
 * PB15-> MOSI
 * PB14-> MISO
 * PB13-> clk
 * PB12-> cs
 *
 *
 *
 * */

uint8_t rx_data = 0;
uint8_t tx_data = 0;

int main(void) {

	RCC->APB2ENR |= (1 << 12); //SPI 1 enable
	RCC->APB1ENR |= (1 << 14); //SPI 2 enable

	RCC->AHB1ENR |= (1 << 0); //GPIOA enable
	RCC->AHB1ENR |= (1 << 1); //GPIOB enable

	GPIOA->MODER |= (2 << 8);  //pa4
	GPIOA->MODER |= (2 << 10); //pa5
	GPIOA->MODER |= (2 << 12); //pa6
	GPIOA->MODER |= (2 << 14); //pa7

	GPIOB->MODER |= (2 << 30); //pb15
	GPIOB->MODER |= (2 << 28); //pb14
	GPIOB->MODER |= (2 << 26); //pb13
	GPIOB->MODER |= (2 << 24); //pb12

	GPIOA->AFR[0] |= (5 << 16); //af5 pa4
	GPIOA->AFR[0] |= (5 << 20); //af5 pa5
	GPIOA->AFR[0] |= (5 << 24); //af5 pa6
	GPIOA->AFR[0] |= (5 << 28); //af5 pa7

	GPIOB->AFR[1] |= (5 << 16); //af5 pb12
	GPIOB->AFR[1] |= (5 << 20); //af5 pb13
	GPIOB->AFR[1] |= (5 << 24); //af5 pb14
	GPIOB->AFR[1] |= (5 << 28); //af5 pb15

	SPI1->CR1 |= (7 << 3); //baud rate division factor (256)
	SPI1->CR1 |= (1 << 2); //master mode
//	SPI1->CR1 |= (1 << 1); //cpol bit 1
//	SPI1->CR1 |= (1 << 0); //cphase bit 1
	SPI1->CR2 |= (1 << 2); //ssoe bit (chip select line acts as an output only)
	SPI1->CR1 |= (1 << 6); //spi enable

	SPI2->CR1 |= (1 << 9); //ssm bit
	SPI2->CR1 &= ~(1 << 8); //ssi bit
	SPI2->CR1 |= (1 << 6); //spi enable

	while (1) {

		while (!((SPI1->SR) & (1 << 1))); //checking if tx register is empty

		SPI1->DR = tx_data;

		while (!((SPI2->SR) & (1 << 0))); //checking if rx register is empty

		rx_data = SPI2->DR;

		for (int k = 0; k < 200000; k++);

	}
}
