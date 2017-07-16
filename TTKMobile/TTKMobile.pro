# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
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
    ../TTKCore/musicCoreKits \
    ../TTKCore/musicCoreKits/utils \
    ../TTKCore/musicLrcKits \
    ../TTKCore/musicNetworkKits \
    ../TTKCore/musicWidgetKits \
    ../TTKThirdParty/MusicExtras \
    ../TTKThirdParty


HEADERS += \
    musicapplication.h \
    musicmobileglobaldefine.h \
    ../TTKCore/musicCoreKits/utils/musiccoreutils.h \
    ../TTKCore/musicCoreKits/utils/musicnumberutils.h \
    ../TTKCore/musicCoreKits/utils/musicstringutils.h \
    ../TTKCore/musicCoreKits/utils/musicalgorithmutils.h \
    ../TTKCore/musicCoreKits/musicsong.h \
    ../TTKCore/musicCoreKits/musictime.h \
    ../TTKCore/musicCoreKits/musicformats.h \
    ../TTKCore/musicCoreKits/musicsettingmanager.h \
    ../TTKCore/musicCoreKits/musicabstractxml.h \
    ../TTKCore/musicCoreKits/musiccryptographichash.h \
    ../TTKCore/musicCoreKits/musicconnectionpool.h \
    ../TTKCore/musicCoreKits/musicbackgroundmanager.h \
    ../TTKCore/musicCoreKits/musicsongtag.h \
    ../TTKCore/musicCoreKits/musicsemaphoreloop.h \
    ../TTKCore/musicLrcKits/musiclrcanalysis.h \
    ../TTKCore/musicWidgetKits/musicdownloadstatusobject.h \
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
    ../TTKCore/musicCoreKits/utils/musiccoreutils.cpp \
    ../TTKCore/musicCoreKits/utils/musicnumberutils.cpp \
    ../TTKCore/musicCoreKits/utils/musicstringutils.cpp \
    ../TTKCore/musicCoreKits/utils/musicalgorithmutils.cpp \
    ../TTKCore/musicCoreKits/musicsong.cpp \
    ../TTKCore/musicCoreKits/musictime.cpp \
    ../TTKCore/musicCoreKits/musicformats.cpp \
    ../TTKCore/musicCoreKits/musicabstractxml.cpp \
    ../TTKCore/musicCoreKits/musiccryptographichash.cpp \
    ../TTKCore/musicCoreKits/musicconnectionpool.cpp \
    ../TTKCore/musicCoreKits/musicbackgroundmanager.cpp \
    ../TTKCore/musicCoreKits/musicsongtag.cpp \
    ../TTKCore/musicCoreKits/musicsemaphoreloop.cpp \
    ../TTKCore/musicLrcKits/musiclrcanalysis.cpp \
    ../TTKCore/musicWidgetKits/musicdownloadstatusobject.cpp \
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

include(../TTKCore/musicNetworkKits/MusicNetworkKits.pri)
include(../TTKThirdParty/MusicExtras/qjson/QJson.pri)
include(../TTKThirdParty/MusicExtras/qaes/QAes.pri)

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

ANDROID_EXTRA_LIBS = \
    bin/$$TTKMusicPlayer/libplugin_input_aac.so \
    bin/$$TTKMusicPlayer/libplugin_input_cue.so \
#    bin/$$TTKMusicPlayer/libplugin_input_ffmpeg.so \
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
    bin/$$TTKMusicPlayer/libtag.so
#    bin/$$TTKMusicPlayer/libavcodec.so \
#    bin/$$TTKMusicPlayer/libavformat.so \
#    bin/$$TTKMusicPlayer/libavutil.so
}

RESOURCES += \
    TTKMobile.qrc \
    extra\TTKExtra.qrc

win32{
    RC_FILE = TTKMobile.rc
}
