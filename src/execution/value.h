//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_VALUE_H
#define LATTEMPT_VALUE_H

#include "../common/list.h"
#include "binding.h"


struct LA1_State;
typedef struct LA1_State LA1_State;

struct ConsCell {
    Value *item;
    Value *next;
};

typedef struct ConsCell ConsCell;

struct DataClosure {
    LinkedList *environment;
    LinkedList *parameters;
    Value *body_source;
};

typedef struct DataClosure DataClosure;

typedef Value *ClosureFunction(LA1_State *state, ConsCell *parameters,
                               void *extra);

enum ClosureType { LA1_CLOSURE_C, LA1_CLOSURE_DATA };

typedef enum ClosureType ClosureType;

typedef struct {
    ClosureFunction *function;
    void *extra;
    int is_macro;
    ClosureType type;
} Closure;

#define LA1_VALUE_TYPE_X() \
    X(NUMBER)              \
    X(CONS)                \
    X(SYMBOL)              \
    X(CLOSURE)

enum ValueType {
#define X(name) LA1_VALUE_##name,
    LA1_VALUE_TYPE_X()
#undef X
};

typedef enum ValueType ValueType;


union UntaggedValue {
    ConsCell *cons;
    KnownSymbol symbol;
    long number;
    Closure *closure;
};

typedef union UntaggedValue UntaggedValue;

struct Value {
    ValueType type;
    int gc_tag;
    UntaggedValue content;
};

const char *la1_get_type_name(ValueType type);

Value *la1_cons_into_value(LA1_State *state, ConsCell *value);

Value *la1_number_into_value(LA1_State *state, long value);

Value *la1_symbol_into_value(LA1_State *state, KnownSymbol symbol);

Closure *la1_create_c_closure(ClosureFunction *function, void *extra);

Value *la1_closure_into_value(LA1_State *state, Closure *closure);

void la1_expect_type(Value *value, ValueType type);

void la1_expect_size(ConsCell *list, unsigned int size);

unsigned int la1_find_cons_list_size(ConsCell *cell);

ConsCell *la1_cons(Value *item, Value *next);

ConsCell *la1_cons_next(ConsCell *cell);

void la1_display_value(Value *value);
void la1_display_value_ln(Value *value);

#endif  // LATTEMPT_VALUE_H
