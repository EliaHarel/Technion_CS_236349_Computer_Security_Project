cmake_minimum_required(VERSION 3.16)
project(newproject C)

set(CMAKE_C_STANDARD 11)

add_executable(newproject main.c des_noop.c)