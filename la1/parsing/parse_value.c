//
// Created by figurantpp on 09/02/23.
//
#include "parse_value.h"
#include "../common/alloc.h"

#include <stdio.h>
#include <stdlib.h>

void dump_list(LinkedList *p_list);

ParseValue *la1_list_into_parse_value(LinkedList *list) {

    ParseValue *value = la1_malloc(sizeof(*value));
    value->type = PARSE_VALUE_LIST;
    value->content.list = list;
    return value;
}

ParseValue *la1_number_into_parse_value(long number) {

    ParseValue *value = la1_malloc(sizeof(*value));
    value->type = PARSE_VALUE_NUMBER;
    value->content.number = number;
    return value;
}

ParseValue *la1_symbol_into_parse_value(char *symbol) {

    ParseValue *value = la1_malloc(sizeof(*value));
    value->type = PARSE_VALUE_SYMBOL;
    value->content.symbol = symbol;
    return value;
}

void la1_dump_parse_value(ParseValue *value) {

    switch (value->type) {

        case PARSE_VALUE_LIST:
            dump_list(value->content.list);
            break;

        case PARSE_VALUE_NUMBER:
            printf("%ld", value->content.number);
            break;

        case PARSE_VALUE_SYMBOL:
            printf("%s", value->content.symbol);
            break;

        default:
            fprintf(stderr, "Illegal parse value type %d\n", value->type);
            abort();
    }

}

void dump_list(LinkedList *list) {

    if (list == NULL) {
        printf("()");
    }
    else {
        printf("( ");
        la1_dump_parse_value(list->content);
        printf(" . ");
        dump_list(list->next);
        printf(" )");
    }

}

