#include <stdint.h>

/*
 This is for configuring simple LED blinking on PORTB-3(PB3)
*/

#define RCC_BASE 0x40021000
#define IOPENR_OFFSET 0x34

#define GPIOB_BASE 0x50000400
#define MODER_OFFSET 0x00
#define ODR_OFFSET 0x14

int main(void) {

 *((uint32_t*) (RCC_BASE + IOPENR_OFFSET)) |= (1 << 1); //enabling port B

 *((uint32_t*) (GPIOB_BASE + MODER_OFFSET)) |= (1 << 6); //setting it as Output
 *((uint32_t*) (GPIOB_BASE + MODER_OFFSET)) &= ~(1 << 7); //setting it as Output

while (1) {
  *((uint32_t*) (GPIOB_BASE + ODR_OFFSET)) ^= (1 << 3); //toggling Output 
  
  //give some software delay
  for (int k = 0; k < 500000; k++);
 
  }

}
