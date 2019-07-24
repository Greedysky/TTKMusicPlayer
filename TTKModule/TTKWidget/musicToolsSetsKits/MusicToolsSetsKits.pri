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
    $$PWD/musicmobiledeviceswidget.h \
    $$PWD/musicaudiorecorderwidget.h \
    $$PWD/musictimerwidget.h \
    $$PWD/musiclocalsongsmanagerwidget.h \
    $$PWD/musictransformwidget.h \
    $$PWD/musicfileinformationwidget.h \
    $$PWD/musicdesktopwallpaperwidget.h \
    $$PWD/musiclocalsongstablewidget.h \
    $$PWD/musicvolumegainwidget.h \
    $$PWD/musicsoundtouchwidget.h \
    $$PWD/musicsongringtonemakerwidget.h \
    $$PWD/musicidentifysongswidget.h \
    $$PWD/musicnetworkconnectiontestwidget.h \
    $$PWD/musicsongchecktoolswidget.h \
    $$PWD/musicsongchecktoolstablewidget.h \
    $$PWD/musicsoundkmicrowidget.h \
    $$PWD/musicsoundkmicrosearchwidget.h \
    $$PWD/musicsoundkmicrosettingpopwidget.h \
    $$PWD/musicspectrumwidget.h

}

contains(CONFIG, TTK_BUILD_LIB){
SOURCES += \
    $$PWD/musicmobiledeviceswidget.cpp \
    $$PWD/musicaudiorecorderwidget.cpp \
    $$PWD/musictimerwidget.cpp \
    $$PWD/musiclocalsongsmanagerwidget.cpp \
    $$PWD/musictransformwidget.cpp \
    $$PWD/musicfileinformationwidget.cpp \
    $$PWD/musicdesktopwallpaperwidget.cpp \
    $$PWD/musiclocalsongstablewidget.cpp \
    $$PWD/musicvolumegainwidget.cpp \
    $$PWD/musicsoundtouchwidget.cpp \
    $$PWD/musicsongringtonemakerwidget.cpp \
    $$PWD/musicidentifysongswidget.cpp \
    $$PWD/musicnetworkconnectiontestwidget.cpp \
    $$PWD/musicsongchecktoolswidget.cpp \
    $$PWD/musicsongchecktoolstablewidget.cpp \
    $$PWD/musicsoundkmicrowidget.cpp \
    $$PWD/musicsoundkmicrosearchwidget.cpp \
    $$PWD/musicsoundkmicrosettingpopwidget.cpp \
    $$PWD/musicspectrumwidget.cpp

}
