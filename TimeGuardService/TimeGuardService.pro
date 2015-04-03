#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T10:28:05
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TimeGuardService
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(QtService/qtservice.pri)
SOURCES += main.cpp
