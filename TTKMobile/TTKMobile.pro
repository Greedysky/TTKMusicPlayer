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

include(../TTKVersion.pri)

QT       += core gui xml qml quick multimedia

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets
    include(../TTKExtra/Qt5/qmmp.pri)
    win32:LIBS += -Lbin/$$TTKMusicPlayer -lqmmp1
}

TRANSLATIONS += TTKMobile.ts
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
    ../TTKCore/musicCore \
    ../TTKCore/musicCore/utils \
    ../TTKCore/musicLrcmanager \
    ../TTKCore/musicNetwork \
    ../TTKCore/musicToolsetswidget/core \
    ../TTKCore/musicWidget \
    ../TTKThirdParty/MusicExtras


HEADERS += \
    musicapplication.h \
    musicmobileglobaldefine.h \
    ../TTKCore/musicCore/utils/musiccoreutils.h \
    ../TTKCore/musicCore/utils/musicnumberutils.h \
    ../TTKCore/musicCore/utils/musicstringutils.h \
    ../TTKCore/musicCore/musicsong.h \
    ../TTKCore/musicCore/musictime.h \
    ../TTKCore/musicCore/musicformats.h \
    ../TTKCore/musicCore/musicsettingmanager.h \
    ../TTKCore/musicCore/musicabstractxml.h \
    ../TTKCore/musicCore/musiccryptographichash.h \
    ../TTKCore/musicCore/musicconnectionpool.h \
    ../TTKCore/musicCore/musicbackgroundmanager.h \
    ../TTKCore/musicLrcmanager/musiclrcanalysis.h \
    ../TTKCore/musicToolsetswidget/core/musicsongtag.h \
    ../TTKCore/musicWidget/musicdownloadstatuslabel.h \
    MobileCore/ttkfilesearchcore.h \
    MobileCore/ttkmusicplaylist.h \
    MobileCore/ttkmusicplayer.h \
    MobileCore/ttkmusicutils.h \
    MobileCore/ttkmusicconfigmanager.h \
    MobileCore/ttkmusicsongssummarizied.h \
    MobileCore/ttknetworkhelper.h \
    MobileCore/ttkmusiclyricmodel.h \
    MobileCore/ttkfilterproxymodel.h \
    MobileCore/ttkradiohelper.h


SOURCES += \
    musicmobilemain.cpp \
    musicapplication.cpp \
    ../TTKCore/musicCore/utils/musiccoreutils.cpp \
    ../TTKCore/musicCore/utils/musicnumberutils.cpp \
    ../TTKCore/musicCore/utils/musicstringutils.cpp \
    ../TTKCore/musicCore/musicsong.cpp \
    ../TTKCore/musicCore/musictime.cpp \
    ../TTKCore/musicCore/musicformats.cpp \
    ../TTKCore/musicCore/musicabstractxml.cpp \
    ../TTKCore/musicCore/musiccryptographichash.cpp \
    ../TTKCore/musicCore/musicconnectionpool.cpp \
    ../TTKCore/musicCore/musicbackgroundmanager.cpp \
    ../TTKCore/musicLrcmanager/musiclrcanalysis.cpp \
    ../TTKCore/musicToolsetswidget/core/musicsongtag.cpp \
    ../TTKCore/musicWidget/musicdownloadstatuslabel.cpp \
    MobileCore/ttkfilesearchcore.cpp \
    MobileCore/ttkmusicplaylist.cpp \
    MobileCore/ttkmusicplayer.cpp \
    MobileCore/ttkmusicutils.cpp \
    MobileCore/ttkmusicconfigmanager.cpp \
    MobileCore/ttkmusicsongssummarizied.cpp \
    MobileCore/ttknetworkhelper.cpp \
    MobileCore/ttkmusiclyricmodel.cpp \
    MobileCore/ttkfilterproxymodel.cpp \
    MobileCore/ttkradiohelper.cpp


CONFIG += TTK_BUILD_LIB

include(../TTKCore/musicNetwork/MusicNetwork.pri)
include(../TTKThirdParty/MusicExtras/qjson/QJson.pri)

contains(ANDROID_TARGET_ARCH, armeabi-v7a) {
# Default rules for deployment.
include(deployment.pri)
include(MobileExtra/QZXing/QZXing/QZXing.pri)
RESOURCES += MobileExtra/QZXing/QZXing.qrc

QT += androidextras

LIBS += -Lbin/$$TTKMusicPlayer -lqmmp

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/org/greedysky/ttkmobile/TTKMobile.java \
    android/src/org/greedysky/ttkmobile/SplashScreen.java

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_EXTRA_LIBS = \
    bin/$$TTKMusicPlayer/libplugin_input_aac.so \
    bin/$$TTKMusicPlayer/libplugin_input_cue.so \
#        bin/$$TTKMusicPlayer/libplugin_input_ffmpeg.so \
    bin/$$TTKMusicPlayer/libplugin_input_flac.so \
    bin/$$TTKMusicPlayer/libplugin_input_gme.so \
    bin/$$TTKMusicPlayer/libplugin_input_mad.so \
    bin/$$TTKMusicPlayer/libplugin_input_modplug.so \
    bin/$$TTKMusicPlayer/libplugin_input_mpc.so \
    bin/$$TTKMusicPlayer/libplugin_input_opus.so \
    bin/$$TTKMusicPlayer/libplugin_input_sndfile.so \
    bin/$$TTKMusicPlayer/libplugin_input_vorbis.so \
    bin/$$TTKMusicPlayer/libplugin_input_wavpack.so \
    bin/$$TTKMusicPlayer/libplugin_output_qtmultimedia.so \
    bin/$$TTKMusicPlayer/libqmmp.so \
    bin/$$TTKMusicPlayer/libtag.so \
#        bin/$$TTKMusicPlayer/libavcodec.so \
#        bin/$$TTKMusicPlayer/libavformat.so \
#        bin/$$TTKMusicPlayer/libavutil.so
}

RESOURCES += \
    TTKMobile.qrc \
    extra\TTKExtra.qrc

win32{
    RC_FILE = TTKMobile.rc
}
