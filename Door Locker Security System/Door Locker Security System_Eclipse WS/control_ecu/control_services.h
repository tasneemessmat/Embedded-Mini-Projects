/*
 * controlservices.h
 *
 *  Created on: Nov 4, 2022
 *      Author: TASNEEM
 */

#ifndef CONTROL_SERVICES_H_
#define CONTROL_SERVICES_H_

#include "std_types.h"

#define CONTROL_ECU_READY 0x3
#define HMI_ECU_READY 0x4

#define UNMATCHED_PASSWORDS 0x05
#define MATCHED_PASSWORDS 0x06

/*Variables for storing password*/
extern uint8 password[6];
extern uint8 ConfirmPassword[6];
extern uint8 PasswordStatus;
extern uint32 g_tick;


void CONTROL_init(void);
void CONTROL_receivePassword(uint8 * password);
void CONTROL_savePasswordsEEPROM(uint16 address ,uint8 * password);
uint8 CONTROL_checkPasswordinMemory(uint16 address, uint8 * password );
void CONTROL_sendResponse (uint8  PasswordStatus);
void CONTROL_timer1 (void);
void CONTROL_openDoor(void);
uint8 CONTROL_handlingUnmatchedPasswords(void);
void CONTROL_createPassword_changePassword(void);
void CONTROL_lock(void);


#endif /* CONTROL_SERVICES_H_ */
