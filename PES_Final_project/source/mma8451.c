/*
 * mma8451.c
 *
 *  Created on: Nov 20, 2021
 *  Author: Jahnavi Pinnamaneni; japi8358@colorado.edu
 */

#include <MKL25Z4.H>
#include "mma8451.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>
#include <stdio.h>

#include <fsl_debug_console.h>

#define SENSITIVITY (4096)
#define MASK 0x8000
#define CALIBRATE_AVG 100

bool x_flag = false, y_flag = false, z_flag = false;
int16_t acc_X=0, acc_Y=0, acc_Z=0;
int16_t X_offset = 0, Y_offset = 0, Z_offset = 0;



//initializes mma8451 sensor
//i2c has to already be enabled
//set active mode, 14 bit samples and 800 Hz ODR
int init_mma()
{
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	return 1;
}

void read_full_xyz()
{
	int i;
	uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	for( i=0; i<5; i++)	{
		data[i] = i2c_repeated_read(0);
	}
	data[i] = i2c_repeated_read(1);

	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	if(acc_X & MASK)
		x_flag = true;
	else
		x_flag = false;

	if(acc_Y & MASK)
		y_flag = true;
	else
		y_flag = false;

	if(acc_Z & MASK)
		z_flag = true;
	else
		z_flag = false;

	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
}




void mma_calibrate(void)
{
	printf("Place the device on a flat surface\r\n");
	delay(5000);
	printf("Calibration Begin\r\n");
	delay(5000);
	int32_t x_cal = 0, y_cal = 0, z_cal = 0;
	for(int i = 0; i <CALIBRATE_AVG ; i++)
	{
		read_full_xyz();
		x_cal += acc_X;
		y_cal += acc_Y;
		z_cal += acc_Z;
	}
	x_cal /= CALIBRATE_AVG;
	y_cal /= CALIBRATE_AVG;
	z_cal /= CALIBRATE_AVG;

	X_offset = -1 * x_cal;
	Y_offset = -1 * y_cal;
	Z_offset = (SENSITIVITY) - z_cal;

	printf("Offset %d %d %d \r\n", X_offset, Y_offset, Z_offset);
	printf("Calibration End\r\n");
}

