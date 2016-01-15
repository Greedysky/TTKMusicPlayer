#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

TARGET = ../../bin/TTKMusicPlayer
#TARGET = MusicCore

TEMPLATE = app
#LIBS += -L"./" -lMusicCore
#TEMPLATE = lib


INCLUDEPATH += $$PWD

TRANSLATIONS += musicLanguage/cn.ts \
                musicLanguage/cn_c.ts \
                musicLanguage/en.ts

include(../TTKMusicPlayer.pri)

include(musicCore/MusicCore.pri)
include(musicWidget/MusicWidget.pri)
include(musicLocalsearch/MusicLocalSearch.pri)
include(musicLrcmanager/MusicLrc.pri)
include(musicRemotewidget/MusicRemote.pri)
include(musicToolsetswidget/MusicToolsSets.pri)
include(musicToolswidget/MusicToolsWidget.pri)
include(musicUi/MusicUi.pri)
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


RESOURCES += \
    musicQrc/MusicPlayerShare.qrc \
    musicQrc/MusicPlayer.qrc \

win32{
    RC_FILE = \
        TTKMusicPlayer.rc
}
