#include <avr/pgmspace.h>
#include <stdio.h>
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <time.h>
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/helius_microrl/microrl.h"
#include "cli_microrl.h"
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "rfid.h"
#include <string.h>

#define LED PORTA2 // Arduino Mega digital pin 24
#define LED_DOOR PORTA4
#define BLINK_DELAY_MS 100
#define UART_BAUD 9600
#define UART_STATUS_MASK 0x00FF

//Create microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;



static inline void start_cli(void)
{
    uart0_puts_p(PSTR("Use backspace to delete entry and enter to confirm.\r\n"));
    microrl_init(prl, uart0_puts);
    microrl_set_execute_callback(prl, cli_execute);
}

static inline void init_sys_timer(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12); // Turn on CTC (Clear Timer on Compare)
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 62549; // Note that it is actually two registers OCR5AH and OCR5AL
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}


static inline void init_uart1(void)
{
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart1_puts_p(PSTR("\r\nError Console started\r\n"));
    uart1_puts_p(PSTR(PROG_VERSION));
}

static inline void init_uart0(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart0_puts_p(PSTR("\r\nConsole started\r\n"));
    uart0_puts(STUDENT);
    uart0_puts_p(PSTR("\r\n"));
}

static inline void init_leds(void)
{
    DDRA |= _BV(DDB2);
    DDRA |= _BV(DDB4);
}


static inline void heartbeat(void)
{
    static time_t prev_time;
    char ascii_buf[11] = {0x00};
    time_t now = time(NULL);

    if (prev_time != now) {
        //Print uptime to uart1
        ltoa(now, ascii_buf, 10);
        uart1_puts_p(PSTR("Uptime: "));
        uart1_puts(ascii_buf);
        uart1_puts_p(PSTR(" seconds\r"));
        //Toggle LED
        PORTA ^= _BV(LED);
        prev_time = now;
    }
}


time_t door_changed = -2;
time_t lastseen;
char lastuid[20];
char uid_string[20];
bool statusis0 = false;
void update_status(int status, char *user)
{
    if (status == 0) {
        lcd_clrscr();
        lcd_puts(STUDENT);
        lcd_goto(0x40);
        lcd_puts("Closed");
        PORTA &= ~_BV(LED_DOOR);
        statusis0 = true;
    } else if (status == 1) {
        lcd_clrscr();
        lcd_puts(STUDENT);
        lcd_goto(0x40);
        lcd_puts("Open   ");
        lcd_puts(user);
        statusis0 = false;
        PORTA |= _BV(LED_DOOR);
        ;
    } else {
        lcd_clrscr();
        lcd_puts(STUDENT);
        lcd_goto(0x40);
        lcd_puts("Closed AxsDenied");
        PORTA &= ~_BV(LED_DOOR);
        statusis0 = false;
    }
}


void handle_door()
{
    Uid uid;
    Uid *uid_ptr = &uid;

    if (PICC_IsNewCardPresent()) {
        strcpy(uid_string, "");
        PICC_ReadCardSerial(uid_ptr);

        for (byte i = 0; i < uid.size; i++) {
            char suid[20];
            itoa(uid.uidByte[i], suid, 10);
            strcat(uid_string, suid);
        }

        lastseen = time(NULL);
    }

    if (time(NULL) - lastseen > 1) {
        strcpy(lastuid, "");
        strcpy(uid_string, "");
    }

    if (strcmp(lastuid, uid_string) != 0) {
        char * user = find(uid_string);

        if (user == NULL) {
            update_status(2, NULL);
            door_changed = time(NULL);
        } else {
            update_status(1, user);
            door_changed = time(NULL);
        }

        strcpy(lastuid, uid_string);
    }

    if (time(NULL) - door_changed >= 2) {
        if (!statusis0) {
            update_status(0, NULL);
        }
    }
}


void main(void)
{
    //Initialise consoles, LEDs, LCD, timer and CLI.
    init_uart0();
    init_uart1();
    init_leds();
    lcd_init();
    lcd_clrscr();
    lcd_puts(STUDENT);
    init_sys_timer();
    start_cli();
    MFRC522_init();
    PCD_Init();
    sei();

    while (1) {
        heartbeat();
        microrl_insert_char (prl, uart0_getc() & UART_STATUS_MASK);
        handle_door();
    }
}
ISR(TIMER1_COMPA_vect)
{
    system_tick();
}
