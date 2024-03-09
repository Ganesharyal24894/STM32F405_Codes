#include <stdint.h>
#include <stm32f4xx.h>

//pb10 i2c 2 scl 29
//pb11 i2c 2 sda 30

uint8_t rx_data;
uint8_t tx_data = 65;

int main(void) {

	RCC->AHB1ENR |= (1 << 1); //b port

	RCC->APB1ENR |= (1 << 22); //i2c2 enable

	GPIOB->MODER &= ~(3 << 20); //clear pb10
	GPIOB->MODER |= (2 << 20); //af mode pb10
	GPIOB->MODER &= ~(3 << 22); //clear pb11
	GPIOB->MODER |= (2 << 22); //af mode pb11

	/*always set AFR just after MODER otherwise in I2C it sets Busy flag in
	 SR which will stop START condition to generate*/

	GPIOB->AFR[1] |= (4 << 8);
	GPIOB->AFR[1] |= (4 << 12);

	GPIOB->OTYPER |= (1 << 10); //od mode
	GPIOB->OTYPER |= (1 << 11); //od mode

	GPIOB->PUPDR &= ~(3 << 20); //clear pb10
	GPIOB->PUPDR |= (1 << 20); //pull up
	GPIOB->PUPDR &= ~(3 << 22); //clear pb11
	GPIOB->PUPDR |= (1 << 22); //pull up

	GPIOB->OSPEEDR |= (3 << 20); //pb10
	GPIOB->OSPEEDR |= (3 << 22); //pb11

	I2C2->CR1 |= (1 << 15); //reset
	I2C2->CR1 &= ~(1 << 15); //reset

	/*
	 * Frequency Calculation for I2C peripheral
	 *
	 * example -  I need 10KHz I2C clk signal
	 *
	 * so -> clk signal time period -> 1/10000 = 100000ns
	 *
	 * in Standard mode T_high will = T_low (since 50% duty cycle)
	 *
	 * so  T_high = 100us/2 = 50000ns
	 * and T_low = 100us/2 = 50000ns
	 *
	 *
	 *  lets say, FpCLK(in CR2 and APB1_CFGR register) = 8Mhz(minimum is 2MHz(SM) and 4MHz(FM)
	 *
	 *  then TpCLK = 1/8000000  = 125nS
	 *
	 *  and Thigh = CCR * TpCLK (we want T_high to be 50000ns)
	 *
	 *	t_high = CCR * Tpclk
	 *
	 *	t_high = CCR * 125ns
	 *
	 *	t_high/125 = CCR
	 *
	 *	50000ns/125ns = 400d
	 *
	 *
	 * */
	//this actually changes APB1BUS freq I2C is connected to this bus
	RCC->CFGR |= (4 << 10); //divided by 2
	I2C2->CR2 |= (8 << 0);  //(FpCLK)freq mhz for I2C peripheral
	I2C2->CCR |= (400<< 0);

	/* for TRISE time formula is (maximum_rise_time/TpCLK)= TRISE value
	 * trise = (max_rise_time/tpclk)+1
	 * trise = (1000ns/125ns)+1
	 * 				=0x09h
	 * */
	I2C2->TRISE = (0x09 << 0);

	//says in datasheet to set 14th bit as '1' but works without this as well
	I2C2->OAR1 |= (1 << 14);

	I2C2->CR1 |= (1 << 0); //pe enable

	while (1) {

		I2C2->CR1 |= (1 << 8); //start condition

		while (!((I2C2->SR1) & (1 << 0))) {
		}; //check if sb bit is set

		//dummy read followed by writing addr to DR clears SB bit
		(void) I2C2->SR1;
		I2C2->DR = (0x27 << 1); //address

		while (!((I2C2->SR1) & (1 << 1))) {
		}; //checking if addr sent

		//necessary to clear ADDR bit
		(void) I2C2->SR1; //dummy read
		(void) I2C2->SR2; //dummy read

		while (!((I2C2->SR1) & (1 << 7))) {
		}; //checking tx register empty

		I2C2->DR = 0x00; //data

		while (!((I2C2->SR1) & (1 << 2))) {
		}; //checking byte transfered BTF bit

		I2C2->CR1 |= (1 << 9); //stop condition
		while (!((I2C2->SR2) & (1 << 1))) {
		}; //checking stop condition

		for (int k = 0; k < 300000; k++)
			;

	}

}
