cmake_minimum_required(VERSION 3.25)
project(Compiler_Proj)

set(CMAKE_CXX_STANDARD 17)

add_executable(Compiler_Proj main.cpp File_Desc.h File_Desc.cpp scanner.h scanner.cpp scanner.cpp scanner.cpp Symbol.h)
