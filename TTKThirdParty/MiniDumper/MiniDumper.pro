# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

QT       += core

TEMPLATE = lib

include(../../TTKVersion.pri)

win32:TARGET = ../../../bin/$$TTKMusicPlayer/TTKDumper
unix:TARGET = ../../lib/$$TTKMusicPlayer/TTKDumper

CONFIG       += warn_off
unix:VERSION += $$TTKMusicPlayer

INCLUDEPATH += $$PWD/../../TTKCore/musicCoreKits

win32{
    LIBS += -lpsapi

HEADERS += \
    $$PWD/mini.h
}

SOURCES += \
    $$PWD/minidumper.cpp \
    $$PWD/ttkdumper.cpp
    
HEADERS += \
    $$PWD/minidumper.h \
    $$PWD/ttkdumper.h

#load extra define
include(../TTKExtrasDefine.pri)

win32{
    RC_FILE = MiniDumper.rc
}
