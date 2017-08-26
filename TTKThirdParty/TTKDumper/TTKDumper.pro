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
unix:VERSION += 1.0.0

INCLUDEPATH += $$PWD/../../TTKCore/musicCoreKits

win32{
    LIBS += -lpsapi
}

SOURCES += \
    $$PWD/minidumper.cpp \
    $$PWD/ttkdumper.cpp
    
HEADERS += \
    $$PWD/mini.h \
    $$PWD/minidumper.h \
    $$PWD/ttkdumper.h

#load extra define
include(../TTKExtrasDefine.pri)

win32{
    RC_FILE = TTKDumper.rc
}
