# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2025 Greedysky Studio
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
# ***************************************************************************

TEMPLATE = lib
CONFIG += warn_off plugin lib

include($$PWD/../../../TTKVersion.pri)

DESTDIR = $$OUT_PWD/../../../bin/$$TTK_VERSION
TARGET = zlib

DEFINES += ZLIB_DLL

HEADERS += \
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

win32:RC_FILE = $$PWD/$${TARGET}.rc
