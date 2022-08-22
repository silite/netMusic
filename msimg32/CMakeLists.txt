cmake_minimum_required(VERSION 3.23)
project(msimg32)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(msimg32
        dllmain.cpp
        framework.h
        pch.cpp
        pch.h)
