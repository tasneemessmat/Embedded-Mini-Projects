/******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: dcmotor.c
 *
 * Description: source file for the LM35 DC-Motor Driver
 *
 * Author: TASNEEM ESSMAT
 *
 *******************************************************************************/

#include "dcmotor.h"
#include "gpio.h"
#include "pwm.h"

void DcMotor_Init(void)
{
	/*Configure the two pins as Output pins */
	GPIO_setupPinDirection(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, PIN_OUTPUT);
	/* Motor is stop at the beginning */
	GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	uint8 a_dutyCycle=0;
	/*
	 * Rotate the DC Motor CW/ or A-CW or stop the motor based on the state input state value.
	 */
	if(state == Stop){
		GPIO_writePin(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, LOGIC_LOW);
	}
	else if(state == Clockwise)
	{
		GPIO_writePin(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, LOGIC_LOW);
	}
	else{
		GPIO_writePin(DC_MOTOR_PORT1_ID, DC_MOTOR_PIN1_ID, LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT2_ID, DC_MOTOR_PIN2_ID, LOGIC_HIGH);
	}
	/*
	 * Calculate the required duty cycle
	 */

	a_dutyCycle = (uint8)(((uint32)speed*255)/(100));

	PWM_Timer0_Start(a_dutyCycle);


}
