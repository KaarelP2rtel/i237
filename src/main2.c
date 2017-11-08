#include <stdio.h>
#define __ASSERT_USE_STDERR
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#define BLINK_DELAY_MS 100


static inline void init_leds(void)
{
    /* set pin 7 of PORTB for output*/
    DDRB |= _BV(DDB7);
    /* set pins 0,2 and 4 of PORTA for output*/
    DDRA |= _BV(DDB0);
    DDRA |= _BV(DDB2);
    DDRA |= _BV(DDB4);
    /*Turn off onboard LED*/
    PORTB &= ~_BV(PORTB7);
}


/* Init error console as stderr in UART1 and print user code info */
static inline void init_errcon(void)
{
    simple_uart1_init();
    stderr = &simple_uart1_out;
    fprintf(stderr, "Version: %s built on: %s %s\n",
            FW_VERSION, __DATE__, __TIME__);
    fprintf(stderr, "avr-libc version: %s avr-gcc version: %s\n",
            __AVR_LIBC_VERSION_STRING__, __VERSION__);
}


static inline void blink_leds(void)
{
    /* Turn off Blue LED, turn on red LED */
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~_BV(PORTB4);
    PORTA |= _BV(PORTB0);
    /* Turn off Red LED, turn on Green LED */
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~_BV(PORTB0);
    PORTA |= _BV(PORTB2);
    /* Turn off Green LED, turn on Blue LED */
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~_BV(PORTB2);
    PORTA |= _BV(PORTB4);
}


void main(void)
{
    init_leds();
    init_errcon();
    /* Test assert - REMOVE IN FUTURE LABS */
    char *array;
    uint32_t i = 1;
    extern int __heap_start, *__brkval;
    int v;
    array = malloc( i * sizeof(char));
    assert(array);
    /* End test assert */

    while (1) {
        blink_leds();
        /* Test assert - REMOVE IN FUTURE LABS */
        /*
         * Increase memory allocated for array by 100 chars
         * until we have eaten it all and print space between stack and heap.
         * That is how assert works in run-time.
         */
        array = realloc( array, (i++ * 100) * sizeof(char));
        fprintf(stderr, "%d\n",
                (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
        assert(array);
        /* End test assert */
    }
}
