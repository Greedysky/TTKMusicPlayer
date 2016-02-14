#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

win32:TARGET = ../../bin/MusicCore
unix:TARGET = ../lib/MusicCore
TEMPLATE = lib

CONFIG += TTK_BUILD_LIB

INCLUDEPATH += $$PWD

include(../TTKMusicPlayer.pri)
unix:VERSION += $$TTKMusicPlayer

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


RESOURCES += \
    ../TTKQrc/MusicPlayerShare.qrc \
    ../TTKQrc/MusicPlayer.qrc \
    ../TTKQrc/MusicPlayerUser.qrc \
    ../TTKQrc/MusicPlayerVedio.qrc
