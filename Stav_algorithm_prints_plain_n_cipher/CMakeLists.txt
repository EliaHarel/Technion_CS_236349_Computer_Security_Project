cmake_minimum_required(VERSION 3.16)
project(Stav_algorithm_prints_plain_n_cipher C)

set(CMAKE_C_STANDARD 11)
set(DCMAKE_SH CMAKE_SH-NOTFOUND)

add_executable(Stav_algorithm_prints_plain_n_cipher main.c des_noop.c)