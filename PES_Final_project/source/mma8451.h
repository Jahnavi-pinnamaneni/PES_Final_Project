/*
 * mma8451.h
 *
 *  Created on: Nov 20, 2021
 * 	Author: Jahnavi Pinnamaneni
 */

#ifndef MMA8451_H_
#define MMA8451_H_

#include <stdint.h>
#include <stdbool.h>

#define MMA_ADDR 0x3A

#define REG_XHI 0x01
#define REG_XLO 0x02
#define REG_YHI 0x03
#define REG_YLO 0x04
#define REG_ZHI	0x05
#define REG_ZLO 0x06

#define REG_WHOAMI 0x0D
#define REG_CTRL1  0x2A
#define REG_CTRL4  0x2D
#define X_OFFSET_REG 0x2F
#define Y_OFFSET_REG 0x30
#define Z_OFFSET_REG 0x31

#define WHOAMI 0x1A

#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)

int init_mma(void);
void read_full_xyz(void);
void mma_calibrate(void);

extern int16_t acc_X, acc_Y, acc_Z;
extern int16_t X_offset, Y_offset, Z_offset;

#endif /* MMA8451_H_ */
