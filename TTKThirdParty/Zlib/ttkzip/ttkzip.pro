# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = lib

include(../../../TTKVersion.pri)

win32:TARGET = ../../../../bin/$$TTKMusicPlayer/TTKZip
unix:TARGET = ../../../lib/$$TTKMusicPlayer/TTKZip

CONFIG       += warn_off
unix:VERSION += 1.0.0

DEFINES += \
    ZLIB_DLL \
    ZLIB_INTERNAL

INCLUDEPATH += $$PWD/../

win32{
    LIBS += -L../../../bin/$$TTKMusicPlayer -lzlib
}
unix:!mac{
    LIBS += -L../../../lib/$$TTKMusicPlayer -lzlib
}

HEADERS  += \
    $$PWD/zip.h \
    $$PWD/unzip.h \
    $$PWD/mztools.h \
    $$PWD/iowin32.h \
    $$PWD/ioapi.h \
    $$PWD/crypt.h

SOURCES += \
    $$PWD/zip.c \
    $$PWD/unzip.c \
    $$PWD/mztools.c \
    $$PWD/iowin32.c \
    $$PWD/ioapi.c
    
win32{
    RC_FILE = ttkzip.rc
}
