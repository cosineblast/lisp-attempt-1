//
// Created by cosineblast on 09/02/23.
//

#include "value.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/alloc.h"
#include "../common/die.h"
#include "gc.h"

void display_list_in(ConsCell *list);

void display_list(ConsCell *list);

void display_value(Value *value);

void la1_display_value(Value *value) { display_value(value); }

void la1_display_value_ln(Value *value) {
    display_value(value);

    printf("\n");
}

void display_value(Value *value) {
    switch (value->type) {
        case LA1_VALUE_CONS:
            display_list(value->content.cons);
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

        case LA1_VALUE_REITERATION:
            printf("<reiteration>");
            break;

        default:

            fprintf(stderr, "Illegal parse value type %d\n", value->type);
            abort();
    }
}

Value *la1_cons_into_value(LA1_State *state, ConsCell *value) {
    assert(state != NULL);
    Value result;
    result.content.cons = value;
    result.type = LA1_VALUE_CONS;
    return la1_gc_spawn(state, &result);
}

Value *la1_cons_into_reiteration_value(LA1_State *state, ConsCell *value) {
    assert(state != NULL);
    Value result;
    result.content.cons = value;
    result.type = LA1_VALUE_REITERATION;
    return la1_gc_spawn(state, &result);
}

Value *la1_number_into_value(LA1_State *state, long value) {
    assert(state != NULL);
    Value result;
    result.content.number = value;
    result.type = LA1_VALUE_NUMBER;
    return la1_gc_spawn(state, &result);
}

Value *la1_symbol_into_value(LA1_State *state, KnownSymbol symbol) {
    assert(state != NULL);
    Value result;
    result.content.symbol = symbol;
    result.type = LA1_VALUE_SYMBOL;
    return la1_gc_spawn(state, &result);
}

Value *la1_closure_into_value(LA1_State *state, Closure *closure) {
    assert(state != NULL);
    Value result;
    result.content.closure = closure;
    result.type = LA1_VALUE_CLOSURE;
    return la1_gc_spawn(state, &result);
}

Closure *la1_create_c_closure(ClosureFunction *function, void *extra) {
    Closure *result = la1_malloc(sizeof(*result));
    result->function = function;
    result->extra = extra;
    result->type = LA1_CLOSURE_C;
    result->is_macro = 0;
    return result;
}

void la1_expect_type(Value *value, ValueType type) {
    if (value->type != type) {
        la1_die_format("Expected type %s, got %s", la1_get_type_name(type),
                       la1_get_type_name(value->type));
    }
}

void la1_expect_size(ConsCell *list, unsigned int size) {
    if (la1_find_cons_list_size(list) != size) {
        la1_die("Wrong number of arguments");
    }
}

const char *la1_get_type_name(ValueType type) {
    switch (type) {
#define X(name)            \
    case LA1_VALUE_##name: \
        return #name;
        LA1_VALUE_TYPE_X()
#undef X
        default:
            la1_die("Unknown type name.");
    }
}

ConsCell *la1_cons_next(ConsCell *cell) {
    la1_expect_type(cell->next, LA1_VALUE_CONS);
    return cell->next->content.cons;
}

unsigned int la1_find_cons_list_size(ConsCell *cell) {

    unsigned int result = 0;

    ConsCell *current = cell;

    while (current != NULL) {
        result += 1;
        current = la1_cons_next(current);
    }

    return result;
}

ConsCell *la1_cons(Value *item, Value *next) {

    assert(item != NULL);
    assert(next != NULL);

    la1_expect_type(next, LA1_VALUE_CONS);

    ConsCell *cell = la1_malloc(sizeof(*cell));
    cell->item = item;
    cell->next = next;

    return cell;
}

void display_list(ConsCell *list) {

    if (list == NULL) {
        printf("()");
    } else {
        printf("( ");
        display_value(list->item);
        printf(" ");
        display_list_in(list->next->content.cons);
        printf(")");
    }
}

void display_list_in(ConsCell *node) {
    if (node != NULL) {
        display_value(node->item);

        printf(" ");
        display_list_in(node->next->content.cons);
    }
}
