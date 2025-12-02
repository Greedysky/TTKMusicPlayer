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

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4){ #Qt5
    QT += widgets
}

TEMPLATE = lib
CONFIG += plugin lib

include($$PWD/../../TTKVersion.pri)

DESTDIR = $$OUT_PWD/../../bin/$$TTK_VERSION
TARGET = TTKExtras

DEFINES += TTK_LIBRARY

win32:msvc{
    LIBS += -luser32
    CONFIG += c++11
}else{
    equals(QT_MAJOR_VERSION, 6){ #Qt6
        QMAKE_CXXFLAGS += -std=c++17
    }else{
        QMAKE_CXXFLAGS += -std=c++11
    }
}

LIBS += -L$$DESTDIR -lTTKLibrary -lTTKUi

#load extra define
include($$PWD/../TTKThirdParty.pri)

include($$PWD/qalgorithm/QAlgorithm.pri)
include($$PWD/qdlna/QDlna.pri)
include($$PWD/qhttpserver/QHttpServer.pri)
include($$PWD/qsync/QSync.pri)
include($$PWD/qjson/QJson.pri)
include($$PWD/qkugou/QKuGou.pri)
include($$PWD/qrencode/QRencode.pri)
include($$PWD/qshortcut/QShortCut.pri)

win32:RC_FILE = $$PWD/$${TARGET}.rc
