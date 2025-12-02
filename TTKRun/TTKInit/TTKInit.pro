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

QT += core

TEMPLATE = app
CONFIG += console

include($$PWD/../../TTKVersion.pri)

DESTDIR = $$OUT_PWD/../../bin/$$TTK_VERSION
TARGET = TTKInit

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

win32{
    msvc{
        HEADERS += $$PWD/../../TTKConfig/musicconfigmodule.h
    }else{
        QMAKE_LFLAGS_CONSOLE += -mwindows
    }
}

LIBS += -L$$DESTDIR -lTTKConfig

INCLUDEPATH += \
    $$PWD/../../TTKConfig \
    $$PWD/../../TTKCommon \
    $$PWD/../../TTKCommon/base \
    $$PWD/../../TTKCommon/TTKLibrary \
    $$PWD/../../TTKModule/TTKCore/musicCoreKits

SOURCES += $$PWD/musicinitmain.cpp

win32:RC_FILE = $$PWD/$${TARGET}.rc
