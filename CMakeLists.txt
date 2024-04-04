cmake_minimum_required(VERSION 3.27)
project(SQLite_clone C)

set(CMAKE_C_STANDARD 11)

add_executable(SQLite_clone main.c
        types/inputBuffer.h
        types/inputFile.c
        types/table.c
        types/types.h
)

