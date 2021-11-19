cmake_minimum_required(VERSION 3.20)
project(pipex C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(pipex
        pipex.c
        pipex.h)
