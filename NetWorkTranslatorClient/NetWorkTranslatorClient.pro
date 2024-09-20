#-------------------------------------------------
#
# Project created by QtCreator 2024-09-19T16:55:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetWorkTranslatorClient
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui
QMAKE_CXXFLAGS += -std=c++11
