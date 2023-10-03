cmake_minimum_required(VERSION 3.5)

project(Search_Engine_Tests LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core Gui Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Test)

add_executable(Search_Engine_Tests
    search_engine_tests.cpp
    ${TESTS_INCLUDE_DIR}/search_engine_tests.h
    ${MY_SRC_DIR}/converterjson.cpp
    ${MY_INCLUDE_DIR}/converterjson.h
    ${MY_SRC_DIR}/invertedindex.cpp
    ${MY_INCLUDE_DIR}/invertedindex.h
    ${MY_SRC_DIR}/searchserver.cpp
    ${MY_INCLUDE_DIR}/searchserver.h
)

target_include_directories(Search_Engine_Tests PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(Search_Engine_Tests PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)


