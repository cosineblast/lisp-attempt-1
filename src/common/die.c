//
// Created by figurantpp on 09/02/23.
//

#include "die.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void la1_die(const char *message) {
    fprintf(stderr, "error: %s\n", message);
    abort();
}

void __attribute__((noreturn)) la1_die_format(const char *format, ...) {
    va_list args;

    va_start(args, format);

    vfprintf(stderr, format, args);

    abort();
}

void __attribute__((noreturn))
la1_die_display(const char *message, Value *value) {
    fprintf(stderr, "error: %s ", message);

    la1_display_value_ln(value);

    fprintf(stderr, "\n");

    abort();
}
