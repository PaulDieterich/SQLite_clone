cmake_minimum_required(VERSION 3.27)
project(SQLite_clone C)

set(CMAKE_C_STANDARD 11)

add_executable(SQLite_clone types/types.c main.c
        types/InputFile.c)

