cmake_minimum_required(VERSION 2.6)
project(ATP)

add_subdirectory(DataFrame)

add_executable(demo demo.cpp)

add_executable(test backtest.cpp)
target_link_libraries(test DataFrame)
