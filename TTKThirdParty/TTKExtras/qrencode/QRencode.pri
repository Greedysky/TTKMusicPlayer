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

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qrcodewidget.h \
    $$PWD/bitstream.h \
    $$PWD/config.h \
    $$PWD/mask.h \
    $$PWD/mmask.h \
    $$PWD/mqrspec.h \
    $$PWD/qrencode_inner.h \
    $$PWD/qrencode.h \
    $$PWD/qrinput.h \
    $$PWD/qrspec.h \
    $$PWD/rscode.h \
    $$PWD/rsecc.h \
    $$PWD/split.h

SOURCES += \
    $$PWD/qrcodewidget.cpp \
    $$PWD/bitstream.c \
    $$PWD/mask.c \
    $$PWD/mmask.c \
    $$PWD/mqrspec.c \
    $$PWD/qrencode.c \
    $$PWD/qrinput.c \
    $$PWD/qrspec.c \
    $$PWD/rscode.c \
    $$PWD/rsecc.c \
    $$PWD/split.c
