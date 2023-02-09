//
// Created by figurantpp on 09/02/23.
//

#include <stdio.h>
#include "value.h"
#include "../common/alloc.h"
#include <stdlib.h>


void display_list_in(LinkedList *list);

void display_list(LinkedList *list);

void la1_display_value(Value *value) {

    switch (value->type) {

        case LA1_VALUE_LIST:
            display_list(value->content.list);
            break;

        case LA1_VALUE_NUMBER:
            printf("%ld", value->content.number);
            break;

        case LA1_VALUE_SYMBOL:
            printf("%s", value->content.symbol);
            break;

        default:
            fprintf(stderr, "Illegal parse value type %d\n", value->type);
            abort();
    }

}

Value *la1_list_into_value(LinkedList *value) {
    Value *result = la1_malloc(sizeof(*result)) ;
    result->content.list = value;
    result->type = LA1_VALUE_LIST;
    return result;
}

Value *la1_number_into_value(long value) {
    Value *result = la1_malloc(sizeof(*result)) ;
    result->content.number = value;
    result->type = LA1_VALUE_NUMBER;
    return result;
}

Value *la1_symbol_into_value(KnownSymbol value) {
    Value *result = la1_malloc(sizeof(*result)) ;
    result->content.symbol = value;
    result->type = LA1_VALUE_SYMBOL;
    return result;
}

void display_list(LinkedList *list) {

    if (list == NULL) {
        printf("()");
    } else {
        printf("( ");
        la1_display_value(list->content);
        printf(" ");
        display_list_in(list->next);
        printf(")");
    }

}

void display_list_in(LinkedList *list) {

    if (list != NULL) {
        la1_display_value(list->content);
        printf(" ");
        display_list_in(list->next);
    }

}

