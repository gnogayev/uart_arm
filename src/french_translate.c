/*
 * french_translate.c
 *
 *  Created on: Aug 25, 2018
 *      Author: gnogayev
 */

#include <stdio.h> // putc, printf, getchar
#include <string.h> // strncmp, memchr
#include "french_translate.h"
#include "uart_interrupt.h" // uart_buffer
#include "uart_stub.h" // uart_tx_string

#define ENGLISH          0
#define FRENCH           1
#define NUM_LANG         2
#define DICTIONARY_SIZE  10

/**
 * Simplified version of English to French dictionary
 */
static char *dictionary[NUM_LANG][DICTIONARY_SIZE] =
{
        { "Hello", "Goodbye", "Thank you", "Man", "Woman", "Boy", "Girl", "House", "Car", "Work" },
        { "Bonjour", "Au revoir", "Merci", "Homme", "Femme", "Garcon", "Fille", "Maison", "Voiture", "Travail" }
};

/**
 * Helper function to do the translation
 * 1. Check lookup table for English word
 * 2. If word exists, return French translation
 */
char *translate_to_french(char const * const string, int size) {
    char *string_out = NULL;
    // Search for word translation in the dictionary
    for (int i = 0; i < DICTIONARY_SIZE; i++) {
        if (strncmp(string,  dictionary[ENGLISH][i], size) == 0) {
            //Check that words are same size
            //strlen is sufficient because strings are static
            if (size == strlen(dictionary[ENGLISH][i])){
                string_out = dictionary[FRENCH][i];
                break;
            }
        }
    }
    return string_out;
}

/**
 * Translation function
 *
 * This function is called from the while loop of the idle task
 */
void french_translate() {

    /* NOTE: it is possible that the buffer will move forward after we check for CR
     * character. As this function will clear the buffer, these new characters may be
     * lost. Therefore, we make an important assumption that the user will not type a
     * new word until the response for the current word is received.
     * As a future optimization, a more complicated ping-pong buffer implementation
     * is preferred, in order to avoid data loss.
     */

    // 1. Check if UART buffer has CR character
    if (NULL == memchr(uart_buffer.buf, '\n', uart_buffer.size)) {
        return;
    }

    // 2. Copy the full word to local buffer
    uart_buffer_t local = uart_buffer;

    // 3 Reset UART buffer
    ATOMIC_SET(uart_buffer.size, 0);

    // 4. Translate word using local buffer copy
    char *buffer_out = translate_to_french(local.buf, local.size - 1 /*do not pass the CR character*/);

    // 5. Write translation (if exists) to UART
    if (buffer_out) {
        // Send translated word to UART
        int size = strlen(buffer_out);
        uart_tx_string(buffer_out, size);
    }

}
