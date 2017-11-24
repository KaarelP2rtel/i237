#include <avr/pgmspace.h>
#ifndef _HMI_MSG_H_
#define _HMI_MSG_H_

const char PROG_VER[] PROGMEM = "\nVersion: %s built on: %s %s";
const char LIBC_GCC_VER[] PROGMEM =
    "\navr-libc version: %s avr-gcc version: %s";
const char STUDENT[] PROGMEM = "Kaarel Partel";
const char ENTER[] PROGMEM = "\nEnter a number>";
const char WRONG[] PROGMEM = "\nPlease enter a number between 0 and 9";
const char ENTERED[] PROGMEM = "\nYou entered: ";
const char NUMBERS[10][6] PROGMEM = {"Zero", "One", "Two", "Three",
                                     "Four", "Five", "Six", "Seven",
                                     "Eight", "Nine"
                                    };
#endif /* _HMI_MSG_H_ */
