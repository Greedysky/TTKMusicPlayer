include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/analyzer
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libanalyzer.so


HEADERS += analyzer.h \
           fft.h \
           visualanalyzerfactory.h \
           inlines.h \
           colorwidget.h
           
           
SOURCES += analyzer.cpp \
           fft.c \
           visualanalyzerfactory.cpp \
           colorwidget.cpp

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

unix:{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -lqmmp -L/usr/lib -I/usr/include
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
