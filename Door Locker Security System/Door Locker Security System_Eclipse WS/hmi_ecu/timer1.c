 /******************************************************************************
 *
 * Module: Timer1
 *
 * File Name: timer1.c
 *
 * Description: Source file for Timer1 AVR driver
 *
 * Author: TASNEEM ESSMAT
 *
 *******************************************************************************/
#include "timer1.h"
#include "avr/interrupt.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	/* Set Timer1 initial value to input value */
	TCNT1 = Config_Ptr -> initial_value;

	if (Config_Ptr -> mode == CTCMode){

		/* Set Compare value as required */
		OCR1A  = Config_Ptr -> compare_value;

		/* Enable Timer1 Compare A Interrupt */
		TIMSK |= (1<<OCIE1A);

		/* Configure timer control register TCCR1A
		 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
		 * 2. FOC1A=1 FOC1B=0
		 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
		 */
		TCCR1A = (1<<FOC1A);

		/* Configure timer control register TCCR1B
		 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
		 * 2. Prescaler : insert the required clock value in the first three bits (CS10, CS11 and CS12)
		 */
		TCCR1B = (1<<WGM12) | (Config_Ptr -> prescaler);
	}
	else if(Config_Ptr -> mode == NormalMode){
		/* Enable Timer1 Overflow Interrupt	*/
		TIMSK |= (1<<TOIE1);

		/* Timer1 operates in Normal Mode */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B);

		/*
		 * insert the required clock value in the first three bits (CS10, CS11 and CS12)
		 * of TCCR1B Register
		 */
		TCCR1B = (Config_Ptr->prescaler);
	}
	else {
		/*#error "This Mode is Not Supported"
		 * Do Nothing
		 */
	}
}

void Timer1_deInit(void)
{
	/* Disconnect clock source and stop the timer
	 * Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;


	/* Disable Timer1 Overflow Interrupt and Timer1 Compare A Interrupt respectively */
	TIMSK &= ~(1<<TOIE1) & ~(1<<OCIE1A) ;
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = *a_ptr;
}

