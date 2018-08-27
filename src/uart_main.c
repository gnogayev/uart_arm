/*
 ============================================================================
 Name        : uart_main.c
 Author      :
 Version     :
 Copyright   : Glen Nogayev
 Description :
 ============================================================================
 */

#include <stdio.h> // putc, printf, getchar
#include <stdlib.h> // putc, printf
#include <stdbool.h> // boolean

#include "french_translate.h" // french_translate()
#include "uart_stub.h" // uart_rx_char

/**
 * The main function simulates a simplified OS idle task. This task is responsible
 * for process/thread (if implemented on given OS) scheduling and putting the CPU to
 * power collapse. We assume that idle thread manages/interfaces to/with components
 * responsible for all power modes and in our code we simply use go_to_pc - go to
 * power collapse function.
 *
 * Though working code is not required, I find it good practice to prototype new
 * designs in my Linux environment.
 */
int main(void) {
    /* Misc initializations including configuring UART to IT (interrupt mode)
     * Refer to below doc:
     * Application note AN4635, Minimization of power consumption using LPUART in
     * micro-controllers of the STM32L0 STM32L4 series, section 3.2
     *
     * Given the requirements and the critical power considerations, our best options
     * are IT and DMA with direct access modes. DMA has an advantage for handling
     * large data blocks, however has difficulty picking up ongoing reception after
     * wake-up (see Application note AN4635, Section 3.4.2).
     * For the word translation use case, received data sets are small: words of few
     * bytes, tens of bytes max. Also, it is possible that the user could type in
     * real time, meaning tens/hundreds of ms between key presses, which should be
     * long enough to put CPU to lower power state. We choose IT mode for UART
     * receiver, meaning each new character will trigger an interrupt. UART send mode
     * could be any - we assume polling.
     */

    // Idle process infinite loop
    while (true) {

        /* 1. Task/process thread scheduling
         * In our case "french translate" process scheduling */
        french_translate();

        /* 2. We assume scheduler calls other concurring tasks as per the given
         * requirements that "french translate" is one of the processes in the system.
         */

        /* 3. Idle process has no other tasks/events to process.
         * CPU can go to lower power state/ or power collapse.
         */
        // go_to_pc()

        /* 4. This statement is just a simulation of the interrupt event that wakes up
         * the CPU. For us to make it debug-able on Linux, we read a character from
         * stdin and call dummy function that simulates an interrupt handler.
         * In this code we skip details of enabling an interrupt handler.
         */
        char rxed_from_uart = uart_rx_char();

        extern char uart_to_interrupt_dummy_interface;
        uart_to_interrupt_dummy_interface = rxed_from_uart;
        void uart_interrupt_handler(); // #4 is a hack, just a debug interface, therefore not in header
        uart_interrupt_handler();
    }

    return EXIT_SUCCESS;
}
