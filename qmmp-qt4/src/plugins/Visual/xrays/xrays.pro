include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/xrays
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libxrays.so


HEADERS += xrays.h \
           fft.h \
           visualxraysfactory.h \
           inlines.h \
           colorwidget.h
           
SOURCES += xrays.cpp \
           fft.c \
           visualxraysfactory.cpp \
           colorwidget.cpp

FORMS    += colorwidget.ui

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
