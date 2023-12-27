# ***************************************************************************
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2024 Greedysky Studio
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

#browser type defined
Browser = 0

equals(QT_MAJOR_VERSION, 4){
    DEFINES += TTK_WEBKIT
    win32{
        CONFIG += qaxcontainer
        Browser = 1
    }else{
        QT += webkit webkitwidgets
        Browser = 2
    }
}
equals(QT_MAJOR_VERSION, 5){
    win32{
        QT += axcontainer
        DEFINES += TTK_WEBKIT
        Browser = 1
    }

    unix:!mac{
        exists($$[QT_INSTALL_LIBS]/libQt5WebKit.so){
            QT += webkit webkitwidgets
            DEFINES += TTK_WEBKIT
            Browser = 2
        }

        #5.6 or later use webengine
        greaterThan(QT_MINOR_VERSION, 5){
            exists($$[QT_INSTALL_LIBS]/libQt5WebEngine.so){
                QT += webenginewidgets
                DEFINES -= TTK_WEBKIT
                DEFINES += TTK_WEBENGINE
                Browser = 3
            }
        }
    }
}

equals(Browser, 0): message("Not found Qt web component, build in KuGou module by no webkit")
equals(Browser, 1): message("Found Qt web component, build in KuGou module by Qt axcontainer")
equals(Browser, 2): message("Found Qt web component, build in KuGou module by Qt webkit")
equals(Browser, 3): message("Found Qt web component, build in KuGou module by Qt webengine")

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qkugouurl.h \
    $$PWD/qkugouwindow.h \
    $$PWD/qkugouuiobject.h

SOURCES += \
    $$PWD/qkugouurl.cpp \
    $$PWD/qkugouwindow.cpp
