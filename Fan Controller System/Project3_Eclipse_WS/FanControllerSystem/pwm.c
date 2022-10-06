/******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: pwm.c
 *
 * Description: source file for the PWM Driver
 *
 * Author: TASNEEM ESSMAT
 *
 *******************************************************************************/
#include"pwm.h"
#include "avr/io.h" /* To use the Timer Registers */

/*Generate a PWM signal with frequency 500Hz*/
void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = 0; // Set Timer Initial Value to 0
	/*
	 * TCCR0 Register Bits Description:
	 * FOC0 bit:  0 is not active as it's PWM mode
	 * WGM01:0: 11 as Fast PWM Mode
	 * COM01:0: 10 as (non-inverting mode)
	 * CS02:0:  010 Clock Select F_CPU/8.
	 */
	TCCR0 = (1<< WGM01) | (1<<WGM00) |(1<<COM01) |(1<<CS01);

	OCR0 = duty_cycle;

	DDRB  = DDRB | (1<<PB3); //set PB3/OC0 as output pin

}

