# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2017 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version MUSIC_WEBENGINE3 of the License, or
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

#brower type defined
Brower = 0

equals(QT_MAJOR_VERSION, 5){
    win32{
        QT += axcontainer
        DEFINES += MUSIC_WEBKIT
        Brower = 1
    }

    unix:!mac{
        exists($$[QT_INSTALL_LIBS]/libQt5WebKit.so){
            QT += webkit webkitwidgets
            DEFINES += MUSIC_WEBKIT
            Brower = 2
        }

        #5.6 or later use webengine
        greaterThan(QT_MINOR_VERSION, 5){
            exists($$[QT_INSTALL_LIBS]/libQt5WebEngine.so){
                QT += webenginewidgets
                DEFINES += MUSIC_WEBENGINE
                Brower = 3
            }
        }
    }
}
else{
    DEFINES += MUSIC_WEBKIT
    win32{
        CONFIG += qaxcontainer
        Brower = 1
    }
    else{
        QT += webkit webkitwidgets
        Brower = 2
    }
}

equals(Brower, 0): message("Not found Qt web component, build in KuGou by no webkit")
equals(Brower, 1): message("Found Qt web component, build in KuGou by Qt axcontainer")
equals(Brower, 2): message("Found Qt web component, build in KuGou by Qt webkit")
equals(Brower, 3): message("Found Qt web component, build in KuGou by Qt webengine")

INCLUDEPATH += $$PWD

HEADERS  += \
    $$PWD/kugouurl.h \
    $$PWD/kugouwindow.h \
    $$PWD/kugouuiobject.h

SOURCES += \
    $$PWD/kugouurl.cpp \
    $$PWD/kugouwindow.cpp
