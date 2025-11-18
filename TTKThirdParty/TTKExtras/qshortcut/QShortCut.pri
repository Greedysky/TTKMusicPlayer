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

SOURCES += $$PWD/qglobalshortcut.cpp

win32{
    SOURCES += $$PWD/qglobalshortcut_win.cpp
    LIBS += -luser32
}
unix:!mac:SOURCES += $$PWD/qglobalshortcut_x11.cpp
mac{
    MACOS_VERSION = $$system(/usr/libexec/PlistBuddy -c "Print ProductVersion" /System/Library/CoreServices/SystemVersion.plist)
    message("Detected macOS version: $$MACOS_VERSION")

    MACOS_VERSION = $$split(MACOS_VERSION, ".")
    MAJOR_VERSION = $$member(MACOS_VERSION, 0)
    MINOR_VERSION = $$member(MACOS_VERSION, 1)

    # macOS version greater than 10.5 use qglobalshortcut_osx.cpp
    # see https://leopard-adc.pepas.com/documentation/Carbon/Reference/KeyboardLayoutServices/Reference/reference.html
    greaterThan(MAJOR_VERSION, 10){
        SOURCES += $$PWD/qglobalshortcut_osx.cpp
    }else{
        equals(MINOR_VERSION, 10){
            greaterThan(MINOR_VERSION, 4){
                SOURCES += $$PWD/qglobalshortcut_osx.cpp
            }else{
                SOURCES += $$PWD/qglobalshortcut_mac.cpp
            }
        }else{
            SOURCES += $$PWD/qglobalshortcut_mac.cpp
        }
    }

    # qglobalshortcut_mac.cpp needs this.
    # Notice, it cannot work on recent macOS versions,
    # so either additional implementation is needed, using
    # modern API, or at least a dummy fallback.
    LIBS += -framework Carbon
}
