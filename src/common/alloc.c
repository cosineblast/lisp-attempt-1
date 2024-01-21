//
// Created by cosineblast on 09/02/23.
//

#include "alloc.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void *la1_malloc(size_t size) {
    void *result = malloc(size);

    if (result == NULL) {
        fprintf(stderr, "Failed to allocate %zu bytes.\n", size);
        abort();
    }

    return result;
}

void *la1_realloc(void *buffer, size_t size) {
    void *result = realloc(buffer, size);

    if (result == NULL) {
        fprintf(stderr, "Failed to reallocate %zu bytes.\n", size);
        abort();
    }

    return result;
}
