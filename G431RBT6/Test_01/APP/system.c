#include "system.h"

void system_init(void)
{
	GPIOC->ODR &= 0x00ff;
	GPIOC->ODR |= ~(0x00 << 8);
	GPIOD->BSRR |= 0x01 << 2;
	GPIOD->BRR |= 0x01 << 2;
}