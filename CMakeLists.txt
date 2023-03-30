cmake_minimum_required(VERSION 3.22)
project(p1 C)

set(CMAKE_C_STANDARD 99)

add_executable(p1 AdvCalc.c token.h formatController.h postfixCalculator.h)

# Find and link against the math library
find_library(MATH_LIB m)
if(MATH_LIB)
    target_link_libraries(p1 ${MATH_LIB})
endif()