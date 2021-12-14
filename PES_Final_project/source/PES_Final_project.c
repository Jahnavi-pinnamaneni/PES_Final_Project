 
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
#include "lcd.h"
#include "i2c.h"
#include "mma8451.h"
#include <stdbool.h>
#include <assert.h>
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define MMA_task_delay 500
#define LCD_task_delay 600

/*
 * This task reads the acceleration vectors in the X, Y and Z directions after initializing
 * i2c communication, the MMA8541Q module and calibrating the same.
 * The task is scheduled to execute every 500ms and this checked with the assert statement
 */
static void MMATask(void *pvParameters) {
	i2c_init();
	if (!init_mma()) {

		while (1)
			;
	}
	mma_calibrate();
	bool test = false;
	for(;;) {
		static uint32_t duration = 0;
		static uint32_t prev_duration = 0;

		duration = xTaskGetTickCount() - prev_duration;
		if(test)
			assert(duration == MMA_task_delay);

		read_full_xyz();

		prev_duration = xTaskGetTickCount();
		test = true;
		vTaskDelay(pdMS_TO_TICKS(MMA_task_delay));

	}
}

/*
 * This task displays the accelerometer readings in the LCD.
 * This task executes every 600ms.
 */
static void LcdTask(void *pvParameters) {
  lcd_init();
  lcd_setup();
  bool test = false;
  for(;;) {
		static uint32_t duration = 0;
		static uint32_t prev_duration = 0;

	    duration = xTaskGetTickCount() - prev_duration;
	    printf("%u msec\n", duration);
	    if(test)
	        	assert(duration == LCD_task_delay);

		lcd_print_value(acc_X, acc_Y, acc_Z );

		prev_duration = xTaskGetTickCount();
		test = true;
		vTaskDelay(pdMS_TO_TICKS(LCD_task_delay));

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

    if ((xTaskCreate(
        MMATask,
        "MMA8451Q",
        configMINIMAL_STACK_SIZE+500,
        (void*)NULL,
        tskIDLE_PRIORITY + 1,
        (xTaskHandle*)NULL
      ) != pdPASS))
    {
       for(;;){}
    }

    if((xTaskCreate(
		LcdTask,
		"Lcd",
		configMINIMAL_STACK_SIZE+500,
		(void*)NULL,
		tskIDLE_PRIORITY,
		(xTaskHandle*)NULL
	  ) != pdPASS))
    {
       for(;;){}
    }

    vTaskStartScheduler();

    for(;;) {
      __asm("NOP");
    }

    return 0 ;
}
