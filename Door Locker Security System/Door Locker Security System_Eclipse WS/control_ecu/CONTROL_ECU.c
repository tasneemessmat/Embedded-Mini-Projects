/*
 * Control_ECU.c
 *
 *  Created on: Oct 25, 2022
 *
 *      Author: TASNEEM ESSMAT
 *
 * Description:	CONTROL_ECU Code in Door Locker Security System.
 * 				CONTROL_ECU is responsible for all the processing and decisions in the system
 * 				 like password checking, open the door and activate the system alarm.
 */


#include "uart.h"
#include <util/delay.h>
#include "control_services.h"



int main(void)
{

	uint8 key;
	/* Initialize Module */
	CONTROL_init();
	CONTROL_createPassword_changePassword();



	while(1)
	{
		/*First Get key pressed from HMI ecu*/
		while(UART_recieveByte() != HMI_ECU_READY);
		UART_sendByte(CONTROL_ECU_READY);
		key = UART_recieveByte();
		_delay_ms(1000);

		if (key == '+'){
			/*Get password to check it with the one saved in memory*/
			CONTROL_receivePassword(password);
			PasswordStatus = CONTROL_checkPasswordinMemory(0x0311, password);
			CONTROL_sendResponse(PasswordStatus);

			if (PasswordStatus == MATCHED_PASSWORDS){
				/*Now Open the door*/
				CONTROL_openDoor();
			}
			else if (PasswordStatus == UNMATCHED_PASSWORDS){

				PasswordStatus = CONTROL_handlingUnmatchedPasswords();


				if(PasswordStatus == MATCHED_PASSWORDS){
					CONTROL_openDoor();
				}
				else{
					CONTROL_lock();
				}
			}
		}
		else if(key == '-')
		{
			/*Get password to check it with the one saved in memory*/
			CONTROL_receivePassword(password);
			CONTROL_checkPasswordinMemory(0x311, password);
			CONTROL_sendResponse(PasswordStatus);

			if(PasswordStatus == MATCHED_PASSWORDS){
				CONTROL_createPassword_changePassword();
			}
			else if(PasswordStatus == UNMATCHED_PASSWORDS){
				PasswordStatus = CONTROL_handlingUnmatchedPasswords();

				if(PasswordStatus == MATCHED_PASSWORDS){
					CONTROL_createPassword_changePassword();
				}
				else if(PasswordStatus == UNMATCHED_PASSWORDS) {
					CONTROL_lock();
				}
			}

		}


	}
}

