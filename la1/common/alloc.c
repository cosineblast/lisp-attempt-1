//
// Created by figurantpp on 09/02/23.
//

#include "alloc.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void *la1_malloc(size_t size) {

    void *result = malloc(size);

    if (result == NULL) {
        fprintf(stderr, "Failed to allocate %zu bytes.\n", size);
        abort();
    }

    return result;
}
