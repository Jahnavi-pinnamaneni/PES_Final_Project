/*
 * mma8451.c
 *
 *  Created on: Nov 20, 2021
 *      Author: pjahn
 */

#include <MKL25Z4.H>
#include "mma8451.h"
#include "i2c.h"
#include "delay.h"
#include <math.h>
#include <stdio.h>

#include <fsl_debug_console.h>

#define SENSITIVITY (4096*4)
#define MASK 0x8000
#define CALIBRATE_AVG 100

bool x_flag = false, y_flag = false, z_flag = false;
int16_t acc_X=0, acc_Y=0, acc_Z=0;
float roll=0.0, pitch=0.0;
int16_t X_offset = 0, Y_offset = 0, Z_offset = 0;
//mma data ready
extern uint32_t DATA_READY;



//initializes mma8451 sensor
//i2c has to already be enabled
int init_mma()
{
	//set active mode, 14 bit samples and 800 Hz ODR
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

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
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

//	acc_X = temp[0]/SENSITIVITY;
//	acc_Y = temp[1]/SENSITIVITY;
//	acc_Z = temp[2]/SENSITIVITY;
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
	printf("x: %d, y:%d, z:%d\r\n",acc_X, acc_Y, acc_Z);
}




void mma_calibrate(void)
{
	printf("Place the device on a flat surface, Calibration will begin in 10 secs\r\n");
	delay(10000);
	printf("Calibration Begin\r\n");
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
	Z_offset = (SENSITIVITY/4) - z_cal;

	printf("Offset %d %d %d \r\n", X_offset, Y_offset, Z_offset);
	printf("Calibration End\r\n");
}

//mma data ready irq
// void PORTA_IRQHandler()
// {
// 	NVIC_ClearPendingIRQ(PORTA_IRQn);
// 	DATA_READY = 1;
// }
