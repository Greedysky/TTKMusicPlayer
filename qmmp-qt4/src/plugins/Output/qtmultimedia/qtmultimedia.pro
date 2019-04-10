include(../../plugins.pri)

QT += multimedia

HEADERS += outputqtmultimediafactory.h \
           outputqtmultimedia.h

SOURCES += outputqtmultimediafactory.cpp \
           outputqtmultimedia.cpp

TARGET=$$PLUGINS_PREFIX/Output/qtmultimedia

INCLUDEPATH += ../../../

CONFIG += warn_on \
          thread \
          plugin

TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Output
    INSTALLS += target
    LIBS += -lqmmp
    QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libqtmultimedia.so
}

win32 {
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -lqmmp0
}
