/*
 * uart_interrupt.c
 *
 *  Created on: Aug 25, 2018
 *      Author: gnogayev
 */

#include "uart_interrupt.h"

/* below global variable is just for simulation purpose,
 * to pass "the character read from UART" to our interrupt handler
 */
char uart_to_interrupt_dummy_interface;

/*
 * Define uart buffer.
 */
uart_buffer_t uart_buffer = { { 0x0 }, 0x0 };

/**
 * This function implements an interrupt handler from UART
 * As we run on arm, we assume that XXX - mutex function implemented
 * using ARM atomic operation allowing SMP synchronisation.
 */
void uart_interrupt_handler() {
    // 1. regular interrupt start stuff (IMB, EMB), see APM interrupt handler doc's

    // our debug interface - to uart
    char volatile rec = uart_to_interrupt_dummy_interface;

    /* 2. add character to a uart buffer, ideally we need to implement a FIFO queue,
     * however for now we'll leave it as a future optimization.
     * For now we'll cover buffer overun case in simplified way -
     * in case the whole large uart interrupt buffer is filled, we'll consider this as
     * an error and reset/ clear it entirelly.
     */
    if (uart_buffer.size == UART_MAX_BUF) {
        // error case, reset
        ATOMIC_SET(uart_buffer.size, 0);
        goto finish;
    }

    // the buffer is not changed until we change it's size
    uart_buffer.buf[uart_buffer.size] = rec;
    ATOMIC_SET(uart_buffer.size, uart_buffer.size + 1); // we should use atomic increment here

    finish:
    // regular interrup finilization, see ARM XXXXX
    return;
}
