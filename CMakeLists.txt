cmake_minimum_required(VERSION 3.17)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(compiler main.cpp getToken.cpp getToken.h parse.cpp parse.h myVar.cpp myVar.h parse_expression.cpp parse_const_var.cpp parse_func_def.cpp parse_others.cpp parse_func_call.cpp generateMIPS.cpp)