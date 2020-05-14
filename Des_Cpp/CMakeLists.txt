cmake_minimum_required(VERSION 3.16)
project(Project_Try C)

set(CMAKE_C_STANDARD 11)
set(DCMAKE_SH CMAKE_SH-NOTFOUND)

add_executable(Project_Try main.c des_noop.c)