/*
 * HMI_ECU.c
 *
 *  Created on: Oct 25, 2022
 *
 *      Author: TASNEEM ESSMAT
 *
 * Description:	Human Machine Interface Code in Door Locker Security System.
 * 				HMI is responsible for interaction with the user: take inputs through keypad
 * 				and display messages on the LCD.
 */

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "HMI_Services.h"
#include <util/delay.h>





int main(void)
{
	uint8 key;
	/* Initialize Module */
	HMI_init();
	LCD_displayString("Hi There !");
	_delay_ms(1000);

	HMI_createPassword_changePassword();

while(1)
{
	/* Display Main System */
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_moveCursor(1, 0);
	LCD_displayString("- : Change Pass");
	/******************************************/

	/* Now get an option from user */
	key = KEYPAD_getPressedKey();
	_delay_ms(500); /* Press time */

	/*Send The required key for control ecu */
	UART_sendByte(HMI_ECU_READY);
	while(UART_recieveByte() != CONTROL_ECU_READY);
	UART_sendByte(key);
	/******************************************/


	if (key == '+'){
		for(int i=0;i<5;i++){
			password[i] ='2';
		}
//		LCD_clearScreen();
//		/*get password from user */
//		LCD_displayString("Please Enter Password: ");
//		HMI_writePassword(password);
	HMI_sendPassword(password);
//		/*wait to see if it matched the one saved or not*/
	HMI_waitResponse(&Password_status);
		/******************************************/

		if (Password_status == MATCHED_PASSWORDS){
			LCD_clearScreen();
			LCD_displayString("Valid Password :)");
			_delay_ms(500);
			/*Now open the door*/
			HMI_openDoor();
		}
		else if(Password_status == UNMATCHED_PASSWORDS){
			LCD_clearScreen();
			LCD_displayString("Invalid Password!");
			_delay_ms(500);

			Password_status = HMI_handlingUnmatchedPasswords();

			if(Password_status == MATCHED_PASSWORDS){
				/*Now open the door*/
				HMI_openDoor();
			}
			else {

				HMI_lock();
			}

		}


	}
	else if (key == '-'){

		for(int i=0;i<5;i++){
			password[i]='9';
		}
//		LCD_clearScreen();
//		/*get password from user */
//		LCD_displayString("Please Enter Password: ");
//		HMI_writePassword(password);
		HMI_sendPassword(password);
		/*wait to see if it matched the one saved or not*/
		HMI_waitResponse(&Password_status);

		if(Password_status == MATCHED_PASSWORDS){
			LCD_clearScreen();
			LCD_displayString("Valid Password :)");
			HMI_createPassword_changePassword();

		}
		if(Password_status == UNMATCHED_PASSWORDS){
			LCD_clearScreen();
			LCD_displayString("Invalid Password!");
			_delay_ms(500);

			Password_status = HMI_handlingUnmatchedPasswords();

			if(Password_status == MATCHED_PASSWORDS){
				HMI_createPassword_changePassword();
			}
			else {
				HMI_lock();

			}

		}
	}


}
}
