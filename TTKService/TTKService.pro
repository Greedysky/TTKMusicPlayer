#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2014 - 2016 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:TARGET = ../../bin/TTKMusicPlayer
unix:TARGET = ../lib/TTKMusicPlayer
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../ \
    ../TTKCore/musicCore

SOURCES += \
    musicservicemain.cpp \
    musicserviceapplication.cpp \
    musicserviceobject.cpp

HEADERS += \
    musicserviceglobaldefine.h \
    musicserviceobject.h \
    musicserviceapplication.h

win32{
    RC_FILE = TTKService.rc
}
