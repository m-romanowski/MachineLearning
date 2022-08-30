cmake_minimum_required(VERSION 3.10)
project(Perceptron)

find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(SOURCES ${CMAKE_SOURCE_DIR}/src)
set(ASSETS ${CMAKE_SOURCE_DIR}/resources)

include_directories(${SOURCES})

set(SOURCE_FILES
        ${SOURCES}/main.cpp
        ${SOURCES}/mainwindow.cpp
        ${SOURCES}/perceptron.cpp
        ${SOURCES}/pixelgrid.cpp
        ${SOURCES}/settings.cpp
        ${SOURCES}/trainingitem.cpp
        ${SOURCES}/util.cpp)

file(COPY ${ASSETS}/ DESTINATION ${CMAKE_BINARY_DIR})
add_executable(Perceptron ${SOURCE_FILES})
target_link_libraries(Perceptron Qt5::Widgets)
