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

INCLUDEPATH += $$PWD

HEADERS  += \
    $$PWD/musicobject.h \
    $$PWD/musicformats.h \
    $$PWD/musicnumberdefine.h \
    $$PWD/musicglobaldefine.h \
    $$PWD/musicotherdefine.h \
    $$PWD/musictime.h \
    $$PWD/musicplayer.h \
    $$PWD/musicplaylist.h \
    $$PWD/musicabstractxml.h \
    $$PWD/musicabstractthread.h \
    $$PWD/musicsettingmanager.h \
    $$PWD/musicconnectionpool.h \
    $$PWD/musicplatformmanager.h \
    $$PWD/musicsingleton.h \
    $$PWD/musiccoremplayer.h \
    $$PWD/musicsong.h \
    $$PWD/musicsongmeta.h \
    $$PWD/musiccryptographichash.h \
    $$PWD/musicbackgroundmanager.h \
    $$PWD/musicsemaphoreloop.h \
    $$PWD/musiccategoryconfigmanager.h  \
    $$PWD/musicplaylistmanager.h \
    $$PWD/musichotkeymanager.h \
    $$PWD/musicruntimemanager.h \
    $$PWD/musicdispatchmanager.h \
    $$PWD/musicextractwrap.h \
    $$PWD/musicbackgroundconfigmanager.h \
    $$PWD/musicconfigmanager.h \
    $$PWD/musicsinglemanager.h


SOURCES += \
    $$PWD/musicformats.cpp \
    $$PWD/musictime.cpp \
    $$PWD/musicplayer.cpp \
    $$PWD/musicplaylist.cpp \
    $$PWD/musicabstractxml.cpp \
    $$PWD/musicabstractthread.cpp \
    $$PWD/musicconnectionpool.cpp \
    $$PWD/musicplatformmanager.cpp \
    $$PWD/musiccoremplayer.cpp \
    $$PWD/musicsingleton.cpp \
    $$PWD/musicsong.cpp \
    $$PWD/musicsongmeta.cpp \
    $$PWD/musiccryptographichash.cpp \
    $$PWD/musicbackgroundmanager.cpp \
    $$PWD/musicsemaphoreloop.cpp \
    $$PWD/musiccategoryconfigmanager.cpp \
    $$PWD/musicplaylistmanager.cpp \
    $$PWD/musichotkeymanager.cpp \
    $$PWD/musicruntimemanager.cpp \
    $$PWD/musicdispatchmanager.cpp \
    $$PWD/musicextractwrap.cpp \
    $$PWD/musicbackgroundconfigmanager.cpp \
    $$PWD/musicconfigmanager.cpp \
    $$PWD/musicsinglemanager.cpp
