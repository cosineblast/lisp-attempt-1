//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_ALLOC_H
#define LATTEMPT_ALLOC_H

#include <stddef.h>

/**
 * Allocates \p size bytes or aborts execution.
 * @param size
 * @return
 */
void *la1_malloc(size_t size);

void *la1_realloc(void *, size_t size);

#endif  // LATTEMPT_ALLOC_H
