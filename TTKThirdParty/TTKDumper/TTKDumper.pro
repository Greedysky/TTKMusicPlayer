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

QT       += core

TEMPLATE = lib

include(../../TTKVersion.pri)

win32:TARGET = ../../../bin/$$TTKMusicPlayer/TTKDumper
unix:TARGET = ../../lib/$$TTKMusicPlayer/TTKDumper

CONFIG       += warn_off
unix:VERSION += 1.0.0

INCLUDEPATH += $$PWD/../../TTKModule/TTKCore/musicCoreKits

win32{
    LIBS += -lpsapi
}

win32:msvc{
    CONFIG +=c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
}

SOURCES += \
    $$PWD/minidumper.cpp \
    $$PWD/ttkdumper.cpp
    
HEADERS += \
    $$PWD/mini.h \
    $$PWD/minidumper.h \
    $$PWD/ttkdumper.h

#load extra define
include(../TTKExtrasDefine.pri)

win32{
    RC_FILE = TTKDumper.rc
}
