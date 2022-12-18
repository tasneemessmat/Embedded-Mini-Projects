/*
 * HMI_Services.h
 *
 *  Created on: Nov 3, 2022
 *      Author: TASNEEM
 */

#ifndef HMI_SERVICES_H_
#define HMI_SERVICES_H_

#define CONTROL_ECU_READY 0x3
#define HMI_ECU_READY 0x4


#define UNMATCHED_PASSWORDS 0x05
#define MATCHED_PASSWORDS 0x06

/*Variables for storing password*/
extern uint8 password [6];
extern uint8 ConfirmPassword [6];
extern uint8 Password_status;

extern uint8 g_tick ;


void HMI_init(void);
void HMI_writePassword(uint8 * arrPass);
void HMI_sendPassword(uint8 * arrPass);
void HMI_waitResponse(uint8 * Password_status);
void HMI_openDoor(void);
void HMI_timer1(void);
uint8 HMI_handlingUnmatchedPasswords(void);
void HMI_lock(void);
void HMI_createPassword_changePassword(void);


#endif /* HMI_SERVICES_H_ */
