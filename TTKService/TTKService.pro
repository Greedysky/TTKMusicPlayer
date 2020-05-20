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

TEMPLATE = app

contains(CONFIG, TTK_BUILD_LIB){
    CONFIG -= TTK_BUILD_LIB
}

CONFIG += TTK_NO_MSVC_LINK_NEED
win32:msvc{
    CONFIG -= TTK_NO_MSVC_LINK_NEED
}

include($$PWD/../TTKMusicPlayer.pri)
unix:VERSION += $$TTKMusicPlayer

TARGET = TTKService

LIBS += -L$$DESTDIR -lTTKCore -lTTKConfig

INCLUDEPATH += \
    $$PWD/../TTKModule \
    $$PWD/../TTKRun \
    $$PWD/../TTKConfig

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/../TTKModule/musicapplication.h \
    $$PWD/../TTKModule/musicleftareawidget.h \
    $$PWD/../TTKModule/musictopareawidget.h \
    $$PWD/../TTKModule/musicrightareawidget.h \
    $$PWD/../TTKModule/musicbottomareawidget.h \
    $$PWD/../TTKModule/musicapplicationobject.h

}

SOURCES += \
    musicservicemain.cpp

win32{
    RC_FILE = TTKService.rc
}
