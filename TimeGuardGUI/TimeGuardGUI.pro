#-------------------------------------------------
#
# Project created by QtCreator 2015-04-05T18:38:57
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeGuardGUI
TEMPLATE = app

SOURCES += main.cpp\
        timeguardmainwindow.cpp \
    servicecommunicationsocket.cpp \
    systeminfo.cpp \
    limittimer.cpp

HEADERS  += timeguardmainwindow.h \
    servicecommunicationsocket.h \
    systeminfo.h \
    limittimer.h

FORMS    += timeguardmainwindow.ui

win32: LIBS += -lwtsapi32

unix|win32: LIBS += -L$$PWD/../libs/QsLog/ -lQsLog2

INCLUDEPATH += $$PWD/../libs/QsLog
DEPENDPATH += $$PWD/../libs/QsLog
