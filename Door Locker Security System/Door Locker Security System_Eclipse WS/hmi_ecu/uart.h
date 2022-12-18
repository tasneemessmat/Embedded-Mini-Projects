 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: TASNEEM ESSMAT
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum {
	Disabled, Reserved, EvenParity, OddParity
}Uart_ParityMode;

typedef enum{
	OneBit, TwoBits
}Uart_StopBits;
typedef enum{
	FiveBits, SixBits , SevenBits, EightBits	//NineBits = 7 //will not support this mode
												//as it's not standard
}Uart_DataBits;
typedef struct
{
	uint32 Uart_BaudRate;
	Uart_ParityMode Parity_Sel;
	Uart_StopBits StopBit_Sel;
	Uart_DataBits DataBits_Sel;

}Uart_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(Uart_ConfigType * UartConfig);


/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
