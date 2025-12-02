# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2026 Greedysky Studio
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
    $$PWD/aes.h \
    $$PWD/aes_locl.h \
    $$PWD/cbc128.h \
    $$PWD/aeswrapper.h \
    $$PWD/deswrapper.h \
    $$PWD/imagewrapper.h

SOURCES += \
    $$PWD/aes_ecb.cpp \
    $$PWD/aes_cbc.cpp \
    $$PWD/aes_core.cpp \
    $$PWD/cbc128.cpp \
    $$PWD/aeswrapper.cpp \
    $$PWD/deswrapper.cpp \
    $$PWD/imagewrapper.cpp
