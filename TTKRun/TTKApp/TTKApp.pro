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

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include($$PWD/../../TTKVersion.pri)

TARGET = TTKMusicPlayer

TEMPLATE = app

win32:msvc{
    CONFIG +=c++11
}else{
    QMAKE_CXXFLAGS += -std=c++11
}

win32{
    DESTDIR = $$OUT_PWD/../../bin/$$TTKMusicPlayer
    LIBS += -L$$DESTDIR -lTTKConfig
}
else{
    DESTDIR = $$OUT_PWD/../../bin
    DEFINES += CONFIG_OUT_BUILD
    SOURCES += $$PWD/../../TTKConfig/musicconfigobject.cpp
    HEADERS += \
        $$PWD/../../TTKConfig/musicconfigobject.h \
        $$PWD/../../TTKConfig/musicconfigdefine.h
}

INCLUDEPATH += \
    $$PWD/../ \
    $$PWD/../../ \
    $$PWD/../../TTKConfig \
    $$PWD/../../TTKThirdParty/TTKDumper \
    $$PWD/../../TTKModule/TTKCore/musicCoreKits

SOURCES += \
    ttkrunmain.cpp \
    ttklocalpeer.cpp \
    ttkrunapplication.cpp \
    ttkrunobject.cpp

HEADERS += \
    ttkrunobject.h \
    ttklocalpeer.h \
    ttkrunapplication.h \

RESOURCES += \
    $$PWD/../../TTKUi/MusicApp.qrc

win32{
    RC_FILE = TTKApp.rc
}
