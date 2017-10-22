#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500

int main (void)
{
    /* set pin 7 of PORTB for output*/
    DDRB |= _BV(DDB7);
    /* set pins 0,2 and 4 of PORTA for output*/
    DDRA |= _BV(DDB0);
    DDRA |= _BV(DDB2);
    DDRA |= _BV(DDB4);
    /*Turn off onboard LED*/
    PORTB &= ~_BV(PORTB7);

    while (1) {
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
}
