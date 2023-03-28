cmake_minimum_required(VERSION 3.22)
project(p1 C)

set(CMAKE_C_STANDARD 99)

add_executable(p1 main.c idk.c parse.c cmake-build-debug/infix-postfix.c deneme.c postfixCalculator.h)
