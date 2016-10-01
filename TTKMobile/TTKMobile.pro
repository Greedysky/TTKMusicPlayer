#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T23:19:41
#
# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2014 - 2016 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

QT       += core gui qml quick multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

DEFINES += \
    MUSIC_LIBRARY \
    MUSIC_MOBILE

INCLUDEPATH += \
    ../ \
    ../TTKCore/musicCore \
    ../TTKCore/musicCore/utils

HEADERS += \
    musicmobileglobaldefine.h \
    ../TTKCore/musicCore/musicsong.h \
    ../TTKCore/musicCore/musictime.h \
    ../TTKCore/musicCore/musicsettingmanager.h \
    ../TTKCore/musicCore/utils/musiccoreutils.h \
    core/ttkfilesearchcore.h \
    core/ttkmusicplaylist.h \
    core/ttkmusicplayer.h \
    core/ttkmusicutils.h


SOURCES += \
    musicmobilemain.cpp \
    ../TTKCore/musicCore/musicsong.cpp \
    ../TTKCore/musicCore/musictime.cpp \
    ../TTKCore/musicCore/utils/musiccoreutils.cpp \
    core/ttkfilesearchcore.cpp \
    core/ttkmusicplaylist.cpp \
    core/ttkmusicplayer.cpp \
    core/ttkmusicutils.cpp


contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
# Default rules for deployment.
include(deployment.pri)
QT += androidextras
#    ANDROID_EXTRA_LIBS = \


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/org/greedysky/ttkmobile/TTKMobile.java \
    android/src/org/greedysky/ttkmobile/SplashScreen.java
}

RESOURCES += TTKMobile.qrc

win32{
    RC_FILE = TTKMobile.rc
}
