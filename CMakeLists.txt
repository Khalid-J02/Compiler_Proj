cmake_minimum_required(VERSION 3.25)
project(Compiler_Proj C)

set(CMAKE_C_STANDARD 11)

add_executable(Compiler_Proj main.c File_Desc.h File_Desc.c)
