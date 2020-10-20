cmake_minimum_required(VERSION 3.17)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(compiler main.cpp getToken.cpp getToken.h parse.cpp parse.h myVar.cpp myVar.h)