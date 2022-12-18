/*
 * StopWatch.c
 *
 *  Created on: Sep 16, 2022
 *      Author: TASNEEM
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include<avr/delay.h>


/* global variables contain the 7-segment count value */
unsigned char SEC1 = 0;					//	Counter 1 for Seconds
unsigned char SEC2 = 0;					//	Counter 2 for Seconds
unsigned char MIN1 = 0;					//	Counter 1 for Minutes
unsigned char MIN2 = 0;					//	Counter 2 for Minutes
unsigned char HOUR1 = 0;				//	Counter 1 for Hours
unsigned char HOUR2 = 0; 				//	Counter 2 for Hours

/* global variable to indicate timer1 interrupts */
unsigned char Timer1_INT_Flag = 0;

void Timer1_CTC_MODE_Init(void);
void INT2_Init(void);
void INT1_Init(void);
void INT0_Init(void);



/* External INT0 Interrupt Service Routine */
ISR(INT0_vect)
{
	/* Reset All Counters */
	SEC1 = 0;
	SEC2 = 0;
	MIN1 = 0;
	MIN2 = 0;
	HOUR1 = 0;
	HOUR2 = 0;
}

/* External INT1 Interrupt Service Routine */
ISR(INT1_vect)
{
	/* Clear bits CS12, CS11 and CS10 to stop the timer (No Clock Source) */

	TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10);
}

/* External INT2 Interrupt Service Routine */
ISR(INT2_vect)
{
	/*Enable Clock Source again at prescaler 1024
	Set Bits CS12, CS10*/

	TCCR1B |= (1<<CS12) | (1<<CS10);

}

/* Interrupt Service Routine for timer0 compare mode */
ISR(TIMER1_COMPA_vect)
{
	/* set the interrupt flag to indicate that INT occurred */

	Timer1_INT_Flag = 1;
}




int main (void)
{
	unsigned short i ;					// index for looping
	DDRC |= 0x0F; 						// use first 4 pins of C as output to MCU (into decoder)
	PORTC &= 0xF0;						// 7-segment display zero at the beginning.

	DDRA |= 0x3F;						// use first 6-pins in PORTA as output for enabling or disabling the 6 7-Segments

	INT0_Init();						// Enable external INT0
	INT1_Init();						// Enable external INT1
	INT2_Init();						// Enable external INT2

	Timer1_CTC_MODE_Init();				// Start the timer.






	while(1)
	{
		if(Timer1_INT_Flag == 1){

			if(SEC1 < 9)
			{
				SEC1++;
			}
			else
			{
				SEC1 = 0;
				if(SEC2 < 5)
				{
					SEC2++;
				}
				else
				{
					SEC2=0;
					if(MIN1 < 9)
					{
						MIN1++;
					}
					else
					{
						MIN1 = 0;
						if(MIN2 < 5)
						{
							MIN2++;
						}
						else
						{
							MIN2 = 0;
							if(HOUR1 < 9)
							{
								HOUR1++;
							}
							else
							{
								HOUR1 = 0;
								if(HOUR2 < 9)
								{
									HOUR2++;
								}
								else
								{
									HOUR2 = 0;
								}
							}
						}
					}

				}
			}
			// reset the interrupt event flag
			Timer1_INT_Flag = 0;
		}


			PORTA = (PORTA & 0xC0) | (1<<PA0);
			PORTC = (PORTC & 0xF0) | (SEC1 & 0x0F);
			_delay_us(0.0001);
			PORTA = (PORTA & 0xC0) | (1<<PA1);
			PORTC = (PORTC & 0xF0) | (SEC2 & 0x0F);
			_delay_us(0.0001);
			PORTA = (PORTA & 0xC0) | (1<<PA2);
			PORTC = (PORTC & 0xF0) | (MIN1 & 0x0F);
			_delay_us(0.0001);
			PORTA = (PORTA & 0xC0) | (1<<PA3);
			PORTC = (PORTC & 0xF0) | (MIN2 & 0x0F);
			_delay_us(0.0001);
			PORTA = (PORTA & 0xC0) | (1<<PA4);
			PORTC = (PORTC & 0xF0) | (HOUR1 & 0x0F);
			_delay_us(0.0001);
			PORTA = (PORTA & 0xC0) | (1<<PA5);
			PORTC = (PORTC & 0xF0) | (HOUR2 & 0x0F);
			_delay_us(0.0001);





	}
}


/* For System clock=1Mhz and prescale F_CPU/1024.
 * Timer frequency will be around 1khz, Ttimer = 1ms
 * So we just need 1000 counts to get 1 second.
 * Compare interrupt will be generated every 1000ms (1 second) to increment our stop watch.
 * 1. Non PWM mode FOC1A=1
 * 2. Compare Mode WGM12 =1
 * 3. clock = F_CPU/1024 CS12=1 CS11=0 CS10=1
 */
void Timer1_CTC_MODE_Init(void)
{
	TCNT1 = 0;				//Initialize timer1 count to 0

	TCCR1A = (1 << FOC1A);
	TCCR1B = (1 << WGM12) | (1 << CS12 )| (1 << CS10); //pre-scalar 1024

	TIMSK = (1<<OCIE1A); 	// Enable Timer1 compare A interrupt
	SREG |= (1<<7);			// Enable interrupts by setting I-bit
	OCR1A = 1000;			// each second as the one count cost 1 milli-sec so we need 1000 m-s

}

/* External INT0 enable and configuration function */
void INT0_Init(void)
{
	DDRD &= ~(1<<PD2);						// SPECFIY PD2 AS INPUT pin(INT0)PUSH BUTTONS
	SREG |= (1<<7);							// Enable interrupts by setting I-bit
	GICR |= (1<<INT0);						// Enable external interrupt pin INT0
	MCUCR |= (1<<ISC01);					// Trigger INT0 with the FALLING edge
	MCUCR &= ~(1<<ISC00);
}

/* External INT1 enable and configuration function */
void INT1_Init(void)
{
	DDRD &= ~(1<<PD3);						// SPECFIY PD3 AS INPUT pin(INT1)PUSH BUTTONS
	SREG |= (1<<7);							// Enable interrupts by setting I-bit
	GICR |= (1<<INT1);						// Enable external interrupt pin INT1
	MCUCR |= (1<<ISC11); 					// Trigger INT1 with the RISING edge
	MCUCR |= (1 << ISC10);
}

/* External INT2 enable and configuration function */
void INT2_Init(void)
{
	DDRB &= ~(1<< PB2); 	// SPECFIY PB2 AS INPUT PIN (INT2)PUSH BUTTON
	SREG |= (1<<7);			// Enable interrupts by setting I-bit
	GICR |= (1<<INT2);		// Enable external interrupt pin INT2
	MCUCSR &= ~(1<<ISC2);	// Trigger INT2 with the FALLING edge
}
