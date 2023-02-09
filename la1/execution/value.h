//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_VALUE_H
#define LATTEMPT_VALUE_H

#include "../common/list.h"
#include "binding.h"

struct Value;

typedef struct Value Value;





struct Function {
    Bindings environment;
    Bindings parameters;
    LinkedList *body;
};

typedef struct Function Function;

enum ValueType {
    LA1_VALUE_NUMBER,
    LA1_VALUE_LIST,
    LA1_VALUE_SYMBOL,
    LA1_VALUE_FUNCTION,
};

typedef enum ValueType ValueType;

union UntaggedValue {
    LinkedList *list;
    KnownSymbol symbol;
    long number;
    Function *function;
};

typedef union UntaggedValue UntaggedValue;

struct Value {
    ValueType type;
    UntaggedValue content;
};

Value* la1_list_into_value(LinkedList *value);
Value* la1_number_into_value(long value);
Value* la1_symbol_into_value(KnownSymbol symbol);

void la1_display_value(Value *value);


#endif //LATTEMPT_VALUE_H
