cmake_minimum_required(VERSION 2.6)

if (POLICY CMP0048)  # suppress warning about project version
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(ATP)

add_subdirectory(DataFrame)

add_executable(demo demo.cpp)

add_executable(test backtest.cpp)
target_link_libraries(test DataFrame)
