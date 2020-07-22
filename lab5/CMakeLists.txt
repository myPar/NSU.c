cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(SRC src/main.c src/archiver.c src/copy_file_data.c src/decompressor.c src/free_tree.c src/structures.c src/structures.h src/util.c src/util.h)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
