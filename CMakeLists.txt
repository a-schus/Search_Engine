cmake_minimum_required(VERSION 3.5)

project(Search_Engine VERSION 1.0 LANGUAGES CXX)

enable_testing()

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(TESTS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/test/include)

set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)

if (${IS_TESTS})
    # Building tests
    add_subdirectory(test)
endif()
