cmake_minimum_required(VERSION 4.3)
project(SQA_G12)

set(CMAKE_CXX_STANDARD 14)

include_directories(Phase03)

add_executable(SQA_G12
        Phase03/Frontend/main.cpp
        Phase03/Frontend/FrontEnd.h
        Phase03/Frontend/User.cpp
        Phase03/Frontend/User.h
        Phase03/Frontend/Game.h
        Phase03/Frontend/FrontEnd.cpp
        Phase03/Frontend/TransactionManager.cpp
        Phase03/Frontend/TransactionManager.h
        Phase03/Frontend/FileManager.cpp
        Phase03/Frontend/FileManager.h)
