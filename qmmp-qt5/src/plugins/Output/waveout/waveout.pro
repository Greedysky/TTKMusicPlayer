include($$PWD/../../plugins.pri)

HEADERS += outputwaveoutfactory.h \
           outputwaveout.h

SOURCES += outputwaveoutfactory.cpp \
           outputwaveout.cpp

DESTDIR = $$PLUGINS_PREFIX/Output
TARGET = waveout

LIBS += -lwinmm

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
