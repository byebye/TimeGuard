a#-------------------------------------------------
#
# Project created by QtCreator 2013-11-29T21:23:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeGuard
TEMPLATE = app


SOURCES += main.cpp\
    FileManager.cpp \
    Timer.cpp \
    TimeGuard.cpp \
    User.cpp \
    Logger.cpp \
    AdminLoginDialog.cpp \
    Admin.cpp

HEADERS  += \
    FileManager.h \
    Timer.h \
    TimeGuard.h \
    User.h \
    Logger.h \
    AdminLoginDialog.h \
    Admin.h

FORMS    += timeguard.ui \
    AdminLoginDialog.ui

LIBS += advapi32.lib
