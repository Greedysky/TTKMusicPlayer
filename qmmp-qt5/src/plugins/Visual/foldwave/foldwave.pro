include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/foldwave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfoldwave.so

include(../common/Visual.pri)

HEADERS += foldwave.h \
           visualfoldwavefactory.h
           
SOURCES += foldwave.cpp \
           visualfoldwavefactory.cpp

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
