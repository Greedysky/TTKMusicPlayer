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

INCLUDEPATH += $$PWD \
               $$PWD/qxtglobalshortcut \
               $$PWD/zlib

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/qxtglobalshortcut/qxtglobal.h \
    $$PWD/qxtglobalshortcut/qxtglobalshortcut.h \
    $$PWD/qxtglobalshortcut/qxtglobalshortcut_p.h \
    $$PWD/zlib/crc32.h \
    $$PWD/zlib/deflate.h \
    $$PWD/zlib/gzguts.h \
    $$PWD/zlib/inffixed.h \
    $$PWD/zlib/inffast.h \
    $$PWD/zlib/inflate.h \
    $$PWD/zlib/inftrees.h \
    $$PWD/zlib/trees.h \
    $$PWD/zlib/zconf.h \
    $$PWD/zlib/zlib.h \
    $$PWD/zlib/zutil.h

}

contains(CONFIG, TTK_BUILD_LIB){
win32:SOURCES += $$PWD/qxtglobalshortcut/qxtglobalshortcut_win.cpp
unix:SOURCES += $$PWD/qxtglobalshortcut/qxtglobalshortcut_x11.cpp
mac:SOURCES += $$PWD/qxtglobalshortcut/qxtglobalshortcut_mac.cpp
SOURCES += \
    $$PWD/qxtglobalshortcut/qxtglobalshortcut.cpp \
    $$PWD/zlib/adler32.c \
    $$PWD/zlib/compress.c \
    $$PWD/zlib/crc32.c \
    $$PWD/zlib/deflate.c \
    $$PWD/zlib/gzclose.c \
    $$PWD/zlib/gzlib.c \
    $$PWD/zlib/gzread.c \
    $$PWD/zlib/gzwrite.c \
    $$PWD/zlib/infback.c \
    $$PWD/zlib/inffast.c \
    $$PWD/zlib/inflate.c \
    $$PWD/zlib/inftrees.c \
    $$PWD/zlib/trees.c \
    $$PWD/zlib/uncompr.c \
    $$PWD/zlib/zutil.c

}
