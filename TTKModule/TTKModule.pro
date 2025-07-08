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
CONFIG += plugin lib

include($$PWD/TTKModule.pri)
include($$PWD/../TTKMusicPlayer.pri)

##qmmp lib check
win32{
    QMMP_DEPANDS = $$DESTDIR/TTKqmmp.dll
    QMMP_DEPANDS = $$replace(QMMP_DEPANDS, /, \\)
}
unix:!mac:QMMP_DEPANDS = $$DESTDIR/libTTKqmmp.so
mac:QMMP_DEPANDS = $$DESTDIR/libTTKqmmp.dylib
!exists($$QMMP_DEPANDS): error("Could not find ttk qmmp library, please download and put it to output dir")

TARGET = TTKCore

INCLUDEPATH += \
    $$PWD \
    $$PWD/../TTKThirdParty/TTKqmmp

HEADERS += \
    $$PWD/musicapplication.h \
    $$PWD/musicapplicationmodule.h \
    $$PWD/musicleftareawidget.h \
    $$PWD/musictopareawidget.h \
    $$PWD/musicrightareawidget.h \
    $$PWD/musicbottomareawidget.h

SOURCES += \
    $$PWD/musicapplication.cpp \
    $$PWD/musicapplicationmodule.cpp \
    $$PWD/musicleftareawidget.cpp \
    $$PWD/musictopareawidget.cpp \
    $$PWD/musicrightareawidget.cpp \
    $$PWD/musicbottomareawidget.cpp

win32:RC_FILE = $$PWD/$${TARGET}.rc
