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

INCLUDEPATH += \
    $$PWD \
    $$PWD/base \
    $$PWD/TTKLibrary \
    $$PWD/TTKDumper

win32{
    QT += xml network
    msvc{
        HEADERS += \
            $$PWD/TTKLibrary/ttkabstractbufferinterface.h \
            $$PWD/TTKLibrary/ttkabstractmovedialog.h \
            $$PWD/TTKLibrary/ttkabstractmoveresizewidget.h \
            $$PWD/TTKLibrary/ttkabstractmovewidget.h \
            $$PWD/TTKLibrary/ttkabstractnetwork.h \
            $$PWD/TTKLibrary/ttkabstractresizeinterface.h \
            $$PWD/TTKLibrary/ttkabstracttablewidget.h \
            $$PWD/TTKLibrary/ttkabstractthread.h \
            $$PWD/TTKLibrary/ttkabstractxml.h \
            $$PWD/TTKLibrary/ttkany.h \
            $$PWD/TTKLibrary/ttkclickedgroup.h \
            $$PWD/TTKLibrary/ttkclickedlabel.h \
            $$PWD/TTKLibrary/ttkclickedslider.h \
            $$PWD/TTKLibrary/ttkcommandline.h \
            $$PWD/TTKLibrary/ttkconcurrent.h \
            $$PWD/TTKLibrary/ttkconcurrentqueue.h \
            $$PWD/TTKLibrary/ttkcryptographichash.h \
            $$PWD/TTKLibrary/ttkdesktopscreen.h \
            $$PWD/TTKLibrary/ttkdefer.h \
            $$PWD/TTKLibrary/ttkdispatchmanager.h \
            $$PWD/TTKLibrary/ttkfileassociation.h \
            $$PWD/TTKLibrary/ttkfileinterface.h \
            $$PWD/TTKLibrary/ttkfunctor.h \
            $$PWD/TTKLibrary/ttkglobalinterface.h \
            $$PWD/TTKLibrary/ttkitemdelegate.h \
            $$PWD/TTKLibrary/ttklibrary.h \
            $$PWD/TTKLibrary/ttklibraryversion.h \
            $$PWD/TTKLibrary/ttklogoutput.h \
            $$PWD/TTKLibrary/ttkplatformsystem.h \
            $$PWD/TTKLibrary/ttksemaphoreloop.h \
            $$PWD/TTKLibrary/ttksmartpointer.h \
            $$PWD/TTKLibrary/ttksmartptr.h \
            $$PWD/TTKLibrary/ttkspinlock.h \
            $$PWD/TTKLibrary/ttksuperenum.h \
            $$PWD/TTKLibrary/ttktabbutton.h \
            $$PWD/TTKLibrary/ttkthemelinelabel.h \
            $$PWD/TTKLibrary/ttktime.h \
            $$PWD/TTKLibrary/ttktoastlabel.h \
            $$PWD/TTKLibrary/ttkunsortedmap.h \
            $$PWD/TTKLibrary/ttkvariant.h
    }
}
