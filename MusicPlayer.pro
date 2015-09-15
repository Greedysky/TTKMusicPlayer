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

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01

SOURCES += \
    musicapplicationmain.cpp\
    musicapplication.cpp \
    localsearch/*.cpp \
    lrcmanager/*.cpp \
    remotewidget/*.cpp \
    toolswidget/*.cpp \
    toolsetswidget/*.cpp \
    core/network/*.cpp \
    core/*.cpp \
    core-widget/*.cpp \
    musicleftareawidget.cpp \
    musictopareawidget.cpp \
    musicrightareawidget.cpp \
    musicbottomareawidget.cpp \
    musicapplicationobject.cpp


HEADERS  += \
    core/*.h \
    musicapplication.h \
    localsearch/*.h \
    lrcmanager/*.h \
    remotewidget/*.h \
    toolswidget/*.h \
    toolsetswidget/*.h \
    core/network/*.h \
    core-widget/*.h \
    musicleftareawidget.h \
    musictopareawidget.h \
    musicrightareawidget.h \
    musicbottomareawidget.h \
    musicapplicationobject.h


FORMS    += \
    ui/application/*.ui \
    ui/core-widget/*.ui \
    ui/lrcmanager/*.ui \
    ui/toolsetswidget/*.ui \
    ui/toolswidget/*.ui


RESOURCES += \
    qrc/MusicPlayerShare.qrc \
    qrc/MusicPlayer.qrc \

RC_FILE = \
    MusicPlayer.rc

include(usermanager/MusicUser.pri)
include(videokits/MusicVideo.pri)


