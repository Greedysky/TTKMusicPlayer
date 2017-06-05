include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/ripples
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libripples.so


HEADERS += ripples.h \
           fft.h \
           visualripplesfactory.h \
           inlines.h
           

SOURCES += ripples.cpp \
           fft.c \
           visualripplesfactory.cpp


INCLUDEPATH += ../../../
CONFIG += warn_on \
            plugin

TEMPLATE = lib

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -lqmmp1
}

unix{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -lqmmp -L/usr/lib -I/usr/include
}
