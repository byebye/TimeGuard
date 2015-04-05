#-------------------------------------------------
#
# Project created by QtCreator 2015-04-05T18:38:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeGuardGUI
TEMPLATE = app


SOURCES += main.cpp\
        timeguardmainwindow.cpp

HEADERS  += timeguardmainwindow.h

FORMS    += timeguardmainwindow.ui

win32: LIBS += -lwtsapi32
