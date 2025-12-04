cmake_minimum_required(VERSION 3.16)

project(interface23 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ AJOUT DES MODULES REQUIS (Inclure SerialPort et Charts)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Sql Widgets Network SerialPort Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql Widgets Network SerialPort Charts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        # --- Classes Métier (CRUD) ---
        employes.cpp
        employes.h
        sponsor.cpp
        sponsor.h

        # --- Connexion BDD ---
        connection.cpp
        connection.h

        # --- Modules Avancés ---
        chatbot.cpp        # IA Gemini
        chatbot.h
        smsmanager.cpp     # SMS
        smsmanager.h
        historymanager.cpp # Historique
        historymanager.h
        smtp.cpp           # ✅ Envoi Email (Mailing)
        smtp.h

        # --- Widgets Graphiques ---
        curvewidget.cpp    # Courbes
        curvewidget.h
        barchartwidget.cpp # Barres
        barchartwidget.h

        # --- Édition ---
        fichedepaie.cpp      # PDF
        fichedepaie.h

        # --- Badgeuse Arduino ---
        serialmanager.cpp
        serialmanager.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(interface23
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(interface23 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(interface23
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# ✅ LIEN AVEC LES BIBLIOTHÈQUES (Inclure SerialPort et Charts)
target_link_libraries(interface23 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Charts
)

# Configuration spécifique MacOS/Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.interface23)
endif()

set_target_properties(interface23 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS interface23
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(interface23)
endif()
