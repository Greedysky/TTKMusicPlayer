# ***************************************************************************
# * This file is part of the TTK Library Module project
# * Copyright (C) 2015 - 2022 Greedysky Studio
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

INCLUDEPATH += \
    $$PWD \
    $$PWD/TTKLibrary \
    $$PWD/TTKDumper

win32{
    QT += xml
    msvc{
        HEADERS += \
            $$PWD/TTKLibrary/ttkabstractmovedialog.h \
            $$PWD/TTKLibrary/ttkabstractmoveresizewidget.h \
            $$PWD/TTKLibrary/ttkabstractmovewidget.h \
            $$PWD/TTKLibrary/ttkabstractresizeinterface.h \
            $$PWD/TTKLibrary/ttkabstractthread.h \
            $$PWD/TTKLibrary/ttkabstractxml.h \
            $$PWD/TTKLibrary/ttkclickedgroup.h \
            $$PWD/TTKLibrary/ttkclickedlabel.h \
            $$PWD/TTKLibrary/ttkclickedslider.h \
            $$PWD/TTKLibrary/ttkcommandline.h \
            $$PWD/TTKLibrary/ttkcryptographichash.h \
            $$PWD/TTKLibrary/ttkdesktopwrapper.h \
            $$PWD/TTKLibrary/ttkglobalhelper.h \
            $$PWD/TTKLibrary/ttklibrary.h \
            $$PWD/TTKLibrary/ttksemaphoreloop.h \
            $$PWD/TTKLibrary/ttksuperenum.h \
            $$PWD/TTKLibrary/ttktabbutton.h \
            $$PWD/TTKLibrary/ttktime.h \
            $$PWD/TTKLibrary/ttktoastlabel.h
    }
}
