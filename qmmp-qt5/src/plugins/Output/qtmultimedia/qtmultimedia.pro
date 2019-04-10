include(../../plugins.pri)

QT += multimedia

HEADERS += outputqtmultimediafactory.h \
           outputqtmultimedia.h

SOURCES += outputqtmultimediafactory.cpp \
           outputqtmultimedia.cpp

unix:android {
    TARGET = $$PLUGINS_PREFIX/../plugin_output_qtmultimedia
}else{
    TARGET = $$PLUGINS_PREFIX/Output/qtmultimedia
}

INCLUDEPATH += ../../../
QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer

CONFIG += warn_on \
          thread \
          plugin

TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    unix:android {
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../plugin_output_libqtmultimedia.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libqtmultimedia.so
        target.path = $$LIB_DIR/qmmp/Output
    }
    INSTALLS += target
    LIBS += -lqmmp
}

win32 {
    LIBS += -lqmmp1
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
}
