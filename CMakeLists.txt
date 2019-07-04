cmake_minimum_required(VERSION 3.1.0)
project(tevianGUI)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})

set (PROJECT_LINK_LIBS ${CMAKE_CURRENT_BINARY_DIR}/libtevianDLL.dll)

set (HEADERS
    zoomview.h
    mainwindow.h
    jsonparser.h
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    zoomview.cpp
    jsonparser.cpp
)

add_executable(tevianGUI WIN32 ${SOURCES})

target_link_libraries(tevianGUI Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets)
target_link_libraries(tevianGUI ${PROJECT_LINK_LIBS})
qt5_use_modules(tevianGUI core network)


