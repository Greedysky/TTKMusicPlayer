# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2020 Greedysky Studio
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
include(../../TTKVersion.pri)
unix:VERSION += 1.0.0.0

TEMPLATE = app
CONFIG += console

UI_DIR = ./.build/ui
MOC_DIR = ./.build/moc
OBJECTS_DIR = ./.build/obj
RCC_DIR = ./.build/rcc

DEFINES += TTK_LIBRARY

win32:DESTDIR = $$OUT_PWD/../../bin/$$TTKMusicPlayer
unix:DESTDIR = $$OUT_PWD/../../lib/$$TTKMusicPlayer
TARGET = TTKConsole

win32:LIBS += -L$$DESTDIR -lTTKCore
unix:LIBS += -L$$DESTDIR -lTTKCore -lqmmp -lTTKUi -lTTKExtras -lTTKWatcher -lzlib -lTTKZip

win32:msvc{
    CONFIG +=c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
}

INCLUDEPATH += \
    $$PWD/../ \
    $$PWD/../../ \
    $$PWD/../../TTKThirdParty \
    $$PWD/../../TTKThirdParty/TTKDumper \
    $$PWD/../../TTKModule/TTKCore/musicCoreKits \
    $$PWD/../../TTKModule/TTKCore/musicPlaylistKits \
    $$PWD/../../TTKModule/TTKCore/musicUtilsKits

SOURCES += \
    musicconsolemain.cpp \
    musicconsoleobject.cpp

HEADERS += \
    ../musicrunglobaldefine.h \
    musicconsoleobject.h

win32{
    RC_FILE = TTKConsole.rc
}
