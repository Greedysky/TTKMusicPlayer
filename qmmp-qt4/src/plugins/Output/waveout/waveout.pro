include(../../plugins.pri)

HEADERS += outputwaveoutfactory.h \
           outputwaveout.h

SOURCES += outputwaveoutfactory.cpp \
           outputwaveout.cpp

HEADERS += ../../../../src/qmmp/output.h


TARGET=$$PLUGINS_PREFIX/Output/waveout

INCLUDEPATH += ../../../
QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer

CONFIG += warn_on \
          thread \
          plugin

TEMPLATE = lib
LIBS += -lqmmp0 -lwinmm

isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
