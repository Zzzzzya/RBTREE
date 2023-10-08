cmake_minimum_required(VERSION 3.0.0)
project(RBTREE)
file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(main ${SRC_LIST})