cmake_minimum_required(VERSION 3.12)
project(ComputerVision)

set(CMAKE_CXX_STANDARD 98)

set(BUILD_1 main)
set(SOURCE_FILES_1 Project2ConvexHull/andrews.cpp)
add_executable(${BUILD_1} ${SOURCE_FILES_1})
