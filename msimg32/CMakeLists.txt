cmake_minimum_required(VERSION 3.23)

project(msimg32)

set(CMAKE_CXX_STANDARD 14)

include_directories(./cef)

SET(LIBHELLO_SRC ./dllmain.cpp)
ADD_LIBRARY(msimg32_dll SHARED ${LIBHELLO_SRC})