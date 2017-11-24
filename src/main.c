#include <avr/pgmspace.h>
#include <stdio.h>
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
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
static inline void init_con(void)
{
    simple_uart0_init();
    stdin = stdout = &simple_uart0_io;
    fprintf_P(stdout, PROG_VER, FW_VERSION, __DATE__, __TIME__);
    fprintf_P(stdout, LIBC_GCC_VER, __AVR_LIBC_VERSION_STRING__, __VERSION__);
    fprintf(stdout, "\n");
    fprintf_P(stdout, STUDENT);
    fprintf(stdout, "\n");
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
    //Initialise LEDs, console and LCD
    init_leds();
    init_con();
    lcd_init();
    lcd_clrscr();
    //Writes students name on the display
    lcd_puts_P(STUDENT);
    //ASCII table values
    unsigned char array[128] = {0};

    for (unsigned char i = 0; i < sizeof(array); i++) {
        array[i] = i;
    }

    //Print ASCII table and Print ASCII for humans using the array.
    fprintf(stdout, "\n");
    print_ascii_tbl(stdout);
    fprintf(stdout, "\n");
    print_for_human(stdout, array, sizeof(array) - 1);

    while (1) {
        //Prints text, reads input and prints input
        char input[20];
        fprintf_P(stdout, ENTER);
        scanf("%s", input);
        fprintf(stdout, input);
        //Converts input into Integer (Warning, ignores non digit characters)
        int in_int = atoi(input);

        //If input is between 0 and 9 prints integer as text from table
        if (in_int >= 0 && in_int < 10) {
            fprintf_P(stdout, ENTERED);
            fprintf_P(stdout, NUMBERS[in_int]);
        } else {
            fprintf_P(stdout, WRONG);
        }

        //Blink LEDs
        blink_leds();
    }
}
