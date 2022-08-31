cmake_minimum_required(VERSION 3.10)
project(Autoassociator)

find_package(Qt5 REQUIRED Widgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(SOURCES ${CMAKE_SOURCE_DIR}/src)
set(ASSETS ${CMAKE_SOURCE_DIR}/resources)

include_directories(${SOURCES})

set(SOURCE_FILES
        ${SOURCES}/bitmaplc.cpp
        ${SOURCES}/clickablelabel.cpp
        ${SOURCES}/graphicsview.cpp
        ${SOURCES}/graphicsviewarea.cpp
        ${SOURCES}/linearclassifier.cpp
        ${SOURCES}/main.cpp
        ${SOURCES}/mainwindow.cpp
        ${SOURCES}/newviewdialog.cpp
        ${SOURCES}/perceptron.cpp
        ${SOURCES}/settings.cpp
        ${SOURCES}/trainingitem.cpp)

file(COPY ${ASSETS}/ DESTINATION ${CMAKE_BINARY_DIR})
add_executable(Autoassociator ${SOURCE_FILES})
target_link_libraries(Autoassociator Qt5::Widgets)
