
#pragma once

#include <stddef.h>

#include "value.h"

struct LA1_DataStack;

typedef struct LA1_DataStack LA1_DataStack;

LA1_DataStack *la1_data_stack_create();

void la1_data_stack_push(LA1_DataStack* stack, Value *value);
Value* la1_data_stack_pop(LA1_DataStack* stack);

void la1_data_stack_free(LA1_DataStack *stack);

