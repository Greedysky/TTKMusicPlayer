# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = lib
CONFIG += TTK_BUILD_LIB

include(../TTKMusicPlayer.pri)
unix:VERSION += $$TTKMusicPlayer

equals(QT_MAJOR_VERSION, 4){
INCLUDEPATH += TTKExtra/Qt4
}
equals(QT_MAJOR_VERSION, 5){
INCLUDEPATH += TTKExtra/Qt5
}

win32:TARGET = ../../bin/$$TTKMusicPlayer/TTKCore
unix:TARGET = ../lib/$$TTKMusicPlayer/TTKCore

win32{
    LIBS += -lIphlpapi -lVersion
    equals(QT_MAJOR_VERSION, 5){
        greaterThan(QT_VER_MINOR, 1):QT  += winextras
        msvc{
            LIBS += -L../bin/$$TTKMusicPlayer -lqmmp1 -lTTKUi -lTTKImage -lTTKExtras -lTTKWatcher -lzlib -lTTKZip
            CONFIG +=c++11
            !contains(QMAKE_TARGET.arch, x86_64){
                 #support on windows XP
                 QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
                 QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01
            }
        }

        gcc{
            LIBS += -L../bin/$$TTKMusicPlayer -lqmmp1 -lTTKUi -lTTKImage -lTTKExtras -lTTKWatcher -lzlib -lTTKZip
        }
    }

    equals(QT_MAJOR_VERSION, 4){
        QT  += multimedia
        gcc{
            LIBS += -L../bin/$$TTKMusicPlayer -lqmmp0 -lTTKUi -lTTKImage -lTTKExtras -lTTKWatcher -lzlib -lTTKZip
        }
    }
}

unix:!mac{
    equals(QT_MAJOR_VERSION, 4){
        QMAKE_CXXFLAGS += -I/usr/include/QtMultimediaKit \
                          -I/usr/include/QtMobility
        LIBS += -lQtMultimediaKit
    }
    LIBS += -L../lib/$$TTKMusicPlayer -lqmmp -lTTKUi -lTTKImage -lTTKExtras -lTTKWatcher -lzlib -lTTKZip
}

DEFINES += MUSIC_LIBRARY

INCLUDEPATH += $$PWD

contains(CONFIG, TTK_BUILD_LIB){
    include(musicUiKits/MusicUiKits.pri)
}
include(musicCoreKits/MusicCoreKits.pri)
include(musicNetworkKits/MusicNetworkKits.pri)
include(musicWidgetKits/MusicWidgetKits.pri)
include(musicWidgetCoreKits/MusicWidgetCoreKits.pri)
include(musicSearchKits/MusicSearchKits.pri)
include(musicLrcKits/MusicLrcKits.pri)
include(musicRemoteKits/MusicRemoteKits.pri)
include(musicToolsSetsKits/MusicToolsSetsKits.pri)
include(musicToolsKits/MusicToolsKits.pri)
include(musicUserKits/MusicUserKits.pri)
include(musicUtilsKits/MusicUtilsKits.pri)
include(musicVideoKits/MusicVideoKits.pri)


SOURCES += \
    musicapplication.cpp \
    musicleftareawidget.cpp \
    musictopareawidget.cpp \
    musicrightareawidget.cpp \
    musicbottomareawidget.cpp \
    musicapplicationobject.cpp


HEADERS  += \
    musicapplication.h \
    musicleftareawidget.h \
    musictopareawidget.h \
    musicrightareawidget.h \
    musicbottomareawidget.h \
    musicapplicationobject.h
    
win32{
    RC_FILE = TTKCore.rc
}
