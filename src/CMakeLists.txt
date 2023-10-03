cmake_minimum_required(VERSION 3.5)

project(Search_Engine VERSION 1.0 LANGUAGES CXX)

enable_testing()

# Building main application
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Concurrent)

set(PROJECT_SOURCES
    main.cpp
    ${MY_INCLUDE_DIR}/mainwindow.h mainwindow.cpp mainwindow.ui
    ${MY_INCLUDE_DIR}/converterjson.h converterjson.cpp
    ${MY_INCLUDE_DIR}/invertedindex.h invertedindex.cpp
    ${MY_INCLUDE_DIR}/searchserver.h searchserver.cpp
    ${MY_INCLUDE_DIR}/searchengineexeptions.h
    ${MY_INCLUDE_DIR}/listmodel.h listmodel.cpp
    images.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Search_Engine
	MANUAL_FINALIZATION
	${PROJECT_SOURCES}
    )
else()
    add_executable(Search_Engine
	${PROJECT_SOURCES}
    )
endif()

target_include_directories(Search_Engine PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(Search_Engine PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Concurrent
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Search_Engine)
endif()

set_target_properties(Search_Engine PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Search_Engine
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Search_Engine)
endif()

