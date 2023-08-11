# ***************************************************************************
# * This file is part of the TTK Library Module project
# * Copyright (C) 2015 - 2023 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU Lesser General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU Lesser General Public License for more details.
#
# * You should have received a copy of the GNU Lesser General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
# ***************************************************************************

QT += xml
greaterThan(QT_MAJOR_VERSION, 4){ #Qt5
    QT += widgets
}

TEMPLATE = lib
CONFIG += plugin lib

include($$PWD/../../TTKVersion.pri)

DESTDIR = $$OUT_PWD/../../bin/$$TTK_VERSION
TARGET = TTKLibrary

DEFINES += TTK_LIBRARY

win32:msvc{
    CONFIG += c++11
    LIBS += -luser32
}else{
    equals(QT_MAJOR_VERSION, 6){ #Qt6
        QMAKE_CXXFLAGS += -std=c++17
    }else{
        QMAKE_CXXFLAGS += -std=c++11
    }
}

INCLUDEPATH += $$PWD/../

HEADERS += \
    $$PWD/../ttkglobal.h \
    $$PWD/../ttklogger.h \
    $$PWD/../ttkmoduleexport.h \
    $$PWD/../ttknumberdefine.h \
    $$PWD/../ttkobject.h \
    $$PWD/../ttkprivate.h \
    $$PWD/../ttkqtcompat.h \
    $$PWD/../ttkqtglobal.h \
    $$PWD/../ttkqtobject.h \
    $$PWD/../ttksingleton.h \
    $$PWD/../ttkversion.h

HEADERS += \
    $$PWD/ttkabstractmovedialog.h \
    $$PWD/ttkabstractmoveresizewidget.h \
    $$PWD/ttkabstractmovewidget.h \
    $$PWD/ttkabstractresizeinterface.h \
    $$PWD/ttkabstractthread.h \
    $$PWD/ttkabstractxml.h \
    $$PWD/ttkany.h \
    $$PWD/ttkclickedgroup.h \
    $$PWD/ttkclickedlabel.h \
    $$PWD/ttkclickedslider.h \
    $$PWD/ttkcommandline.h \
    $$PWD/ttkconcurrentqueue.h \
    $$PWD/ttkcryptographichash.h \
    $$PWD/ttkdefer.h \
    $$PWD/ttkdesktopwrapper.h \
    $$PWD/ttkfileassocation.h \
    $$PWD/ttkglobalhelper.h \
    $$PWD/ttkitemdelegate.h \
    $$PWD/ttklibrary.h \
    $$PWD/ttklibraryversion.h \
    $$PWD/ttkplatformsystem.h \
    $$PWD/ttksemaphoreloop.h \
    $$PWD/ttksuperenum.h \
    $$PWD/ttktabbutton.h \
    $$PWD/ttkthemelinelabel.h \
    $$PWD/ttktime.h \
    $$PWD/ttktoastlabel.h

SOURCES += \
    $$PWD/ttkabstractmovedialog.cpp \
    $$PWD/ttkabstractmoveresizewidget.cpp \
    $$PWD/ttkabstractmovewidget.cpp \
    $$PWD/ttkabstractthread.cpp \
    $$PWD/ttkabstractxml.cpp \
    $$PWD/ttkany.cpp \
    $$PWD/ttkclickedgroup.cpp \
    $$PWD/ttkclickedlabel.cpp \
    $$PWD/ttkclickedslider.cpp \
    $$PWD/ttkcommandline.cpp \
    $$PWD/ttkcryptographichash.cpp \
    $$PWD/ttkdesktopwrapper.cpp \
    $$PWD/ttkfileassocation.cpp \
    $$PWD/ttkglobalhelper.cpp \
    $$PWD/ttkitemdelegate.cpp \
    $$PWD/ttkplatformsystem.cpp \
    $$PWD/ttksemaphoreloop.cpp \
    $$PWD/ttksuperenum.cpp \
    $$PWD/ttktabbutton.cpp \
    $$PWD/ttkthemelinelabel.cpp \
    $$PWD/ttktime.cpp \
    $$PWD/ttktoastlabel.cpp

RESOURCES += $$PWD/TTKLibrary.qrc

win32:RC_FILE = $$PWD/TTKLibrary.rc
