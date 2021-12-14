/*
 * delay.h
 *
 *  Created on: Dec 11, 2021
 *  Author: Jahnavi Pinnamaneni;japi8358@colorado.edu
 */

#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

/*
 * This function generates delay in ms
 *
 * Parameters: required msec
 */
void delay(int ms);

/*
 * This functions generates delay for specified number of counts
 *
 * Parameters: required count
 */
void Delay (uint32_t dly);

#endif /* DELAY_H_ */
