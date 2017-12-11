#include <avr/pgmspace.h>
#include "hmi_msg.h"

static const char one[] PROGMEM = "One";
static const char two[] PROGMEM = "Two";
static const char thr[] PROGMEM = "Three";
static const char fou[] PROGMEM = "Four";
static const char fiv[] PROGMEM = "Five";
static const char six[] PROGMEM = "Six";
static const char sev[] PROGMEM = "Seven";
static const char eig[] PROGMEM = "Eight";
static const char nin[] PROGMEM = "Nine";

PGM_P const NUMBERS[] PROGMEM = {one, two, thr, fou, fiv, six, sev, eig, nin};
