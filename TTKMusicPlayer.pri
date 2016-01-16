#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

QT       += core gui xml sql

equals(QT_MAJOR_VERSION, 4){
QT       += multimedia network script
CONFIG   += gcc
}
equals(QT_MAJOR_VERSION, 5){
QT       += widgets multimediawidgets
}


UI_DIR = ./.build/ui/
MOC_DIR = ./.build/moc/
OBJECTS_DIR = ./.build/obj
RCC_DIR = ./.build/rcc


win32{
    equals(QT_MAJOR_VERSION, 5){
    QT  += winextras
        msvc{
            LIBS += -L"../bin/MPlugins" -llibzplay
            !contains(QMAKE_TARGET.arch, x86_64){
                 #support on windows XP
                 QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
            }
        }

        gcc{
            LIBS += ../bin/MPlugins/libzplay.a
            QMAKE_CXXFLAGS += -std=c++11
            QMAKE_CXXFLAGS += -Wunused-function
            QMAKE_CXXFLAGS += -Wswitch
        }
    }

    equals(QT_MAJOR_VERSION, 4){
        gcc{
            LIBS += ../bin/MPlugins/libzplay.a
            QMAKE_CXXFLAGS += -std=c++11
            QMAKE_CXXFLAGS += -Wunused-function
            QMAKE_CXXFLAGS += -Wswitch
        }
    }
}

DEFINES += MUSIC_LIBRARY

contains(CONFIG, TTK_BUID_LIB){
  include(TTKMusicPlayer/musicUi/MusicUi.pri)
}
include(TTKMusicPlayer/musicCore/MusicCore.pri)
include(TTKMusicPlayer/musicWidget/MusicWidget.pri)
include(TTKMusicPlayer/musicLocalsearch/MusicLocalSearch.pri)
include(TTKMusicPlayer/musicLrcmanager/MusicLrc.pri)
include(TTKMusicPlayer/musicRemotewidget/MusicRemote.pri)
include(TTKMusicPlayer/musicToolsetswidget/MusicToolsSets.pri)
include(TTKMusicPlayer/musicToolswidget/MusicToolsWidget.pri)
include(TTKMusicPlayer/musicUsermanager/MusicUser.pri)
include(TTKMusicPlayer/musicVideokits/MusicVideo.pri)
