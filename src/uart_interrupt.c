/*
 * uart_interrupt.c
 *
 *  Created on: Aug 25, 2018
 *      Author: gnogayev
 */

#include "uart_interrupt.h"

/* Below global variable is just for simulation purpose. It is used
 * to pass "the character read from UART" to our interrupt handler.
 */
char uart_to_interrupt_dummy_interface;

/*
 * Define UART buffer
 */
uart_buffer_t uart_buffer = { { 0x0 }, 0x0 };

/**
 * This function implements an interrupt handler from UART
 *
 * As we are running on ARM, we assume that we are using ARM atomic operation
 * allowing SMP synchronization.
 */
void uart_interrupt_handler() {
    // 1. Standard interrupt start stuff

    // Our debug interface to UART
    char volatile recv = uart_to_interrupt_dummy_interface;

    /* 2. Add character to UART buffer.
     * Buffer overrun case is covered in simplified way - in case the whole UART
     * interrupt buffer is filled, consider this as an error and clear it entirely.
     */
    if (uart_buffer.size == UART_MAX_BUF) {
        // error case, reset
        ATOMIC_SET(uart_buffer.size, 0);
        goto finish;
    }

    // Buffer is not changed until we change its size
    uart_buffer.buf[uart_buffer.size] = recv;
    // We should use atomic increment here
    ATOMIC_SET(uart_buffer.size, uart_buffer.size + 1);

    finish:
    // Standard interrupt finalization
    return;
}
