# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2019 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
#
# * You should have received a copy of the GNU General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
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
    HEADERS  += $$PWD/iowin32.h
    SOURCES += $$PWD/iowin32.c
}
unix:!mac{
    LIBS += -L../../../lib/$$TTKMusicPlayer -lzlib
}

HEADERS  += \
    $$PWD/zip.h \
    $$PWD/unzip.h \
    $$PWD/mztools.h \
    $$PWD/ioapi.h \
    $$PWD/crypt.h

SOURCES += \
    $$PWD/zip.c \
    $$PWD/unzip.c \
    $$PWD/mztools.c \
    $$PWD/ioapi.c
    
win32{
    RC_FILE = ttkzip.rc
}
