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

win32:LIBS += -L../bin -lzlib
unix:LIBS += -L../lib -lzlib

INCLUDEPATH += $$PWD

HEADERS  += \
    $$PWD/crypt.h \
    $$PWD/ioapi.h \
    $$PWD/quazip.h \
    $$PWD/quazipfile.h \
    $$PWD/quazipfileinfo.h \
    $$PWD/quazipnewinfo.h \
    $$PWD/unzip.h \
    $$PWD/zconf.h \
    $$PWD/zip.h \
    $$PWD/gojobzip.h \
    $$PWD/zlib.h


SOURCES += \
    $$PWD/ioapi.c \
    $$PWD/quazip.cpp \
    $$PWD/quazipfile.cpp \
    $$PWD/quazipnewinfo.cpp \
    $$PWD/gojobzip.cpp \
    $$PWD/unzip.c \
    $$PWD/zip.c
