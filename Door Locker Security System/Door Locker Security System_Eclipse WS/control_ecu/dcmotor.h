/******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: dcmotor.h
 *
 * Description: header file for the LM35 DC-Motor Driver
 *
 * Author: TASNEEM ESSMAT
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* DC-Motor HW Ports and Pins Ids */
#define DC_MOTOR_PORT1_ID                 PORTB_ID
#define DC_MOTOR_PIN1_ID                  PIN6_ID

#define DC_MOTOR_PORT2_ID                 PORTB_ID
#define DC_MOTOR_PIN2_ID                  PIN7_ID

/*******************************************************************************
 *               				  ENUMS		           		  				   *
 *******************************************************************************/
typedef enum{
	Stop, Clockwise, AntiClockwise
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * The function responsible for rotating the DC Motor CW/ or A-CW or stop the motor based on
 * the state input state value.
 * and Sending the required duty cycle to the PWM driver based on the required speed value.
 * */
void DcMotor_Init(void);

/*
 * The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DCMOTOR_H_ */
