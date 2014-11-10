#-------------------------------------------------
#
# Project created by QtCreator 2013-11-29T21:23:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeGuard
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    admin.cpp \
    adminlogindialog.cpp \
    filemanager.cpp \
    logger.cpp \
    timeguard.cpp \
    timer.cpp \
    user.cpp \
    messages.cpp \
    systemquery.cpp \
    userstablemodel.cpp \
    timelimit.cpp

HEADERS  += \
    admin.h \
    adminlogindialog.h \
    filemanager.h \
    logger.h \
    timeguard.h \
    timer.h \
    user.h \
    messages.h \
    systemquery.h \
    userstablemodel.h \
    timelimit.h

FORMS    += timeguard.ui \
    adminlogindialog.ui

LIBS += advapi32.lib\
        netapi32.lib

RESOURCES += \
    images.qrc
