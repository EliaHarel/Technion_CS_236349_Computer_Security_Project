cmake_minimum_required(VERSION 3.17)
project(Code)

set(CMAKE_CXX_STANDARD 11)


add_executable(Technion_CS_236349_Computer_Security_Project_Cpp main.cpp DES_Encrypt.cpp DataUtils.cpp
         RunningAttackUtil.cpp Tables.cpp
        AttackAlgorithm2.cpp AttackAlgorithm2.h
        ShortTablesAttackAlgorithm2FewLevels.cpp ShortTablesAttackAlgorithm2FewLevels.h)