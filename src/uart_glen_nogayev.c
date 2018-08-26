/*
 ============================================================================
 Name        : uart_glen_nogayev.c
 Author      :
 Version     :
 Copyright   : Glen Nogayev
 Description :
 ============================================================================
 */

#include <stdio.h> // putc, printf, getchar()
#include <stdlib.h> // putc, printf
#include <stdbool.h> // true

#include "french_translate.h" //  french_translate();

/**
 * We assume that CPU runs some posix compatible OS. E.g. QNX, Embedded linux.
 * So that we can write this code using standard posix API. If this code runs on other
 * custom real time os, we assume pthread_* function wrappers for standard syscalls.
 *
 * Though working code is not required, I find a good practice to prototipe new designs
 * on my Linux box.
 * The main function simulates way simplified OS idel task. The task responsible for
 * process, thread (if implemented on given OS) scheduling and putting the CPU to
 * power collapse. We assime that idel thread manages/ iterfaces components responsible
 * for all power modes and in our code we simply use go_to_pc - go to power collapse
 * function.
 */
int main(void) {
    /* misc initializations including configuring UART to IT (interrupt mode)
     * XXX - URL for the doc. Point to IT sect.
     *
     * Given the reqirments, and the critical power considerations,
     * our best options are IT and DMA URT modes. DMA has an advantage for
     * handling a large data blocks, however has difficulties with sleep in receiver
     * mode (see XXX, section XXX, DMA, cmobined mode stuff).
     * For the word translation use case, where received data sets are small - words (few bites,
     * tens of bytes max), and it is possible that the user could type in real time, meanning
     * tens, hundreds of ms between key presses, that is long enought to put CPU in lower
     * power state. We choose IT mode for UART receiver, meanning each new character will trigger
     * and interrupt. UART send mode could be any - polling, DMA.
     */

    // idel process infinite loop
    while (true) {

        /* 1. task/ process thread scheduling
         * in our case "french translate" process scheduling */
        french_translate();

        /* 2. we assume shceduler calls other concuring tasks as
         * by the given requirments "franch translate" is just a side process that should not
         * impact power consumption much
         */

        /* 3. idel process has no other tasks/ events to processs
         * all threads/ processes yelded
         * CPU can go to lower power state/ or power collapse
         */
        // go_to_pc()

        /* 4. this statement is just a simulation of the interrupt event
         * that wakes up the CPU.
         * For us to make it debuggable on linux, we read a caracter from stdin
         * and call dummy function that simulates a interrupt handler.
         * In this code we skip details of enabling an interrupt handler.
         */
        char rxed_from_uart = getchar();
        extern char uart_to_interrupt_dummy_interface;
        uart_to_interrupt_dummy_interface = rxed_from_uart;
        void uart_interrupt_handler(); // #4 is a hack, just a debug interface, therefore not in header
        uart_interrupt_handler();
        // printf(">>%c<<", rxed_from_uart);
    }

    return EXIT_SUCCESS;
    puts("Zalupa"); /* prints Zalupa */
}
