include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/gwave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libgwave.so


HEADERS += gwave.h \
           fft.h \
           visualgwavefactory.h \
           inlines.h \
           colorwidget.h
           
SOURCES += gwave.cpp \
           fft.c \
           visualgwavefactory.cpp \
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
