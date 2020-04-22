cmake_minimum_required(VERSION 3.15)
project(rep_project C)

set(CMAKE_C_STANDARD 99)

add_executable(rep_project main.c archiver.c decompressor.c archiver.h decompressor.h structures.c structures.h debugging.c debugging.h free_tree.c free_tree.h util.c util.h get_test_data.c get_test_data.h)