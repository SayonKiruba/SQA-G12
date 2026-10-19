cmake_minimum_required(VERSION 4.3)
project(SQA_G12)

set(CMAKE_CXX_STANDARD 14)

include_directories(Phase02)

add_executable(SQA_G12
        Phase02/main.cpp
        Phase02/FrontEnd.h
        Phase02/User.cpp
        Phase02/User.h
        Phase02/Game.h
        Phase02/FrontEnd.cpp
        Phase02/TransactionManager.cpp
        Phase02/TransactionManager.h
        Phase02/FileManager.cpp
        Phase02/FileManager.h)
