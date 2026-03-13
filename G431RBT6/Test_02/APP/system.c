#include "system.h"

void system_init(void)
{
	GPIOC->ODR |= 0xff00;
	GPIOD->BSRR |= 0x01 << 2;
	GPIOD->BRR |= 0x01 << 2;
}

