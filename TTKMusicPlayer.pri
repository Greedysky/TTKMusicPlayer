#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

QT       += core gui xml sql

equals(QT_MAJOR_VERSION, 4){
QT       += network script
CONFIG   += gcc
include(TTKExtra/Qt4/qmmp.pri)
}
equals(QT_MAJOR_VERSION, 5){
QT       += widgets multimediawidgets
include(TTKExtra/Qt5/qmmp.pri)
}

UI_DIR = ./.build/ui/
MOC_DIR = ./.build/moc/
OBJECTS_DIR = ./.build/obj
RCC_DIR = ./.build/rcc

win32{
    equals(QT_MAJOR_VERSION, 5){
        QT  += winextras
        msvc{
            LIBS += -L../bin -lqmmp1
            !contains(QMAKE_TARGET.arch, x86_64){
                 #support on windows XP
                 QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
            }
        }

        gcc{
            LIBS += -L../bin -lqmmp1
            QMAKE_CXXFLAGS += -std=c++11
            QMAKE_CXXFLAGS += -Wunused-function
            QMAKE_CXXFLAGS += -Wswitch
        }
    }

    equals(QT_MAJOR_VERSION, 4){
        QT  += multimedia
        gcc{
            LIBS += -L../bin -lqmmp0
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
    LIBS += -L../lib -lqmmp
}

TTKMusicPlayer = 2.2.9.1

DEFINES += MUSIC_LIBRARY
#DEFINES += USE_MULTIPLE_QUERY

contains(CONFIG, TTK_BUILD_LIB){
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
