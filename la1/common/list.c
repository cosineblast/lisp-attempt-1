//
// Created by figurantpp on 09/02/23.
//

#include "list.h"
#include "alloc.h"

LinkedList *la1_cons(void *content, LinkedList *next) {

    LinkedList *result = la1_malloc(sizeof(*result));
    result->content = content;
    result->next = next;
    return result;
}

LinkedList *la1_empty_list() {
    return NULL;
}
