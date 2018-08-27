/*
 * uart_stub.h
 *
 *  Created on: Aug 27, 2018
 *      Author: gnogayev
 */

#ifndef UART_STUB_H_
#define UART_STUB_H_

// Function prototypes

/**
 * Function to receive new character from UART
 */
char uart_rx_char(void);

/**
 * Function to send new character to UART
 */
void uart_tx_char(char data);

/**
 * Function to send string to UART
 */
void uart_tx_string(char *string, int size);

#endif /* UART_STUB_H_ */
