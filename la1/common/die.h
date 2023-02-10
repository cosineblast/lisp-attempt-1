//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_DIE_H
#define LATTEMPT_DIE_H

#include "../execution/value.h"

void __attribute__((noreturn)) la1_die(const char *message);

void __attribute__((noreturn)) la1_die_format(const char *format, ...);

void __attribute__((noreturn)) la1_die_display(const char *message, Value *value);

#endif //LATTEMPT_DIE_H
