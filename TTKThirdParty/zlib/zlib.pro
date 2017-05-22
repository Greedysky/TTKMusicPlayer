# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = lib

include(../../TTKVersion.pri)

win32:TARGET = ../../../bin/$$TTKMusicPlayer/zlib
unix:TARGET = ../../lib/$$TTKMusicPlayer/zlib

CONFIG       += warn_off
unix:VERSION += 1.2.8

DEFINES += \
    ZLIB_DLL \
    ZLIB_INTERNAL

HEADERS  += \
    $$PWD/zconf.h \
    $$PWD/zlib.h \
    $$PWD/crc32.h \
    $$PWD/deflate.h \
    $$PWD/gzguts.h \
    $$PWD/inffixed.h \
    $$PWD/inffast.h \
    $$PWD/inflate.h \
    $$PWD/inftrees.h \
    $$PWD/trees.h \
    $$PWD/zutil.h

SOURCES += \
    $$PWD/adler32.c \
    $$PWD/compress.c \
    $$PWD/crc32.c \
    $$PWD/deflate.c \
    $$PWD/gzclose.c \
    $$PWD/gzlib.c \
    $$PWD/gzread.c \
    $$PWD/gzwrite.c \
    $$PWD/inflate.c \
    $$PWD/infback.c \
    $$PWD/inftrees.c \
    $$PWD/inffast.c \
    $$PWD/trees.c \
    $$PWD/uncompr.c \
    $$PWD/zutil.c
    
win32{
    RC_FILE = zlib.rc
}
