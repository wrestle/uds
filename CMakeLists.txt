cmake_minimum_required(VERSION 2.8)
project(udatastruct)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Werror")

aux_source_directory(./ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
