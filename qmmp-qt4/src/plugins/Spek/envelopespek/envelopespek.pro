include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Spek/envelopespek
QMAKE_CLEAN =$$PLUGINS_PREFIX/Spek/libenvelopespek.so


SOURCES += \
        envelopespek.cpp \
        fft.c \
        visualenvelopespekfactory.cpp

HEADERS  += \
        envelopespek.h \
        fft.h \
        inlines.h \
        visualenvelopespekfactory.h

INCLUDEPATH += ../../../

CONFIG += warn_on \
            plugin

TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11

win32:{
    INCLUDEPATH += ./
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -lqmmp0
}

unix{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -lqmmp -L/usr/lib -I/usr/include
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
}
