#-------------------------------------------------
#
# Project created by QtCreator 2014-01-03T23:58:23
#
#-------------------------------------------------

TEMPLATE = app

TARGET = timeguardservice

CONFIG += console
CONFIG -= qt app_bundle

SOURCES += \
    main.cpp \
    servicebase.cpp \
    timeguardservice.cpp

HEADERS += \
    servicebase.h \
    timeguardservice.h

LIBS += advapi32.lib
