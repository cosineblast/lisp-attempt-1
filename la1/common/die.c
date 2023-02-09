//
// Created by figurantpp on 09/02/23.
//

#include "die.h"

#include <stdlib.h>
#include <stdio.h>

void die(const char *message) {
    fprintf(stderr, "error: %s\n", message);
    abort();

}
