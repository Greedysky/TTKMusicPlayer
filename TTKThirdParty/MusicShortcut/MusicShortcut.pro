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

win32:TARGET = ../../../bin/MusicShortcut
unix:TARGET = ../../lib/MusicShortcut
TEMPLATE = lib

HEADERS  += \
    qxtglobal.h \
    qxtglobalshortcut.h \
    qxtglobalshortcut_p.h


win32:SOURCES += qxtglobalshortcut_win.cpp
unix:SOURCES += qxtglobalshortcut_x11.cpp
mac:SOURCES += qxtglobalshortcut_mac.cpp

SOURCES += \
    qxtglobalshortcut.cpp

