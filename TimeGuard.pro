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
    admin.cpp \
    adminlogindialog.cpp \
    filemanager.cpp \
    logger.cpp \
    timeguard.cpp \
    timer.cpp \
    user.cpp

HEADERS  += \
    admin.h \
    adminlogindialog.h \
    filemanager.h \
    logger.h \
    timeguard.h \
    timer.h \
    user.h

FORMS    += timeguard.ui \
    adminlogindialog.ui

LIBS += advapi32.lib\
        netapi32.lib
