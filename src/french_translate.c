/*
 * french_translate.c
 *
 *  Created on: Aug 25, 2018
 *      Author: gnogayev
 */

#include "french_translate.h"
#include <stdio.h>
#include <string.h> // strchr
#include "uart_interrupt.h" // uart_buffer

#define ENGLISH          0
#define FRENCH           1
#define NUM_LANG         2
#define DICTIONARY_SIZE  5

static char *dictionary[NUM_LANG][DICTIONARY_SIZE] =
{
        { "Hello", "Goodbye","Thank you", "Man", "Woman" },
        { "Bonjour", "Au revoir", "Merci", "Homme", "Femme" }
};

char *translate_to_french(char const * const string, int size) {
    char *string_out = NULL;
    // Search for word translation in the dictionary
    for (int i = 0; i < DICTIONARY_SIZE; i++) {
        if (strncmp( string,  dictionary[ENGLISH][i], size) == 0) {
            string_out = dictionary[FRENCH][i];
            break;
        }
    }
    return string_out;
}

/**
 * Translation function
 */
void french_translate() {

    /* Here we make an important assumption/ limitation,
     * It is possible that the buffer will move forward after
     * we chack for CR charcter. As this function clear the buffer, these
     * new characters may be lost.
     * In our implementation
     * we assume that the user should wait untill he see's the response, therefore
     * this situation avoided.
     * In real life we'll have to use a bit more complicated ping pong buffer
     * implementation, so no data is lost
     */

    // DBG
    printf("dbg - %s\n", uart_buffer.buf);

    // 1. check if the buffer has CR character
    if (NULL == strchr(uart_buffer.buf, '\n')) {
        // dbg
        printf("nope\n");

        return;
    }

    // 2. we've got the word, copy the buffer
    uart_buffer_t local = uart_buffer;

    // dbg
    printf("GREAT SUCCESS %s - %i\n", local.buf, local.size);

    // 3 and reset it
    ATOMIC_SET(uart_buffer.size, 0);

    // 3. translate what we've got using the local copy
    char *buffer_out = translate_to_french(local.buf, local.size - 1 /*do not pass the CR character*/);

    // 4. write translation (if exists) to uart
    if (buffer_out) {
        // WRITE TO UART Here
        printf("OK - %s\n", buffer_out);
    }

}
