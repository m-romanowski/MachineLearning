cmake_minimum_required(VERSION 3.10)
project(Adaline)

find_package(Qt5 REQUIRED PrintSupport Widgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(SOURCES ${CMAKE_SOURCE_DIR}/src)
set(ASSETS ${CMAKE_SOURCE_DIR}/resources)

include_directories(${SOURCES})

set(SOURCE_FILES
        ${SOURCES}/adaline.cpp
        ${SOURCES}/customplot.cpp
        ${SOURCES}/main.cpp
        ${SOURCES}/mainwindow.cpp
        ${SOURCES}/pixelgrid.cpp
        ${SOURCES}/qcustomplot.cpp
        ${SOURCES}/settings.cpp
        ${SOURCES}/trainingitem.cpp
        ${SOURCES}/util.cpp)

file(COPY ${ASSETS}/ DESTINATION ${CMAKE_BINARY_DIR})
add_executable(Adaline ${SOURCE_FILES})
target_link_libraries(Adaline Qt5::PrintSupport Qt5::Widgets)
