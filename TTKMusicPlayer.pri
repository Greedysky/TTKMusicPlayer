# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2019 Greedysky Studio
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
include(TTKExtra/Qt4/qmmp.pri)
}
equals(QT_MAJOR_VERSION, 5){
QT       += widgets multimediawidgets
include(TTKExtra/Qt5/qmmp.pri)
}

UI_DIR = ./.build/ui
MOC_DIR = ./.build/moc
OBJECTS_DIR = ./.build/obj
RCC_DIR = ./.build/rcc

include(TTKVersion.pri)

##openssl lib check
win32:{
    SSL_DEPANDS = $$OUT_PWD/../bin/$$TTKMusicPlayer/ssleay32.dll
    SSL_DEPANDS = $$replace(SSL_DEPANDS, /, \\)
#    exists($$SSL_DEPANDS):LIBS += -L../bin/$$TTKMusicPlayer -lssl
}
unix:!mac{
    SSL_DEPANDS = $$OUT_PWD/../lib/$$TTKMusicPlayer/libssleay32.so
    exists($$SSL_DEPANDS):LIBS += -L../lib/$$TTKMusicPlayer -lssl
}

##qmmp lib check
win32:{
    QMMP_DEPANDS = $$OUT_PWD/../bin/$$TTKMusicPlayer
    equals(QT_MAJOR_VERSION, 4){
        QMMP_DEPANDS = $$QMMP_DEPANDS/qmmp0.dll
    }else{
        QMMP_DEPANDS = $$QMMP_DEPANDS/qmmp1.dll
    }
    QMMP_DEPANDS = $$replace(QMMP_DEPANDS, /, \\)
}
unix:!mac{
    QMMP_DEPANDS = $$OUT_PWD/../lib/$$TTKMusicPlayer/libqmmp.so
}
!exists($$QMMP_DEPANDS): error("Could not find qmmp library, please download and put it to output dir")

win32{
    LIBS += -lIphlpapi -lVersion -lole32 -luuid
    equals(QT_MAJOR_VERSION, 5){
        greaterThan(QT_MINOR_VERSION, 1):QT  += winextras
        msvc{
            LIBS += -L../bin/$$TTKMusicPlayer -lqmmp1 -lTTKUi -lTTKExtras -lTTKWatcher -lzlib -lTTKZip -luser32
            CONFIG +=c++11
            !contains(QMAKE_TARGET.arch, x86_64){
                 #support on windows XP
                 QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
                 QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01
            }
        }

        gcc{
            LIBS += -L../bin/$$TTKMusicPlayer -lqmmp1 -lTTKUi -lTTKExtras -lTTKWatcher -lzlib -lTTKZip
            QMAKE_CXXFLAGS += -std=c++11
            QMAKE_CXXFLAGS += -Wunused-function
            QMAKE_CXXFLAGS += -Wswitch
        }
    }

    equals(QT_MAJOR_VERSION, 4){
        QT  += multimedia
        gcc{
            LIBS += -L../bin/$$TTKMusicPlayer -lqmmp0 -lTTKUi -lTTKExtras -lTTKWatcher -lzlib -lTTKZip
            QMAKE_CXXFLAGS += -std=c++11
            QMAKE_CXXFLAGS += -Wunused-function
            QMAKE_CXXFLAGS += -Wswitch
        }
    }
}

unix:!mac{
    equals(QT_MAJOR_VERSION, 4){
        QMAKE_CXXFLAGS += -I/usr/include/QtMultimediaKit \
                          -I/usr/include/QtMobility
        LIBS += -lQtMultimediaKit
    }

    QMAKE_CXXFLAGS += -std=c++11
    QMAKE_CXXFLAGS += -Wunused-function
    QMAKE_CXXFLAGS += -Wswitch
    LIBS += -L../lib/$$TTKMusicPlayer -lqmmp -lTTKUi -lTTKExtras -lTTKWatcher -lzlib -lTTKZip
}

DEFINES += TTK_LIBRARY
DEFINES += QMMP_LIBRARY

#########################################
HEADERS += $$PWD/musicglobal.h
INCLUDEPATH += $$PWD
#########################################
include(TTKThirdParty/TTKThirdParty.pri)
#########################################
include(TTKModule/TTKModule.pri)
