cmake_minimum_required(VERSION 3.16)
project(content_creators_manger VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatique Qt (obligatoire)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# COMPOSANTS QT OBLIGATOIRES (SerialPort ajouté = Arduino)
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Sql
    SerialPort      # ← C’EST ÇA QUI MANQUAIT PEUT-ÊTRE CHEZ TOI !!!
    PrintSupport
)

# Exécutable
qt_add_executable(content_creators_manger
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    logindialog.cpp
    logindialog.h
    logindialog.ui
    connection.cpp
    connection.h
    qrcodegenerator.cpp
    qrcodegenerator.h
    qrcodegen.cpp
    resources.qrc
)

# Liaison des libs Qt
target_link_libraries(content_creators_manger PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::SerialPort      # ← INDISPENSABLE POUR QSerialPort
    Qt6::PrintSupport
)

# Windows = pas de console
if(WIN32)
    set_target_properties(content_creators_manger PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
