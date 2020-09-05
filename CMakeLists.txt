cmake_minimum_required(VERSION 3.0.0)
project(char8)
set(CMAKE_CXX_STANDARD 20)

add_library(catch2 INTERFACE)
target_include_directories(catch2
INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/_external/Catch2
)

add_library(char8 INTERFACE)
target_include_directories(char8
INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

add_executable(char8_test
    tests/main.cpp
    tests/utf8_test.cpp
    tests/sjis_test.cpp
)
target_link_libraries(char8_test
PRIVATE
    catch2
    char8
)
