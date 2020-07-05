cmake_minimum_required(VERSION 3.15)
project(Cohen_Sutherland_algorithm C)

set(CMAKE_C_STANDARD 11)

add_executable(Cohen_Sutherland_algorithm main.c)
target_link_libraries(Cohen_Sutherland_algorithm -lOpenGL32 -lGlu32 -lFreeGlut)