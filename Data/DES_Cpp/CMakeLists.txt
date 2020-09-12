cmake_minimum_required(VERSION 3.16)
project(DES_Cpp)

set(CMAKE_CXX_STANDARD 17)
set(DCMAKE_SH CMAKE_SH-NOTFOUND)

add_executable(DES_Cpp main.cpp DES_Encrypt.cpp)