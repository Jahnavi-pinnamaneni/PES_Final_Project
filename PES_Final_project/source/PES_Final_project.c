/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Final_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "lcd.h"
#include "i2c.h"
#include "mma8451.h"
#include <stdbool.h>
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */


static void MainTask(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
	i2c_init();																/* init i2c	*/
	if (!init_mma()) {												/* init mma peripheral */

		while (1)																/* not able to initialize mma */
			;
	}
  for(;;) {
	//printf("Hello\r\n");
	read_full_xyz();
	convert_xyz_to_roll_pitch();

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}


static void LcdTask(void *pvParameters) {
  (void)pvParameters; /* parameter not used */
  lcd_init();
  lcd_setup();
  for(;;) {
		lcd_print_sign(x_flag, y_flag, z_flag);
		lcd_print_value(abs(acc_X), abs(acc_Y), abs(acc_Z));
    vTaskDelay(pdMS_TO_TICKS(600));
  }
}


/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\n");
    RED_LED_Init();
    RED_LED_OFF();
    if ((xTaskCreate(
        MainTask,  /* pointer to the task */
        "Main", /* task name for kernel awareness debugging */
        configMINIMAL_STACK_SIZE+500, /* task stack size */
        (void*)NULL, /* optional task startup argument */
        tskIDLE_PRIORITY + 1,  /* initial priority */
        (xTaskHandle*)NULL /* optional task handle to create */
      ) != pdPASS)) {
    /*lint -e527 */
       for(;;){} /* error! probably out of memory */
      /*lint +e527 */
    }
    if((xTaskCreate(
       	        LcdTask,  /* pointer to the task */
       	        "Lcd", /* task name for kernel awareness debugging */
       	        configMINIMAL_STACK_SIZE+500, /* task stack size */
       	        (void*)NULL, /* optional task startup argument */
       	        tskIDLE_PRIORITY,  /* initial priority */
       	        (xTaskHandle*)NULL /* optional task handle to create */
       	      ) != pdPASS))
    {
        /*lint -e527 */
           for(;;){} /* error! probably out of memory */
          /*lint +e527 */
    }
    vTaskStartScheduler();
    for(;;) { /* Infinite loop to avoid leaving the main function */
      __asm("NOP"); /* something to use as a breakpoint stop while looping */
    }
    return 0 ;
}
