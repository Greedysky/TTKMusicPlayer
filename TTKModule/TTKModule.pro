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

TEMPLATE = lib

include($$PWD/TTKModule.pri)
include($$PWD/../TTKMusicPlayer.pri)
CONFIG += plugin lib


##qmmp lib check
include($$PWD/../TTKExtra/qmmp.pri)
win32:{
    QMMP_DEPANDS = $$DESTDIR/qmmp1.dll
    QMMP_DEPANDS = $$replace(QMMP_DEPANDS, /, \\)
}
unix:!mac{
    QMMP_DEPANDS = $$DESTDIR/libqmmp.so
}
!exists($$QMMP_DEPANDS): error("Could not find qmmp library, please download and put it to output dir")


TARGET = TTKCore

INCLUDEPATH += $$PWD

SOURCES += \
    musicapplication.cpp \
    musicleftareawidget.cpp \
    musictopareawidget.cpp \
    musicrightareawidget.cpp \
    musicbottomareawidget.cpp \
    musicapplicationobject.cpp


HEADERS  += \
    musicapplication.h \
    musicleftareawidget.h \
    musictopareawidget.h \
    musicrightareawidget.h \
    musicbottomareawidget.h \
    musicapplicationobject.h
    
win32{
    RC_FILE = TTKCore.rc
}
