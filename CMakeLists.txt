cmake_minimum_required(VERSION 2.9)

if (POLICY CMP0048)  # suppress warning about project version
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(ATP)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(DataFrame)
add_subdirectory(src)
