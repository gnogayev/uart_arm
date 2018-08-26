/*
 * uart_interrupt.h
 *
 *  Created on: Aug 25, 2018
 *      Author: gnogayev
 */

#ifndef UART_INTERRUPT_H_
#define UART_INTERRUPT_H_

/**
 * For our use case lets assume the length of the longest English word:
 * https://en.wikipedia.org/wiki/Longest_word_in_English
 * Let's use the longest in a major dictionary that is 45 carcters.
 * Let's have some spare space too (x2).
 */
#define UART_MAX_BUF (45 *2)

/**
 * The buffer structure
 */
typedef struct {
    char buf[UART_MAX_BUF];
    unsigned int size;
} uart_buffer_t;

/**
 * Delcare uart buffer
 */
extern uart_buffer_t uart_buffer;

/**
 * We can use only atocmic operations insede the interrupt handler
 * We assume this macro implements atomic set
 */
#define ATOMIC_SET(X, Y) X = Y


#endif /* UART_INTERRUPT_H_ */
