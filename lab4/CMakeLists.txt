cmake_minimum_required(VERSION 3.5.1)
project(lab4 C)

set(SRC src/main.c src/expression_correctness.c src/input.c src/postfix_notation.c src/structures.c src/structures.h)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
