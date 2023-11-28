cmake_minimum_required(VERSION 3.23)
project(project C)

set(CMAKE_C_STANDARD 99)

add_executable(project parent.c child.c)
