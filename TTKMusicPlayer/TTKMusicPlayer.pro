#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
#-------------------------------------------------

TARGET = ../../bin/MusicCore
TEMPLATE = lib

CONFIG += TTK_BUILD_LIB

INCLUDEPATH += $$PWD

include(../TTKMusicPlayer.pri)

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
    musicQrc/MusicPlayerShare.qrc \
    musicQrc/MusicPlayer.qrc
