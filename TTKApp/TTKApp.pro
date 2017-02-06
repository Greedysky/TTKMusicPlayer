# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../TTKVersion.pri)
unix:VERSION += $$TTKMusicPlayer

win32:TARGET = ../../bin/TTKMusicPlayer
unix:TARGET = ../lib/TTKMusicPlayer

TEMPLATE = app

win32:msvc{
    CONFIG +=c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
}

INCLUDEPATH += ../ \
    ../TTKCore/musicCoreKits

SOURCES += \
    musicrunmain.cpp \
    musiclocalpeer.cpp \
    musicrunapplication.cpp \
    musicrunobject.cpp

HEADERS += \
    musicrunglobaldefine.h \
    musicrunobject.h \
    musiclocalpeer.h \
    musicrunapplication.h

RESOURCES += \
    ../TTKQrc/MusicApp.qrc

win32{
    RC_FILE = TTKApp.rc
}
