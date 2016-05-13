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

win32{
    TARGET = ../../../bin/MusicExtras
    msvc{
        LIBS += -luser32
    }
}
unix:TARGET = ../../lib/MusicExtras
TEMPLATE = lib



HEADERS  += \
    $$PWD/qxtglobal.h \
    $$PWD/qxtglobalshortcut.h \
    $$PWD/qxtglobalshortcut_p.h


win32:SOURCES += $$PWD/qxtglobalshortcut_win.cpp
unix:SOURCES += $$PWD/qxtglobalshortcut_x11.cpp
mac:SOURCES += $$PWD/qxtglobalshortcut_mac.cpp

SOURCES += \
    $$PWD/qxtglobalshortcut.cpp

