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

unix:!mac{
    equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 0){
        QT += x11extras
    }
}

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qglobalshortcut.h \
    $$PWD/qglobalshortcut_p.h

win32{
    SOURCES += $$PWD/qglobalshortcut_win.cpp
    LIBS += -luser32
}
unix:SOURCES += $$PWD/qglobalshortcut_x11.cpp
mac:SOURCES += $$PWD/qglobalshortcut_mac.cpp

SOURCES += $$PWD/qglobalshortcut.cpp
