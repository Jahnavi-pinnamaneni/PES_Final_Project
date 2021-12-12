/*
 * led.h
 *
 *  Created on: Dec 1, 2021
 *      Author: pjahn
 */

#ifndef LED_H_
#define LED_H_

void RED_LED_Init();

/*
 * @brief: Initializes the GPIO registers for GREEN led
 */
void GREEN_LED_Init();

/*
 * @brief: Initializes the GPIO registers for BLUE led
 */
void BLUE_LED_Init();

/*
 * @brief: Sets RED LED
 */
void RED_LED_ON();

/*
 * @brief: Resets RED LED
 */
void RED_LED_OFF();

/*
 * @brief: Sets GREEN LED
 */
void GREEN_LED_ON();

/*
 * @brief: Resets GREEN LED
 */
void GREEN_LED_OFF();

/*
 * @brief: Sets BLUE LED
 */
void BLUE_LED_ON();

/*
 * @brief: Resets BLUE LED
 */
void BLUE_LED_OFF();

/*
 * @brief: Sets WHITE LED
 */
void WHITE_LED_ON();

/*
 * @brief: Resets WHITE LED
 */
void WHITE_LED_OFF();


#endif /* LED_H_ */
