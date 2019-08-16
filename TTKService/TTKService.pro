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

TEMPLATE = app

contains(CONFIG, TTK_BUILD_LIB){
    CONFIG -= TTK_BUILD_LIB
}

CONFIG += TTK_NO_MSVC_LINK_NEED
win32{
    msvc{
        CONFIG -= TTK_NO_MSVC_LINK_NEED
    }
}
include(../TTKMusicPlayer.pri)
unix:VERSION += $$TTKMusicPlayer

win32{
    TARGET = ../../bin/$$TTKMusicPlayer/TTKService
    LIBS += -L../bin/$$TTKMusicPlayer -lTTKCore -lTTKDumper
}
unix{
    TARGET = ../lib/$$TTKMusicPlayer/TTKService
    LIBS += -L./lib/$$TTKMusicPlayer -lTTKCore -lTTKDumper
}


INCLUDEPATH += ../TTKModule

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    ../TTKModule/musicapplication.h \
    ../TTKModule/musicleftareawidget.h \
    ../TTKModule/musictopareawidget.h \
    ../TTKModule/musicrightareawidget.h \
    ../TTKModule/musicbottomareawidget.h \
    ../TTKModule/musicapplicationobject.h

}

SOURCES += \
    musicservicemain.cpp

win32{
    RC_FILE = TTKService.rc
}
