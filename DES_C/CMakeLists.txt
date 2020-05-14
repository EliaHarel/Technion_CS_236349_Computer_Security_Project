cmake_minimum_required(VERSION 3.16)
project(DES_C C)

set(CMAKE_C_STANDARD 11)
set(DCMAKE_SH CMAKE_SH-NOTFOUND)

add_executable(DES_C main.c des_noop.c)