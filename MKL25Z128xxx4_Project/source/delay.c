/*
 * delay.c
 *
 *  Created on: Dec 11, 2021
 *  Author: Jahnavi Pinnamaneni; japi8358@colorado.edu
 */

#include "delay.h"

void delay(int ms)
{
	ms = 5330 * ms;
	while(ms--)
	{
		__asm ("nop");
	}
}

void Delay (uint32_t dly) {
  volatile uint32_t t;

	for (t=dly*10000; t>0; t--)
		;
}
