#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T10:28:05
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = TimeGuardService
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(QtService/qtservice.pri)

SOURCES += main.cpp \
   timeguardservice.cpp \
    userslimitcontroller.cpp \
    user.cpp \
    userlimittimer.cpp \
    limitsettingsmanager.cpp \
    guicommunicationsocket.cpp \
    individualcommunicationchannel.cpp \
    usersessionmanager.cpp


HEADERS += \
   timeguardservice.h \
    userslimitcontroller.h \
    user.h \
    userlimittimer.h \
    limitsettingsmanager.h \
    guicommunicationsocket.h \
    individualcommunicationchannel.h \
    usersessionmanager.h


unix|win32: LIBS += -L$$PWD/../libs/QsLog/ -lQsLog2

INCLUDEPATH += $$PWD/../libs/QsLog
DEPENDPATH += $$PWD/../libs/QsLog

win32: LIBS += -lwtsapi32
