include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/volumewave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libvolumewave.so

include(../common/Visual.pri)

HEADERS += volumewave.h \
           visualvolumewavefactory.h
           
SOURCES += volumewave.cpp \
           visualvolumewavefactory.cpp

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
