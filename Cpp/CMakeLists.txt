cmake_minimum_required(VERSION 3.17)
project(Technion_CS_236349_Computer_Security_Project_Cpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(Technion_CS_236349_Computer_Security_Project_Cpp main.cpp DES_Encrypt.cpp Data.cpp
        AttackAlgorithm1.cpp RuningAttackUtil.cpp Tables.cpp)