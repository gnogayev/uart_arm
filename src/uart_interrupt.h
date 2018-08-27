/*
 * uart_interrupt.h
 *
 *  Created on: Aug 25, 2018
 *      Author: gnogayev
 */

#ifndef UART_INTERRUPT_H_
#define UART_INTERRUPT_H_

/**
 * For our use case, we find the length of the longest English word below:
 * https://en.wikipedia.org/wiki/Longest_word_in_English
 *
 * Let's use the longest word in a major dictionary, which is 45 characters
 * long and provide some extra space (x2).
 */
#define UART_MAX_BUF (45 * 2)

/**
 * UART buffer structure
 */
typedef struct {
    char buf[UART_MAX_BUF];
    unsigned int size;
} uart_buffer_t;

/**
 * Declare UART buffer
 */
extern uart_buffer_t uart_buffer;

/**
 * We can use only atomic operations inside the interrupt handler.
 * We assume this macro implements atomic set.
 */
#define ATOMIC_SET(X, Y) X = Y


#endif /* UART_INTERRUPT_H_ */
