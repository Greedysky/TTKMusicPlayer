# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2018 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
#
# * You should have received a copy of the GNU General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
# =================================================

include(../TTKVersion.pri)

QT       += core gui xml qml quick multimedia

UI_DIR = ./.build/ui
MOC_DIR = ./.build/moc
OBJECTS_DIR = ./.build/obj
RCC_DIR = ./.build/rcc

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets
    include(../TTKExtra/Qt5/qmmp.pri)
    win32:LIBS += -Lbin/$$TTKMusicPlayer -lqmmp1
}

TRANSLATIONS += TTKMobile.ts
#lupdate_only
#{
#    SOURCES +=  \
#        MobileWidgets/* \
#        MobileWidgets/Core/*
#}

##update translation
win32:exists($$[QT_INSTALL_BINS]/lrelease.exe){
    LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease.exe

    isEmpty(LRELEASE_EXECUTABLE){
        error(Could not find lrelease executable)
    }
    else{
        message(Found lrelease executable: $$LRELEASE_EXECUTABLE)
    }

    output = $$OUT_PWD/bin/$$TTKMusicPlayer/MLanguage
    output = $$replace(output, /, \\)
    !exists($$output):system(md $$output)

    system(for /r %i in (*.ts) do $$LRELEASE_EXECUTABLE %i)
    system(for /r %i in (*.qm) do ren %i *.ln)
    system(for /r %i in (*.ln) do copy /y %i $$output)
    system(for /r %i in (*.ln) do move /y %i $$PWD/extra)
}else{
    output = $$OUT_PWD/bin/$$TTKMusicPlayer
    output = $$replace(output, /, \\)
    !exists($$output):system(md $$output)
}

TEMPLATE = app
win32:TARGET = ../bin/$$TTKMusicPlayer/TTKMobile

QMAKE_CXXFLAGS += -std=c++11

DEFINES += \
    MUSIC_LIBRARY \
    MUSIC_MOBILE

INCLUDEPATH += \
    MobileCore \
    ../ \
    ../TTKModule/TTKCore/musicCoreKits \
    ../TTKModule/TTKCore/musicUtilsKits \
    ../TTKModule/TTKCore/musicLrcKits \
    ../TTKModule/TTKCore/musicNetworkKits \
    ../TTKModule/TTKWidget/musicWidgetKits \
    ../TTKThirdParty/TTKExtras \
    ../TTKThirdParty


HEADERS += \
    musicapplication.h \
    musicmobileglobaldefine.h \
    ../TTKModule/TTKCore/musicCoreKits/musicsong.h \
    ../TTKModule/TTKCore/musicCoreKits/musictime.h \
    ../TTKModule/TTKCore/musicCoreKits/musicformats.h \
    ../TTKModule/TTKCore/musicCoreKits/musicsettingmanager.h \
    ../TTKModule/TTKCore/musicCoreKits/musicabstractxml.h \
    ../TTKModule/TTKCore/musicCoreKits/musiccryptographichash.h \
    ../TTKModule/TTKCore/musicCoreKits/musicconnectionpool.h \
    ../TTKModule/TTKCore/musicCoreKits/musicbackgroundmanager.h \
    ../TTKModule/TTKCore/musicCoreKits/musicsongtag.h \
    ../TTKModule/TTKCore/musicCoreKits/musicsemaphoreloop.h \
    ../TTKModule/TTKWidget/musicWidgetKits/musicdownloadstatusobject.h \
    MobileCore/ttkmusicutils.h \
    MobileCore/ttkmusicconfigmanager.h \
    MobileCore/ttknetworkhelper.h


SOURCES += \
    musicmobilemain.cpp \
    musicapplication.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musicsong.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musictime.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musicformats.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musicabstractxml.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musiccryptographichash.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musicconnectionpool.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musicbackgroundmanager.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musicsongtag.cpp \
    ../TTKModule/TTKCore/musicCoreKits/musicsemaphoreloop.cpp \
    ../TTKModule/TTKWidget/musicWidgetKits/musicdownloadstatusobject.cpp \
    MobileCore/ttkmusicutils.cpp \
    MobileCore/ttkmusicconfigmanager.cpp \
    MobileCore/ttknetworkhelper.cpp


CONFIG += MUSIC_BUILD_LIB

include(../TTKModule/TTKCore/musicUtilsKits/MusicUtilsKits.pri)
include(../TTKModule/TTKCore/musicNetworkKits/MusicNetworkKits.pri)
include(../TTKThirdParty/TTKExtras/qjson/QJson.pri)
include(../TTKThirdParty/TTKExtras/qalg/QAlg.pri)

contains(ANDROID_TARGET_ARCH, armeabi-v7a) {
# Default rules for deployment.
include(deployment.pri)

QT += androidextras

LIBS += -Lbin/$$TTKMusicPlayer -lqmmp

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/org/greedysky/ttkmobile/TTKMobile.java \
    android/src/org/greedysky/ttkmobile/SplashScreen.java

ANDROID_EXTRA_LIBS = \
    bin/$$TTKMusicPlayer/libplugin_input_flac.so \
    bin/$$TTKMusicPlayer/libplugin_input_mad.so \
    bin/$$TTKMusicPlayer/libplugin_input_wavpack.so \
    bin/$$TTKMusicPlayer/libplugin_input_vorbis.so \
    bin/$$TTKMusicPlayer/libqmmp.so \
    bin/$$TTKMusicPlayer/libtag.so
}

RESOURCES += \
    TTKMobile.qrc \
    extra\TTKExtra.qrc

win32{
    RC_FILE = TTKMobile.rc
}
