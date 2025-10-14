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

#browser type defined
TTK_WEB_MODULE = 0

win32{
    DEFINES += TTK_MINIBLINK
    TTK_WEB_MODULE = 1
}else{
    equals(QT_MAJOR_VERSION, 4){
        DEFINES += TTK_WEBKIT
        QT += webkit webkitwidgets
        TTK_WEB_MODULE = 2
    }

    equals(QT_MAJOR_VERSION, 5){
        unix:!mac{
            exists($$[QT_INSTALL_LIBS]/libQt5WebKit.so){
                QT += webkit webkitwidgets
                DEFINES += TTK_WEBKIT
                TTK_WEB_MODULE = 2
            }

            #5.6 or later use webengine
            greaterThan(QT_MINOR_VERSION, 5){
                exists($$[QT_INSTALL_LIBS]/libQt5WebEngine.so){
                    QT += webenginewidgets
                    DEFINES -= TTK_WEBKIT
                    DEFINES += TTK_WEBENGINE
                    TTK_WEB_MODULE = 3
                }
            }
        }

        mac{
            exists($$[QT_INSTALL_LIBS]/libQt5WebKit.dylib){
                QT += webkit webkitwidgets
                DEFINES += TTK_WEBKIT
                TTK_WEB_MODULE = 2
            }

            #5.6 or later use webengine
            greaterThan(QT_MINOR_VERSION, 5){
                exists($$[QT_INSTALL_LIBS]/libQt5WebEngine.dylib){
                    QT += webenginewidgets
                    DEFINES -= TTK_WEBKIT
                    DEFINES += TTK_WEBENGINE
                    TTK_WEB_MODULE = 3
                }
            }
        }
    }
}

equals(TTK_WEB_MODULE, 1): message("Found Qt web component, build in KuGou module by Qt miniblink")
equals(TTK_WEB_MODULE, 2): message("Found Qt web component, build in KuGou module by Qt webkit")
equals(TTK_WEB_MODULE, 3): message("Found Qt web component, build in KuGou module by Qt webengine")
equals(TTK_WEB_MODULE, 0): message("Not found Qt web component, build in KuGou module by none web component")

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qkugouurl.h \
    $$PWD/qkugouwindow.h \
    $$PWD/qkugouuiobject.h

SOURCES += \
    $$PWD/qkugouurl.cpp \
    $$PWD/qkugouwindow.cpp

win32:include($$PWD/miniblink/miniblink.pri)
