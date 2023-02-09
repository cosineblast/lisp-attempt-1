//
// Created by figurantpp on 09/02/23.
//

#ifndef LATTEMPT_SPECIAL_FORMS_H
#define LATTEMPT_SPECIAL_FORMS_H

#include "value.h"
#include "execution.h"


Value *la1_if_special_form(LA1_State *state, LinkedList *arguments);

Value *la1_lambda_special_form(LA1_State *state, LinkedList *lambda_arguments);

Value *la1_quote_special_form(LA1_State *state, LinkedList *arguments);
Value *la1_let_special_form(LA1_State *state, LinkedList *arguments);
Value *la1_nil_special_form(LA1_State *state, LinkedList *arguments);

Value *la1_do_special_form(LA1_State *state, LinkedList *arguments);


#endif //LATTEMPT_SPECIAL_FORMS_H
