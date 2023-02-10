//
// Created by figurantpp on 09/02/23.
//

#include <stdio.h>
#include "value.h"
#include "../common/alloc.h"
#include "../common/die.h"
#include <stdlib.h>


void display_list_in(LinkedList *list);

void display_list(LinkedList *list);

void display_value(Value *value);

void la1_display_value(Value *value) {

    display_value(value);

    printf("\n");

}

void display_value(Value *value) {
    switch (value->type) {

        case LA1_VALUE_LIST:
            display_list(value->content.list);
            break;

        case LA1_VALUE_NUMBER:
            printf("%ld", value->content.number);
            break;

        case LA1_VALUE_SYMBOL:
            printf("%s", (char *) value->content.symbol);
            break;

        case LA1_VALUE_CLOSURE:
            printf("<function>");
            break;

        default:

            fprintf(stderr, "Illegal parse value type %d\n", value->type);
            abort();
    }
}


Value *la1_list_into_value(LinkedList *value) {
    Value *result = la1_malloc(sizeof(*result));
    result->content.list = value;
    result->type = LA1_VALUE_LIST;
    return result;
}

Value *la1_number_into_value(long value) {
    Value *result = la1_malloc(sizeof(*result));
    result->content.number = value;
    result->type = LA1_VALUE_NUMBER;
    return result;
}

Value *la1_symbol_into_value(KnownSymbol value) {
    Value *result = la1_malloc(sizeof(*result));
    result->content.symbol = value;
    result->type = LA1_VALUE_SYMBOL;
    return result;
}

Value *la1_closure_into_value(Closure *closure) {
    Value *result = la1_malloc(sizeof(*result));
    result->content.closure = closure;
    result->type = LA1_VALUE_CLOSURE;
    return result;
}

Closure *la1_create_closure(ClosureFunction *function, void *extra) {

    Closure *result = la1_malloc(sizeof(*result));
    result->function = function;
    result->extra = extra;
    return result;
}

void la1_expect_type(Value *value, ValueType type) {

    if (value->type != type) {
        la1_die_format(
                "Expected type %s, got %s",
                la1_get_type_name(type),
                la1_get_type_name(value->type)
        );
    }

}

void la1_expect_size(LinkedList *list, unsigned int size) {

    if (la1_find_list_size(list) != size) {
        la1_die("Wrong number of arguments");
    }

}

const char *la1_get_type_name(ValueType type) {

    switch (type) {
#define X(name) case LA1_VALUE_##name: return #name;
        LA1_VALUE_TYPE_X()
#undef X
        default:
            la1_die("Unknown type name.");
    }
}


void display_list(LinkedList *list) {

    if (list == NULL) {
        printf("()");
    } else {
        printf("( ");
        display_value(list->content);
        printf(" ");
        display_list_in(list->next);
        printf(")");
    }

}

void display_list_in(LinkedList *list) {

    if (list != NULL) {
        display_value(list->content);

        printf(" ");
        display_list_in(list->next);
    }

}

