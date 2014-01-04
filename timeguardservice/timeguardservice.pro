#-------------------------------------------------
#
# Project created by QtCreator 2014-01-03T23:58:23
#
#-------------------------------------------------

#QT       += core
#QT       -= gui

TARGET = timeguardservice
CONFIG += console
CONFIG -= qt
CONFIG -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    servicebase.cpp \
    timeguardservice.cpp

HEADERS += \
    servicebase.h \
    timeguardservice.h

LIBS += advapi32.lib
