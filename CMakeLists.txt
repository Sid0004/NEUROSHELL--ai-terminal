cmake_minimum_required(VERSION 3.16)
project(NEUROSHELL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
        main.cpp
        TerminalWindow.cpp
        TerminalWindow.h
)

qt_add_executable(NEUROSHELL
        ${SOURCES}
)

target_link_libraries(NEUROSHELL
        PRIVATE Qt6::Widgets
)
