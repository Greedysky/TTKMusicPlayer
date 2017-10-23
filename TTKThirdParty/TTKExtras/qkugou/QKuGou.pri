# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2017 Greedysky Studio
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

equals(QT_MAJOR_VERSION, 5){
    win32{
        QT += axcontainer
        DEFINES += MUSIC_WEBKIT
    }

    unix:!mac{
        exists($$[QT_INSTALL_LIBS]/libQt5WebKit.so){
            QT += webkit webkitwidgets
            DEFINES += MUSIC_WEBKIT
        }
    }
}
else{
    DEFINES += MUSIC_WEBKIT
    win32{
        CONFIG += qaxcontainer
    }
    else{
        QT += webkit webkitwidgets
    }
}

win32{
    message("Webview build in KuGou by QAxContainer")
}
unix:!mac{
    contains(QT, webkit){
        message("Found Qt webkit component, build in KuGou by Qt webkit")
    }
    else{
        message("Not found Qt webkit component, build in KuGou by no webkit")
    }
}

INCLUDEPATH += $$PWD

HEADERS  += \
    $$PWD/kugouurl.h \
    $$PWD/kugouwindow.h \
    $$PWD/kugouuiobject.h

SOURCES += \
    $$PWD/kugouurl.cpp \
    $$PWD/kugouwindow.cpp
