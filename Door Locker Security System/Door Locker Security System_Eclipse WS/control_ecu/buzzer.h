/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the Buzzer driver
 *
 * Author: TASNEEM ESSMAT
 *
 *******************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* BUZZER HW Port and Pin Ids */
#define BUZZER_PORT_ID		PORTA_ID
#define BUZZER_PIN_ID		PIN0_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * The function is responsiple for :
 * 			1- Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * 			2- Turn off the buzzer through the GPIO.
 */
void Buzzer_init();

/*
 * Function to enable the Buzzer through the GPIO.
 */
void Buzzer_on(void);
/*
 * Function to disable the Buzzer through the GPIO.
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
