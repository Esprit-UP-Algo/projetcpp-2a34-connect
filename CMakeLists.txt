cmake_minimum_required(VERSION 3.16)
project(content_creators_manger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

qt_add_executable(content_creators_manger
    main.cpp
    mainwindow.cpp
    logindialog.cpp
    qrcodegenerator.cpp
    qrcodegen.cpp
    connection.cpp

    mainwindow.h
    logindialog.h
    qrcodegenerator.h
    connection.h
)

target_sources(content_creators_manger PRIVATE
    mainwindow.ui
    logindialog.ui
)

target_link_libraries(content_creators_manger PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Sql
)

file(COPY qrcodegen.hpp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
