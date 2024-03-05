cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)

project(calculations C)

add_executable(calculations
    main.c
    "src/fileout.c"
)

target_include_directories(calculations PRIVATE
    "include/"
)
