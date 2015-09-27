#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

QT       += core gui xml sql
QT       += multimedia multimediawidgets
QT       += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer
#TARGET = MusicCore

TEMPLATE = app
#LIBS += -L"./" -lMusicCore
#TEMPLATE = lib
LIBS += -L"./MPlugins/" -llibzplay

DEFINES += MUSIC_LIBRARY

INCLUDEPATH += $$PWD/core \
               $$PWD/core\network \
               $$PWD/core-widget \
               $$PWD/localsearch \
               $$PWD/lrcmanager \
               $$PWD/remotewidget \
               $$PWD/toolsetswidget \
               $$PWD/toolswidget \
               $$PWD/usermanager \
               $$PWD/videokits \
               $$PWD/

TRANSLATIONS += language/cn.ts \
                language/cn_c.ts \
                language/en.ts

#support on windows XP
QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01


include(core/MusicCore.pri)
include(core-widget/MusicWidget.pri)
include(localsearch/MusicLocalSearch.pri)
include(lrcmanager/MusicLrc.pri)
include(remotewidget/MusicRemote.pri)
include(toolsetswidget/MusicToolsSets.pri)
include(toolswidget/MusicToolsWidget.pri)
include(usermanager/MusicUser.pri)
include(videokits/MusicVideo.pri)


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
    ui/application/musicapplication.ui


RESOURCES += \
    qrc/MusicPlayerShare.qrc \
    qrc/MusicPlayer.qrc \

RC_FILE = \
    MusicPlayer.rc
