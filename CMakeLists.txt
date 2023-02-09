cmake_minimum_required(VERSION 3.24)
project(lattempt C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-Werror -Wall")

add_executable(lattempt la1/main.c la1/parsing/parse_value.h la1/common/list.c la1/common/list.h la1/parsing/lexer.c la1/parsing/lexer.h la1/common/alloc.c la1/common/alloc.h la1/parsing/parser.c la1/parsing/parser.h la1/parsing/parse_value.c la1/execution/execution.c la1/execution/execution.h la1/execution/value.c la1/execution/value.h la1/execution/binding.c la1/execution/binding.h la1/execution/special_forms.c la1/execution/special_forms.h la1/execution/realize.c la1/common/die.c la1/common/die.h la1/execution/builtin.c la1/execution/builtin.h)
