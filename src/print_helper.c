#include <stdio.h>
#include "print_helper.h"

int print_ascii_tbl(FILE *stream)
{
    for (int i = 33; i < 127; i++) {
        if (!fprintf(stream, "%c ", i)) {
            return 0;
        }
    }

    return fprintf(stream, "\n");
}


int print_for_human (FILE *stream, const unsigned char *array, const size_t len)
{
    if (len > 1) {
        for (unsigned int i = 0; i <= len; i++) {
            if (array[i] > 32 && array[i] < 127) {
                if (!fprintf(stream, "%c", array[i])) {
                    return 0;
                }
            } else {
                if (!fprintf(stream, "\"0x%02X\"", array[i])) {
                    return 0;
                }
            }
        }
    }

    return fprintf(stream, "\n");
}


