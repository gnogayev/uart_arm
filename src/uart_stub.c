/*
 * uart_stub.c
 *
 *  Created on: Aug 27, 2018
 *      Author: gnogayev
 */

#include <stdio.h> // putc, printf, getchar
#include "uart_stub.h"

/**
 * Function to receive new character from UART
 */
char uart_rx_char(void) {
    char data = getchar();
    return data;
}

/**
 * Function to send new character to UART
 */
void uart_tx_char(char data) {
    // Write to UART port
}

/**
 * Function to send string to UART
 */
void uart_tx_string(char *string, int size) {
    int i = 0;
    while(i <= size) {
        uart_tx_char(string[i]);
        i++;
    }
    printf("%s\n", string);
}
