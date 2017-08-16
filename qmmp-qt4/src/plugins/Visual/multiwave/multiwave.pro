include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/multiwave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libmultiwave.so

include(../common/Visual.pri)

HEADERS += multiwave.h \
           visualmultiwavefactory.h

SOURCES += multiwave.cpp \
           visualmultiwavefactory.cpp

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
