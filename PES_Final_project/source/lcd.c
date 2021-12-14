/*
 * lcd.c
 *
 *  Created on: Dec 9, 2021
 *  Author: Jahnavi Pinnamaneni; japi8358@colorado.edu
 */

#include "lcd.h"
#include "delay.h"
#include <math.h>

#define PORTC_MASK 11
#define MASK(x) (1 << (x))

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

#define X_ADD (0x83)
#define Y_ADD (0x8C)
#define Z_ADD (0xC7)

#define X 0x80
#define Y 0x89
#define Z 0xC4

#define X_COLON 0x81
#define Y_COLON 0x8A
#define Z_COLON 0xC5

#define X_SIGN 0x82
#define Y_SIGN 0x8B
#define Z_SIGN 0xC6

#define SIGN_MASK 0x8000

#define BIT_1 0x01
#define BIT_2 0x02
#define BIT_3 0x04
#define BIT_4 0x08
#define BIT_5 0x10
#define BIT_6 0x20
#define BIT_7 0x40
#define BIT_8 0x80

#define INIT 0x30
#define FUNCTION_SET 0x38
#define SET_CURSOR 0x10
#define DISPLAY_ON 0x0F
#define ENTRY_MODE 0x06

/*
 * This function sets the GPIO configuration
 */
static void port_cfg(uint8_t var)
{
	PORTC->PCR[var] &= PORT_PCR_MUX_MASK;
	PORTC->PCR[var] = PORT_PCR_MUX(1);
	PTC->PDDR |= MASK(var);
}

/*
 * This functions sets the GPIO output to LOW
 */
static void clear_pin(uint8_t var)
{
	GPIOC->PCOR |= MASK(var);
}

/*
 * This function sets the GPIO output to HIGH
 */
static void set_pin(uint8_t var)
{
	GPIOC->PSOR |= MASK(var);
}

/*
 * This function gates the clock for PORT C and configures the control and data pins required
 * for communication with LCD
 */
static void GPIO_Init()
{
	SIM->SCGC5 |= MASK(PORTC_MASK);
	port_cfg(RS);
	port_cfg(RW);
	port_cfg(E);

	port_cfg(D0);
	port_cfg(D1);
	port_cfg(D2);
	port_cfg(D3);
	port_cfg(D4);
	port_cfg(D5);
	port_cfg(D6);
	port_cfg(D7);
}

/*
 * This function is used to clear the data lines
 */
static void clear_port()
{
	clear_pin(D0);
	clear_pin(D1);
	clear_pin(D2);
	clear_pin(D3);
	clear_pin(D4);
	clear_pin(D5);
	clear_pin(D6);
	clear_pin(D7);
}

/*
 * Since Data lines are not connected to KL25Z in a sequential order, the data needs to be masked to
 * set the correct pins
 */
static void lcd_port_write(uint8_t data)
{
	clear_port();
	if(data & BIT_1)
		set_pin(D0);
	if(data & BIT_2)
		set_pin(D1);
	if(data & BIT_3)
		set_pin(D2);
	if(data & BIT_4)
		set_pin(D3);
	if(data & BIT_5)
		set_pin(D4);
	if(data & BIT_6)
		set_pin(D5);
	if(data & BIT_7)
		set_pin(D6);
	if(data & BIT_8)
		set_pin(D7);
}

/*
 * This is a helper function to set or clear the Register select pin
 */
static void lcd_rs(uint8_t data)
{
	if(data)
		set_pin(RS);
	else
		clear_pin(RS);
}

/*
 * This is a helper function to set or clear the Read/Write pin
 */
static void lcd_rw(uint8_t data)
{
	if(data)
		set_pin(RW);
	else
		clear_pin(RW);
}

/*
 * This is a helper function to set or clear the Enable pin
 */
static void lcd_e(uint8_t data)
{
	if(data)
		set_pin(E);
	else
		clear_pin(E);
}

/*
 * This function writes instructions to the LCD display
 */
void lcd_cmd(uint8_t cmd)
{
	lcd_rs(0);
	lcd_rw(0);
	lcd_port_write(cmd);
	lcd_e(1);
	delay(1);
	lcd_e(0);
}

/*
 * This function writes data to the LCD Display
 */
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

/*
 * This function initializes the LCD Display
 */
void lcd_init()
{
	GPIO_Init();
	lcd_cmd(INIT);
	delay(30);
	lcd_cmd(INIT);
	delay(10);
	lcd_cmd(INIT);
	delay(10);
	lcd_cmd(FUNCTION_SET);
	lcd_cmd(SET_CURSOR);
	lcd_cmd(DISPLAY_ON);
	lcd_cmd(ENTRY_MODE);
}

/*
 * This function sets the constant characters on the LCD display
 *  ___________________________________
 * | X:                      Y:        |
 * |___________Z:______________________|
 *
 * The addresses are used only in this one place and do not change through course of excecution hence
 * they are defined with preprocessor statement
 */
void lcd_setup()
{
	//X
	lcd_cmd(X);
	lcd_data('X');
	lcd_cmd(X_COLON);
	lcd_data(':');


	//Y
	lcd_cmd(Y);
	lcd_data('Y');
	lcd_cmd(Y_COLON);
	lcd_data(':');


	//Z
	lcd_cmd(Z);
	lcd_data('Z');
	lcd_cmd(Z_COLON);
	lcd_data(':');

}

/*
 * Helper function to put a character on the display
 */
void lcd_putch(uint8_t add, uint8_t data)
{
	lcd_cmd(add);
	lcd_data(data);
}

/*
 * This function checks if the accelerometer reading is positive or negative and displays +/- accordingly
 */
void lcd_print_sign(bool x_flag, bool y_flag, bool z_flag)
{
	if(x_flag == true)
		lcd_putch(X_SIGN, '-');
	else
		lcd_putch(X_SIGN, '+');

	if(y_flag == true)
		lcd_putch(Y_SIGN, '-');
	else
		lcd_putch(Y_SIGN, '+');

	if(x_flag == true)
		lcd_putch(Z_SIGN, '-');
	else
		lcd_putch(Z_SIGN, '+');
}

/*
 * This function parses the input accelerometer reading for the number digits and converts them string
 * so that the numbers can be displayed on the LCD
 */
void lcd_print (uint8_t add,uint16_t num)
{
	uint8_t temp[4];
	uint8_t digits = (log10(num) +1);
	for(int i = 0; i<digits; i++)
	{
		*(temp+i) = num % 10;
		num = num/10;
	}
	for(int i =0; i<4; i++)
	{
		lcd_putch((add+i), 0x20);
	}
	for(int i =0; i<digits; i++)
	{
		lcd_putch(add, (temp[digits - 1 - i] + '0'));
		add++;
	}
}

/*
 * This function prints the X, Y and Z values on the LCD Display
 */
void lcd_print_value(int16_t acc_X, int16_t acc_Y, int16_t acc_Z)
{
	bool x_flag = false, y_flag = false, z_flag = false;
	if(acc_X & SIGN_MASK)
		x_flag = true;
	else
		x_flag = false;

	if(acc_Y & SIGN_MASK)
		y_flag = true;
	else
		y_flag = false;

	if(acc_Z & SIGN_MASK)
		z_flag = true;
	else
		z_flag = false;
	lcd_print_sign(x_flag, y_flag, z_flag);

	printf("x: %d, y: %d, z: %d \r\n", ((acc_X>>2) + X_offset),((acc_Y>>2) + Y_offset), ((acc_Z>>2) + Z_offset) );
	lcd_print(X_ADD, abs((acc_X>>2) + X_offset));
	lcd_print(Y_ADD, abs((acc_Y>>2) + Y_offset));
	lcd_print(Z_ADD, abs((acc_Z>>2) + Z_offset));
}
