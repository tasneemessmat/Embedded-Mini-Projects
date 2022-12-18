/*
 * control_services.c
 *
 *  Created on: Nov 4, 2022
 *      Author: TASNEEM
 */

#include "control_services.h"
#include "uart.h"
#include "external_eeprom.h"
#include "timer1.h"
#include "twi.h"
#include <util/delay.h>
#include "dcmotor.h"
#include "buzzer.h"
#include <avr/io.h>

/* global variable contain the ticks count of the timer */
uint32 g_tick = 0;


/*Variables for storing password*/
uint8 password [6];
uint8 ConfirmPassword [6];
uint8 PasswordStatus;

void CONTROL_init(void)
{
	/* Initialize The UART */
	Uart_ConfigType UARTConfig = {9600, Disabled, OneBit, EightBits};
	UART_init(&UARTConfig);

	TWI_ConfigType TWIConfig = {0x01,2};
	/* Initialize the TWI/I2C Driver */
	TWI_init(&TWIConfig);

	DcMotor_Init();
	SREG |= (1<<7);
	Timer1_setCallBack(CONTROL_timer1);




}
void CONTROL_receivePassword(uint8 * password){
	for(int i =0 ; i< 5 ; i++)
	{
		while(UART_recieveByte() != HMI_ECU_READY){}

		/* Now send that i'm ready to receive any passwords! */
		UART_sendByte(CONTROL_ECU_READY);
		password[i] = UART_recieveByte();
	}
}

void CONTROL_savePasswordsEEPROM(uint16 address,uint8 * password){
	int i;
	for(i =0 ; i < 5;i++ ){
		EEPROM_writeByte(address+i, password[i]); /* Write every character in the external EEPROM - Any address */
		_delay_ms(10); /* Very Important Delay */
	}

}
/* Function for Creating password for the system or changing it*/
void CONTROL_createPassword_changePassword(void){
	uint8 i=0;
	/*First Loop Is responsible for Creating a Password for the system*/
	while(1){
		CONTROL_receivePassword(password);
		CONTROL_receivePassword(ConfirmPassword);

		/* Check if 2 passwords are identical*/
		for ( i =0; i < 5; i++){
			if (password[i] != ConfirmPassword[i])
			{	PasswordStatus = UNMATCHED_PASSWORDS;
			break;
			}
			else {
				PasswordStatus = MATCHED_PASSWORDS;
			}
		}
		CONTROL_sendResponse(PasswordStatus);
		if(PasswordStatus == MATCHED_PASSWORDS){
			CONTROL_savePasswordsEEPROM(0x0311, password);
			break;
		}
		else if(PasswordStatus == UNMATCHED_PASSWORDS){
			/*Repeat same process*/
		}

	}
}
uint8 CONTROL_checkPasswordinMemory(uint16 address, uint8 * password ){
	uint8 status,i,x;

	for(i =0 ; i<5 ; i++){
		x=0;
		EEPROM_readByte(address+i,&x);
		if (x == password[i]){
			status = MATCHED_PASSWORDS;
		}
		else{
			status = UNMATCHED_PASSWORDS;
		}

	}
	return status;
}

void CONTROL_sendResponse (uint8  PasswordStatus)
{
    UART_sendByte(CONTROL_ECU_READY);
    /* Now send if they are matching or not to the HMI ECU */
    /* First wait for him to be ready */
    while(UART_recieveByte() != HMI_ECU_READY){}

    UART_sendByte(PasswordStatus);
}
void CONTROL_openDoor(void){
	/* Structure for adjusting TIMER1 to control the door timing
	 * We will use  TIMER1 : F_CPU = 1 MHz , Prescaler = 1024
	 *                            Ftimer =  1 KHZ
	 *                            Ttimer = 1 milli second
	 *                            with compare value of 3000
	 *                            Ttimer = 3000 * 0.001 = 3 seconds
	 *                          1- To make the door opening time is 15 second we need 5 overflow
	 *                          2- To make the door holding time is 1 overflow (5+1) seconds we need 6 overflow
	 *                          3- To make the door closing time is (5+1+5) seconds we need 11 overflow
	 * */
	Timer1_ConfigType TimerDoor= {0,3000,F_CPU_1024,CTCMode};

	DcMotor_Rotate(Clockwise, 100);
	g_tick =0;
	Timer1_init(&TimerDoor);


	while(g_tick < 5);

	DcMotor_Rotate(Stop, 0);

	while(g_tick < 6);

	DcMotor_Rotate(AntiClockwise, 100);

	while(g_tick < 11);
	DcMotor_Rotate(Stop, 0);

	_delay_ms(1000);
	Timer1_deInit();
	g_tick = 0;

}
void CONTROL_timer1 (void){

	g_tick++;

}
uint8 CONTROL_handlingUnmatchedPasswords(void){

	/*Take Password*/
	CONTROL_receivePassword(password);
	CONTROL_checkPasswordinMemory(0x311, password);
	CONTROL_sendResponse(PasswordStatus);

	if(PasswordStatus == UNMATCHED_PASSWORDS){
		/*Give one more chance*/
		CONTROL_receivePassword(password);
		CONTROL_checkPasswordinMemory(0x311, password);
		CONTROL_sendResponse(PasswordStatus);

		if(PasswordStatus == UNMATCHED_PASSWORDS){
			/*Last One*/
			CONTROL_receivePassword(password);
			CONTROL_checkPasswordinMemory(0x311, password);
			CONTROL_sendResponse(PasswordStatus);

			return PasswordStatus;


		}
		else if (PasswordStatus == MATCHED_PASSWORDS){
			return PasswordStatus;
		}
	}

	else if(PasswordStatus == MATCHED_PASSWORDS){
		return PasswordStatus;
	}

	return PasswordStatus;

}
void CONTROL_lock(void){
	/* Structure for adjusting TIMER1 to control 1 minute lock
	 * We will use  TIMER1 : F_CPU = 1 MHz , Prescaler = 1024
	 *                            Ftimer =  1 KHZ
	 *                            Ttimer = 1 milli second
	 *                            with compare value of 30000
	 *                            Ttimer = 30000 * 0.001 = 30 seconds
	 * So we need 2 overflows
	 **/
	Timer1_ConfigType TimerError = {0,30000,F_CPU_1024,CTCMode};
	g_tick =0;
	/*Enable Buzzer for 1 minute*/
	Buzzer_on();

	Timer1_init(&TimerError);

	while(g_tick < 2);
	Timer1_deInit();
	Buzzer_off();
}
