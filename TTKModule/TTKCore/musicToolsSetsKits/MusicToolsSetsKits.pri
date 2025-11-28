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

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/musicdesktopwallpaperthread.h \
    $$PWD/musicbackupmodule.h \
    $$PWD/musictimerautomodule.h \
    $$PWD/musicsongsmanagerthread.h \
    $$PWD/musicnetworktestthread.h \
    $$PWD/musicsongchecktoolsthread.h \
    $$PWD/musicsongchecktoolsunit.h

SOURCES += \
    $$PWD/musicdesktopwallpaperthread.cpp \
    $$PWD/musicbackupmodule.cpp \
    $$PWD/musictimerautomodule.cpp \
    $$PWD/musicsongsmanagerthread.cpp \
    $$PWD/musicnetworktestthread.cpp \
    $$PWD/musicsongchecktoolsthread.cpp


equals(QT_MAJOR_VERSION, 6):lessThan(QT_MINOR_VERSION, 2){
    message("Qt6.0.x and 6.1.x not support multimedia")
}else{
    HEADERS += $$PWD/musicaudiorecordermodule.h
    SOURCES += $$PWD/musicaudiorecordermodule.cpp
}
