# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2020 Greedysky Studio
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

QT       += core gui xml sql

equals(QT_MAJOR_VERSION, 4){
QT       += network
CONFIG   += gcc
}
equals(QT_MAJOR_VERSION, 5){
QT       += widgets multimediawidgets
}

include($$PWD/TTKVersion.pri)
DESTDIR = $$OUT_PWD/../bin/$$TTKMusicPlayer

##openssl lib check
win32:{
    SSL_DEPANDS = $$DESTDIR/ssleay32.dll
    SSL_DEPANDS = $$replace(SSL_DEPANDS, /, \\)
#    exists($$SSL_DEPANDS):LIBS += -L$$DESTDIR -lssl
}
unix:!mac{
    SSL_DEPANDS = $$DESTDIR/libssleay32.so
    exists($$SSL_DEPANDS):LIBS += -L$$DESTDIR -lssl
}

win32{
    LIBS += -lIphlpapi -lVersion -lole32 -luuid
    equals(QT_MAJOR_VERSION, 5){
        greaterThan(QT_MINOR_VERSION, 1):QT  += winextras
        msvc{
            LIBS += -L$$DESTDIR -lqmmp1 -lTTKUi -lTTKExtras -lTTKWatcher -lTTKDumper -lzlib -lTTKZip -luser32
            CONFIG +=c++11
            !contains(QMAKE_TARGET.arch, x86_64){
                 #support on windows XP
                 QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
                 QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01
            }
        }

        gcc{
            QMAKE_CXXFLAGS += -std=c++11 -Wunused-function -Wunused-result -Wswitch
            LIBS += -L$$DESTDIR -lqmmp1 -lTTKUi -lTTKExtras -lTTKWatcher -lTTKDumper -lzlib -lTTKZip
        }
    }

    equals(QT_MAJOR_VERSION, 4){
        QT  += multimedia
        gcc{
            QMAKE_CXXFLAGS += -std=c++11 -Wunused-function -Wunused-result -Wswitch
            LIBS += -L$$DESTDIR -lqmmp1 -lTTKUi -lTTKExtras -lTTKWatcher -lTTKDumper -lzlib -lTTKZip
        }
    }
}

unix:!mac{
    equals(QT_MAJOR_VERSION, 4){
        QMAKE_CXXFLAGS += -I/usr/include/QtMultimediaKit \
                          -I/usr/include/QtMobility
        LIBS += -lQtMultimediaKit
    }

    QMAKE_CXXFLAGS += -std=c++11 -Wunused-function -Wunused-result -Wswitch
    LIBS += -L$$DESTDIR -lqmmp -lTTKUi -lTTKExtras -lTTKWatcher -lTTKDumper -lzlib -lTTKZip
}

DEFINES += TTK_LIBRARY QMMP_LIBRARY

#########################################
HEADERS += $$PWD/musicglobal.h
INCLUDEPATH += $$PWD
#########################################
include($$PWD/TTKThirdParty/TTKThirdParty.pri)
