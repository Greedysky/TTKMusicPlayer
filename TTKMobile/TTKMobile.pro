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

TTKMusicPlayer = 2.4.1.0

QT       += core gui xml qml quick multimedia

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets
    include(../TTKExtra/Qt5/qmmp.pri)
    win32:LIBS += -Lbin/$$TTKMusicPlayer -lqmmp1
}

TRANSLATIONS += TTKMobile.ts
!android{
    ##update translation
    unix:exists($$[QT_INSTALL_BINS]/lrelease){
        LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease
    }

    unix:exists($$[QT_INSTALL_BINS]/lrelease-qt5){
        LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease-qt5
    }

    win32:exists($$[QT_INSTALL_BINS]/lrelease.exe){
        LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease.exe
    }
    isEmpty(LRELEASE_EXECUTABLE){
        error(Could not find lrelease executable)
    }
    else{
        message(Found lrelease executable: $$LRELEASE_EXECUTABLE)
    }

    unix:{
        output = $$OUT_PWD/lib/$$TTKMusicPlayer/MLanguage
        !exists($$output):system(mkdir $$output)

        system(find . -name *.ts | xargs $$LRELEASE_EXECUTABLE)
        system(find . -name *.qm | xargs rename -vf 's/.qm/.ln/' *  )
        system(for F in TTKLanguage/*.ln ; do mv $F $$output ;done)
    }
    win32:{
        output = $$OUT_PWD/bin/$$TTKMusicPlayer/MLanguage
        output = $$replace(output, /, \\)
        !exists($$output):system(md $$output)

        system(for /r %i in (*.ts) do $$LRELEASE_EXECUTABLE %i)
        system(for /r %i in (*.qm) do ren %i *.ln)
        system(for /r %i in (*.ln) do move /y %i $$output)
    }
}

TEMPLATE = app
win32:TARGET = ../bin/$$TTKMusicPlayer/TTKMobile

QMAKE_CXXFLAGS += -std=c++11

DEFINES += \
    MUSIC_LIBRARY \
    MUSIC_MOBILE

INCLUDEPATH += \
    ../ \
    ../TTKCore/musicCore \
    ../TTKCore/musicCore/utils \
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
    ../TTKCore/musicCore/musicsong.h \
    ../TTKCore/musicCore/musictime.h \
    ../TTKCore/musicCore/musicsettingmanager.h \
    ../TTKCore/musicCore/musicabstractxml.h \
    ../TTKCore/musicCore/musiccryptographichash.h \
    ../TTKCore/musicCore/musicconnectionpool.h \
    ../TTKCore/musicCore/musicbackgroundmanager.h \
    ../TTKCore/musicLrcmanager/musiclrcanalysis.h \
    ../TTKCore/musicToolsetswidget/core/musicsongtag.h \
    ../TTKCore/musicWidget/musicdownloadstatuslabel.h \
    core/ttkfilesearchcore.h \
    core/ttkmusicplaylist.h \
    core/ttkmusicplayer.h \
    core/ttkmusicutils.h \
    core/ttkmusicconfigmanager.h \
    core/ttkmusicsongssummarizied.h \
    core/ttknetworkhelper.h \
    core/ttkmusiclyricmodel.h


SOURCES += \
    musicmobilemain.cpp \
    musicapplication.cpp \
    ../TTKCore/musicCore/utils/musiccoreutils.cpp \
    ../TTKCore/musicCore/utils/musicnumberutils.cpp \
    ../TTKCore/musicCore/musicsong.cpp \
    ../TTKCore/musicCore/musictime.cpp \
    ../TTKCore/musicCore/musicabstractxml.cpp \
    ../TTKCore/musicCore/musiccryptographichash.cpp \
    ../TTKCore/musicCore/musicconnectionpool.cpp \
    ../TTKCore/musicCore/musicbackgroundmanager.cpp \
    ../TTKCore/musicLrcmanager/musiclrcanalysis.cpp \
    ../TTKCore/musicToolsetswidget/core/musicsongtag.cpp \
    ../TTKCore/musicWidget/musicdownloadstatuslabel.cpp \
    core/ttkfilesearchcore.cpp \
    core/ttkmusicplaylist.cpp \
    core/ttkmusicplayer.cpp \
    core/ttkmusicutils.cpp \
    core/ttkmusicconfigmanager.cpp \
    core/ttkmusicsongssummarizied.cpp \
    core/ttknetworkhelper.cpp \
    core/ttkmusiclyricmodel.cpp


CONFIG += TTK_BUILD_LIB
include(../TTKCore/musicNetwork/MusicNetwork.pri)
include(../TTKThirdParty/MusicExtras/qjson/QJson.pri)

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
# Default rules for deployment.
include(deployment.pri)
QT += androidextras
#    ANDROID_EXTRA_LIBS = \

LIBS += -Lbin/ -lqmmp

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
        bin/libplugin_input_aac.so \
        bin/libplugin_input_cue.so \
#        bin/libplugin_input_ffmpeg.so \
        bin/libplugin_input_flac.so \
        bin/libplugin_input_gme.so \
        bin/libplugin_input_mad.so \
        bin/libplugin_input_modplug.so \
        bin/libplugin_input_mpc.so \
        bin/libplugin_input_opus.so \
        bin/libplugin_input_sndfile.so \
        bin/libplugin_input_vorbis.so \
        bin/libplugin_input_wavpack.so \
        bin/libplugin_output_qtmultimedia.so \
        bin/libqmmp.so \
        bin/libtag.so \
#        bin/libavcodec.so \
#        bin/libavformat.so \
#        bin/libavutil.so
}

RESOURCES += \
    TTKMobile.qrc \
    extra\TTKExtra.qrc

win32{
    RC_FILE = TTKMobile.rc
}
