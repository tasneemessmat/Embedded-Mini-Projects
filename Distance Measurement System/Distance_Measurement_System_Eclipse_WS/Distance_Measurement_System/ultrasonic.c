/*
 * ultrasonic.c
 *
 *  Created on: Oct 11, 2022
 *      Author: TASNEEM
 */
#include "ultrasonic.h"

#include "icu.h"
#include "gpio.h"
#include <util/delay.h>





/* Create configuration structure for ICU driver*/
static Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
/* global variable contain the ticks count of the timer */


void Ultrasonic_init(void)
{
	/* Initialize the ICU driver as required*/
	Icu_init(&Icu_Config);

	/* Set the Call back function pointer in the ICU driver */
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	/*Setup the direction for the trigger pin as output pin through the GPIO driver*/
	GPIO_setupPinDirection(ULTRASONIC_Trig_PORT_ID , ULTRASONIC_Trig_PIN_ID, PIN_OUTPUT);

}
void Ultrasonic_Trigger(void)
{
	GPIO_writePin(ULTRASONIC_Trig_PORT_ID, ULTRASONIC_Trig_PIN_ID, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(ULTRASONIC_Trig_PORT_ID, ULTRASONIC_Trig_PIN_ID, LOGIC_LOW);
}


uint16 Ultrasonic_readDistance(void)
{
	uint16 a_ultrasonicDistance = 0;
	Ultrasonic_Trigger();

	a_ultrasonicDistance = g_timeHigh / SPEED_TIMER_PARAM;

	return a_ultrasonicDistance;
}

void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/*Detect Falling Edge instead*/
		Icu_setEdgeDetectionType(FALLING);

	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value T1 */
		g_timeHigh = Icu_getInputCaptureValue();

		/* Clear the timer counter register to start measurements again */
		Icu_clearTimerValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
		g_edgeCount = 0;

	}
}