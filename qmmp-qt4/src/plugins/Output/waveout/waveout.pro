include(../../plugins.pri)

HEADERS += outputwaveoutfactory.h \
           outputwaveout.h

SOURCES += outputwaveoutfactory.cpp \
           outputwaveout.cpp

HEADERS += ../../../../src/qmmp/output.h

TARGET=$$PLUGINS_PREFIX/Output/waveout
QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libwaveout.so

LIBS += -lwinmm

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
