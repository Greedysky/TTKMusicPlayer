# ***************************************************************************
# * This file is part of the TTK Library Module project
# * Copyright (C) 2015 - 2025 Greedysky Studio
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

QT += xml network
greaterThan(QT_MAJOR_VERSION, 4){ #Qt5
    QT += widgets
}

TEMPLATE = lib
CONFIG += plugin lib

include($$PWD/../../TTKVersion.pri)

DESTDIR = $$OUT_PWD/../../bin/$$TTK_VERSION
TARGET = TTKLibrary

DEFINES += TTK_LIBRARY

win32:LIBS += -lgdi32
win32:msvc{
    CONFIG += c++11
    LIBS += -luser32
}else{
    equals(QT_MAJOR_VERSION, 6){ #Qt6
        QT += core5compat
        QMAKE_CXXFLAGS += -std=c++17
    }else{
        QMAKE_CXXFLAGS += -std=c++11
    }
}

INCLUDEPATH += \
    $$PWD/../ \
    $$PWD/../base

HEADERS += \
    $$PWD/../base/ttkcompat.h \
    $$PWD/../base/ttkglobal.h \
    $$PWD/../base/ttklogger.h \
    $$PWD/../base/ttkmoduleexport.h \
    $$PWD/../base/ttkprivate.h \
    $$PWD/../base/ttkqtcompat.h \
    $$PWD/../base/ttkqtglobal.h \
    $$PWD/../base/ttkqtobject.h \
    $$PWD/../base/ttksingleton.h \
    $$PWD/../ttkobject.h \
    $$PWD/../ttkversion.h

HEADERS += \
    $$PWD/ttkabstractbufferinterface.h \
    $$PWD/ttkabstractmovedialog.h \
    $$PWD/ttkabstractmoveresizewidget.h \
    $$PWD/ttkabstractmovewidget.h \
    $$PWD/ttkabstractnetwork.h \
    $$PWD/ttkabstractresizeinterface.h \
    $$PWD/ttkabstracttablewidget.h \
    $$PWD/ttkabstractthread.h \
    $$PWD/ttkabstractxml.h \
    $$PWD/ttkany.h \
    $$PWD/ttkclickedgroup.h \
    $$PWD/ttkclickedlabel.h \
    $$PWD/ttkclickedslider.h \
    $$PWD/ttkcommandline.h \
    $$PWD/ttkconcurrent.h \
    $$PWD/ttkconcurrentqueue.h \
    $$PWD/ttkcryptographichash.h \
    $$PWD/ttkdefer.h \
    $$PWD/ttkdesktopscreen.h \
    $$PWD/ttkdispatchmanager.h \
    $$PWD/ttkfileassociation.h \
    $$PWD/ttkfileinterface.h \
    $$PWD/ttkfunctor.h \
    $$PWD/ttkglobalinterface.h \
    $$PWD/ttkitemdelegate.h \
    $$PWD/ttklibrary.h \
    $$PWD/ttklibraryversion.h \
    $$PWD/ttklogoutput.h \
    $$PWD/ttkplatformsystem.h \
    $$PWD/ttksemaphoreloop.h \
    $$PWD/ttksmartptr.h \
    $$PWD/ttkspinlock.h \
    $$PWD/ttksuperenum.h \
    $$PWD/ttktabbutton.h \
    $$PWD/ttkthemelinelabel.h \
    $$PWD/ttktime.h \
    $$PWD/ttktoastlabel.h \
    $$PWD/ttkunsortedmap.h \
    $$PWD/ttkvariant.h

SOURCES += \
    $$PWD/ttkabstractmovedialog.cpp \
    $$PWD/ttkabstractmoveresizewidget.cpp \
    $$PWD/ttkabstractmovewidget.cpp \
    $$PWD/ttkabstractnetwork.cpp \
    $$PWD/ttkabstracttablewidget.cpp \
    $$PWD/ttkabstractthread.cpp \
    $$PWD/ttkabstractxml.cpp \
    $$PWD/ttkany.cpp \
    $$PWD/ttkclickedgroup.cpp \
    $$PWD/ttkclickedlabel.cpp \
    $$PWD/ttkclickedslider.cpp \
    $$PWD/ttkcommandline.cpp \
    $$PWD/ttkcryptographichash.cpp \
    $$PWD/ttkdesktopscreen.cpp \
    $$PWD/ttkdispatchmanager.cpp \
    $$PWD/ttkfileassociation.cpp \
    $$PWD/ttkglobalinterface.cpp \
    $$PWD/ttkitemdelegate.cpp \
    $$PWD/ttklogoutput.cpp \
    $$PWD/ttkplatformsystem.cpp \
    $$PWD/ttksemaphoreloop.cpp \
    $$PWD/ttksuperenum.cpp \
    $$PWD/ttktabbutton.cpp \
    $$PWD/ttkthemelinelabel.cpp \
    $$PWD/ttktime.cpp \
    $$PWD/ttktoastlabel.cpp

RESOURCES += $$PWD/$${TARGET}.qrc

win32:RC_FILE = $$PWD/$${TARGET}.rc
