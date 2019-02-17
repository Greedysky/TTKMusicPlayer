include(../../qmmp.pri)

unix:android {
    EXTRA_PREFIX=../../../../../extra/android
}else{
    EXTRA_PREFIX=../../../../../extra/gcc
}

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    PLUGINS_PREFIX=../../../../lib/$$TTKMusicPlayer/qmmp
    LIBS += -lqmmp
}

win32 {
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    PLUGINS_PREFIX=../../../../../bin/$$TTKMusicPlayer/plugins
    LIBS += -lqmmp1
}

INCLUDEPATH += ../../../qmmp ../../../

CONFIG += warn_on plugin lib thread link_pkgconfig
TEMPLATE = lib
