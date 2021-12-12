/*
 * lcd.c
 *
 *  Created on: Dec 9, 2021
 *  Author: Jahnavi Pinnamaneni; japi8358@colorado.edu
 */

#include "lcd.h"
#include "delay.h"

#define PORTC_MASK 11
#define RS 7
#define RW 0
#define E 3
#define D0 4
#define D1 5
#define D2 6
#define D3 10
#define D4 11
#define D5 12
#define D6 13
#define D7 16
#define MASK(x) (1 << (x))

static void GPIO_Init()
{
	//Try using PORTC->GPCLR
	SIM->SCGC5 |= MASK(PORTC_MASK);
	PORTC->PCR[RS] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[RS] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(RS);
	PORTC->PCR[RW] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[RW] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(RW);
	PORTC->PCR[E] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[E] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(E);

	PORTC->PCR[D0] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[D0] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(D0);
	PORTC->PCR[D1] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[D1] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(D1);
	PORTC->PCR[D2] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[D2] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(D2);
	PORTC->PCR[D3] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[D3] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(D3);
	PORTC->PCR[D4] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[D4] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(D4);
	PORTC->PCR[D5] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[D5] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(D5);
	PORTC->PCR[D6] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[D6] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(D6);
	PORTC->PCR[D7] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[D7] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(D7);
}

static void clear_port()
{
	GPIOC->PCOR |= MASK(D0);
	GPIOC->PCOR |= MASK(D1);
	GPIOC->PCOR |= MASK(D2);
	GPIOC->PCOR |= MASK(D3);
	GPIOC->PCOR |= MASK(D4);
	GPIOC->PCOR |= MASK(D5);
	GPIOC->PCOR |= MASK(D6);
	GPIOC->PCOR |= MASK(D7);
//	GPIOC->PCOR |= 0xFFFF;
}

static void lcd_port_write(uint8_t data)
{
	clear_port();
	if(data & 0x01)
		GPIOC->PSOR |= MASK(D0);
	if(data & 0x02)
		GPIOC->PSOR |= MASK(D1);
	if(data & 0x04)
		GPIOC->PSOR |= MASK(D2);
	if(data & 0x08)
		GPIOC->PSOR |= MASK(D3);
	if(data & 0x10)
		GPIOC->PSOR |= MASK(D4);
	if(data & 0x20)
		GPIOC->PSOR |= MASK(D5);
	if(data & 0x40)
		GPIOC->PSOR |= MASK(D6);
	if(data & 0x80)
		GPIOC->PSOR |= MASK(D7);
}

static void lcd_rs(uint8_t data)
{
	if(data)
		GPIOC->PSOR |= MASK(RS);
	else
		GPIOC->PCOR |= MASK(RS);
}

static void lcd_rw(uint8_t data)
{
	if(data)
		GPIOC->PSOR |= MASK(RW);
	else
		GPIOC->PCOR |= MASK(RW);
}

static void lcd_e(uint8_t data)
{
	if(data)
		GPIOC->PSOR |= MASK(E);
	else
		GPIOC->PCOR |= MASK(E);
}

void lcd_cmd(uint8_t cmd)
{
	lcd_rs(0);
	lcd_rw(0);
	lcd_port_write(cmd);
	lcd_e(1);
	delay(1);
	lcd_e(0);
}

//void lcd_busywait()
//{
//	lcd_rs(0);
//	lcd_rw(1);
//	while(GPIOC->PDOR & 0x00010000);
//}

void lcd_data(uint8_t data)
{
	GPIOC->PCOR |= 0xFFFF;
	lcd_rs(1);
	lcd_rw(0);
	lcd_port_write(data);
	lcd_e(1);
	delay(1);
	lcd_e(0);
}
void lcd_init()
{
	GPIO_Init();
	lcd_cmd(0x30);
	delay(30);
	lcd_cmd(0x30);
	delay(10);
	lcd_cmd(0x30);
	delay(10);
	lcd_cmd(0x38);
	lcd_cmd(0x00);
	lcd_cmd(0x10);
	lcd_cmd(0x0F);
	lcd_cmd(0x06);
}
void lcd_setup()
{
	//X
	lcd_cmd(0x80);
	lcd_data('X');
	lcd_cmd(0x81);
	lcd_data(':');
	lcd_cmd(0x84);
	lcd_data('g');

	//Y
	lcd_cmd(0x8B);
	lcd_data('Y');
	lcd_cmd(0x8C);
	lcd_data(':');
	lcd_cmd(0x8F);
	lcd_data('g');

	//Z
	lcd_cmd(0xC5);
	lcd_data('Z');
	lcd_cmd(0xC6);
	lcd_data(':');
	lcd_cmd(0xC9);
	lcd_data('g');
}


void lcd_putch(uint8_t add, uint8_t data)
{
	lcd_cmd(add);
	lcd_data(data);
}

void lcd_print_sign(bool x_flag, bool y_flag, bool z_flag)
{
	if(x_flag == true)
		lcd_putch(0x82, '-');
	else
		lcd_putch(0x82, '+');

	if(y_flag == true)
		lcd_putch(0x8D, '-');
	else
		lcd_putch(0x8D, '+');

	if(x_flag == true)
		lcd_putch(0xC7, '-');
	else
		lcd_putch(0xC7, '+');
}

void lcd_print_value(int16_t acc_X, int16_t acc_Y, int16_t acc_Z)
{
	lcd_putch(0x83, (acc_X + '0'));
	lcd_putch(0x8E, (acc_Y + '0'));
	lcd_putch(0xC8, (acc_Z + '0'));
}
