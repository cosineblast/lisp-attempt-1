//
// Created by cosineblast on 09/02/23.
//

#ifndef LATTEMPT_PARSE_VALUE_H
#define LATTEMPT_PARSE_VALUE_H

#include "../common/list.h"

enum ParseValueType {
    PARSE_VALUE_LIST,
    PARSE_VALUE_NUMBER,
    PARSE_VALUE_SYMBOL,
};

typedef enum ParseValueType ParseValueType;

union UntaggedParseValue {
    LinkedList *list;
    long number;
    char *symbol;
};

typedef union UntaggedParseValue UntaggedParseValue;

struct ParseValue {
    ParseValueType type;
    UntaggedParseValue content;
};

typedef struct ParseValue ParseValue;

ParseValue *la1_list_into_parse_value(LinkedList *list);

ParseValue *la1_number_into_parse_value(long number);

ParseValue *la1_symbol_into_parse_value(char *symbol);

void la1_display_parse_value(ParseValue *value);

void la1_dump_parse_value(ParseValue *value);

void la1_free_parse_value(ParseValue *value);

#endif  // LATTEMPT_PARSE_VALUE_H
