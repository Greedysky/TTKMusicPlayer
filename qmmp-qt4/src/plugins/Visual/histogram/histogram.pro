include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/histogram
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libhistogram.so


HEADERS += histogram.h \
           fft.h \
           visualhistogramfactory.h \
           inlines.h
           

SOURCES += histogram.cpp \
           fft.c \
           visualhistogramfactory.cpp


INCLUDEPATH += ../../../
CONFIG += warn_on \
            plugin

TEMPLATE = lib

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -lqmmp0
}

unix{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -lqmmp -L/usr/lib -I/usr/include
}
