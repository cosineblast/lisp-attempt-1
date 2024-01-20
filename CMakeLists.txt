cmake_minimum_required(VERSION 3.18)
project(lattempt C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-Werror -Wall -fsanitize=address -Wno-unused-function")

add_executable(
    lattempt
    src/main.c
    src/parsing/parse_value.h
    src/common/list.c
    src/common/list.h
    src/parsing/lexer.c
    src/parsing/lexer.h
    src/common/alloc.c
    src/common/alloc.h
    src/parsing/parser.c
    src/parsing/parser.h
    src/parsing/parse_value.c
    src/execution/execution.c
    src/execution/execution.h
    src/execution/value.c
    src/execution/value.h
    src/execution/binding.c
    src/execution/binding.h
    src/execution/special_forms.c
    src/execution/special_forms.h
    src/execution/realize.c
    src/common/die.c
    src/common/die.h
    src/execution/builtin.c
    src/execution/builtin.h
    src/execution/gc.c
    src/execution/gc.h
    src/execution/realize.h
    src/execution/state.h)
