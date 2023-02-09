//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_BUILTIN_H
#define LATTEMPT_BUILTIN_H

#include "value.h"

Value *la1_builtin_list(LA1_State *state, LinkedList *arguments, void *);

Value *la1_builtin_cons(LA1_State *state, LinkedList *arguments, void *);

Value *la1_builtin_plus(LA1_State *state, LinkedList *arguments, void *);


#endif //LATTEMPT_BUILTIN_H
