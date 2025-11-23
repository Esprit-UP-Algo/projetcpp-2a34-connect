cmake_minimum_required(VERSION 3.16)

project(FinanceManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql Charts)

set(SOURCES
    main.cpp
    mainwindow.cpp
    connection.cpp
    notificationmanager.cpp
    predictionengine.cpp
)

set(HEADERS
    mainwindow.h
    connection.h
    notificationmanager.h
    predictionengine.h
)

set(FORMS
    mainwindow.ui
)

add_executable(FinanceManager ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(FinanceManager
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
)

set_target_properties(FinanceManager PROPERTIES
    WIN32_EXECUTABLE TRUE
)
