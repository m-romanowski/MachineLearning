#-------------------------------------------------
#
# Project created by QtCreator 2019-10-13T11:23:15
#
#-------------------------------------------------

ROOT_DIRECTORY = $$PWD
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
EXEC_DIRECTORY = $${BUILD_DIRECTORY}

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}
TARGET = Perceptron
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        perceptron.cpp \
        pixelgrid.cpp \
        trainingitem.cpp \
        util.cpp \
        settings.cpp

HEADERS += \
        mainwindow.h \
        perceptron.h \
        pixelgrid.h \
        trainingitem.h \
        util.h \
        settings.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
