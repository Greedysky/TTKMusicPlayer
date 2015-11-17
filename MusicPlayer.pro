#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

QT       += core gui xml sql

equals(QT_MAJOR_VERSION, 4){
QT       += multimedia network script
}
equals(QT_MAJOR_VERSION, 5){
QT       += widgets multimediawidgets
}

TARGET = MusicPlayer
#TARGET = MusicCore

TEMPLATE = app
#LIBS += -L"./" -lMusicCore
#TEMPLATE = lib

win32{
    equals(QT_MAJOR_VERSION, 5):{
    QT   += winextras
        msvc{
            LIBS += -L"./MPlugins" -llibzplay
            #support on windows XP
            QMAKE_LFLAGS_WINDOWS += /SUBSYSTEM:WINDOWS,5.01
        }

        gcc{
            LIBS += ./MPlugins/libzplay.a
            QMAKE_CXXFLAGS += -std=c++11
            QMAKE_CXXFLAGS += -Wunused-function
            QMAKE_CXXFLAGS += -Wswitch
        }
    }
    equals(QT_MAJOR_VERSION, 4):{
            LIBS += ./MPlugins/libzplay.a
            QMAKE_CXXFLAGS += -std=c++11
            QMAKE_CXXFLAGS += -Wunused-function
            QMAKE_CXXFLAGS += -Wswitch
    }
}
DEFINES += MUSIC_LIBRARY

INCLUDEPATH += $$PWD

TRANSLATIONS += musicLanguage/cn.ts \
                musicLanguage/cn_c.ts \
                musicLanguage/en.ts

include(musicCore/MusicCore.pri)
include(musicWidget/MusicWidget.pri)
include(musicLocalsearch/MusicLocalSearch.pri)
include(musicLrcmanager/MusicLrc.pri)
include(musicRemotewidget/MusicRemote.pri)
include(musicToolsetswidget/MusicToolsSets.pri)
include(musicToolswidget/MusicToolsWidget.pri)
include(musicUsermanager/MusicUser.pri)
include(musicVideokits/MusicVideo.pri)


SOURCES += \
    musicapplicationmain.cpp\
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


FORMS    += \
    musicUi/application/musicapplication.ui


RESOURCES += \
    musicQrc/MusicPlayerShare.qrc \
    musicQrc/MusicPlayer.qrc \

RC_FILE = \
    MusicPlayer.rc
