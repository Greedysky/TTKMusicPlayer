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

INCLUDEPATH += $$PWD

!contains(CONFIG, TTK_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musiclrcmanager.h \
    $$PWD/musiclrcmanagerfordesktop.h \
    $$PWD/musiclrcmanagerforinline.h \
    $$PWD/musiclrccontainer.h \
    $$PWD/musiclrccontainerfordesktop.h \
    $$PWD/musiclrccontainerforinline.h \
    $$PWD/musiclrcsearchwidget.h \
    $$PWD/musiclrcsearchtablewidget.h \
    $$PWD/musiclrcartphotouploadwidget.h \
    $$PWD/musiclrcfloatwidget.h \
    $$PWD/musiclrcfloatsettingwidget.h \
    $$PWD/musiclrcfloatphotowidget.h \
    $$PWD/musiclrcfloatplaywidget.h \
    $$PWD/musiclrcmakerwidget.h \
    $$PWD/musiclrcerrorwidget.h \
    $$PWD/musiclrclocallinkwidget.h \
    $$PWD/musiclrccolorwidget.h \
    $$PWD/musiclrccommentswidget.h \
    $$PWD/musiclrctranslatedwidget.h \
    $$PWD/musiclrccontainerforwallpaper.h \
    $$PWD/musiclrcposterwidget.h \
    $$PWD/musiclrcdownloadbatchwidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musiclrcmanager.cpp \
    $$PWD/musiclrcmanagerfordesktop.cpp \
    $$PWD/musiclrcmanagerforinline.cpp \
    $$PWD/musiclrccontainer.cpp \
    $$PWD/musiclrccontainerfordesktop.cpp \
    $$PWD/musiclrccontainerforinline.cpp \
    $$PWD/musiclrcsearchwidget.cpp \
    $$PWD/musiclrcsearchtablewidget.cpp \
    $$PWD/musiclrcartphotouploadwidget.cpp \
    $$PWD/musiclrcfloatwidget.cpp \
    $$PWD/musiclrcfloatsettingwidget.cpp \
    $$PWD/musiclrcfloatphotowidget.cpp \
    $$PWD/musiclrcfloatplaywidget.cpp \
    $$PWD/musiclrcmakerwidget.cpp \
    $$PWD/musiclrcerrorwidget.cpp \
    $$PWD/musiclrclocallinkwidget.cpp \
    $$PWD/musiclrccolorwidget.cpp \
    $$PWD/musiclrccommentswidget.cpp \
    $$PWD/musiclrctranslatedwidget.cpp \
    $$PWD/musiclrccontainerforwallpaper.cpp \
    $$PWD/musiclrcposterwidget.cpp \
    $$PWD/musiclrcdownloadbatchwidget.cpp

}
