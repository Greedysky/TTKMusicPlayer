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

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4){ #Qt5
    QT += widgets
}

TEMPLATE = app

include($$PWD/../../../TTKVersion.pri)

DESTDIR = $$OUT_PWD/../../../bin/$$TTK_VERSION
TARGET = GenerateSkin

DEFINES += TTK_LIBRARY

win32:msvc{
    CONFIG += c++11
}else{
    equals(QT_MAJOR_VERSION, 6){ #Qt6
        QMAKE_CXXFLAGS += -std=c++17
    }else{
        QMAKE_CXXFLAGS += -std=c++11
    }
}

LIBS += -L$$DESTDIR -lTTKCore
unix:LIBS += -L$$DESTDIR -lTTKqmmp -lTTKLibrary -lTTKUi -lTTKExtras -lTTKWatcher -lttkzip -lzlib

INCLUDEPATH += \
    $$PWD/../../../TTKCommon \
    $$PWD/../../../TTKCommon/base \
    $$PWD/../../../TTKCommon/TTKLibrary \
    $$PWD/../../../TTKModule/TTKCore/musicCoreKits \
    $$PWD/../../../TTKModule/TTKCore/musicUtilsKits \
    $$PWD/../../../TTKModule/TTKWidget/musicCoreKits \
    $$PWD/../../../TTKModule/TTKWidget/musicUiKits

HEADERS += $$PWD/toolskin.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/toolskin.cpp

FORMS += $$PWD/toolskin.ui

win32:RC_FILE = $$PWD/$${TARGET}.rc
