cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)

project(gridcap C)

FILE(
    GLOB sourceFiles 
    src/*.c
    src/*.cpp
)

add_executable(gridcap
    src/main.c
    ${sourceFiles}
)

target_link_libraries(gridcap
    -lpthread
)

target_include_directories(gridcap PRIVATE
    "include/"
)
