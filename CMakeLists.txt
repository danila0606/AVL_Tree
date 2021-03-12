cmake_minimum_required(VERSION 3.15)

project(Tree)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory(tests)


add_executable(Tree main.cpp gtests.cpp)