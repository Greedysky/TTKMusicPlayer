# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2021 Greedysky Studio
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

include($$PWD/../../TTKVersion.pri)

DESTDIR = $$OUT_PWD/../../bin/$$TTKMusicPlayer
TARGET = TTKDumper

CONFIG += plugin lib

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
    $$PWD/miniprocess.cpp \
    $$PWD/minidumper.cpp \
    $$PWD/ttkdumper.cpp
    
HEADERS += \
    $$PWD/miniprocess.h \
    $$PWD/minidumper.h \
    $$PWD/ttkdumper.h \
    $$PWD/ttklogger.h

#load extra define
include($$PWD/../TTKExtrasDefine.pri)

win32{
    RC_FILE = TTKDumper.rc
}
