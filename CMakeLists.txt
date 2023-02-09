cmake_minimum_required(VERSION 3.24)
project(lattempt C)

set(CMAKE_C_STANDARD 99)

add_executable(lattempt la1/main.c la1/parsing/parse_value.h la1/common/list.c la1/common/list.h la1/parsing/lexer.c la1/parsing/lexer.h la1/common/alloc.c la1/common/alloc.h la1/parsing/parser.c la1/parsing/parser.h la1/parsing/parse_value.c la1/execution/execution.c la1/execution/execution.h la1/execution/value.c la1/execution/value.h la1/execution/binding.c la1/execution/binding.h)
