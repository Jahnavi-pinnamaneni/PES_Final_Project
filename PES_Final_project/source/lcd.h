/*
 * lcd.h
 *
 *  Created on: Dec 9, 2021
 *  Author: Jahnavi Pinnamaneni; japi8358@colorado.edu
 */

#ifndef LCD_H_
#define LCD_H_

#include "MKL25Z4.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mma8451.h"

void lcd_init();
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_print_value(int16_t acc_X, int16_t acc_Y, int16_t acc_Z);
void lcd_print_sign(bool x_flag, bool y_flag, bool z_flag);
void lcd_putch(uint8_t add, uint8_t data);
void lcd_setup();

#endif /* LCD_H_ */
