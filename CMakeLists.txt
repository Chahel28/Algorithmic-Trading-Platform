cmake_minimum_required(VERSION 2.6)

if (POLICY CMP0048)  # suppress warning about project version
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(ATP)

add_subdirectory(DataFrame)

add_executable(Demo Demo.cpp)

add_executable(Exchange Exchange.cpp)

add_executable(Test Backtest.cpp)
target_link_libraries(Test DataFrame)
