include(../../plugins.pri)

HEADERS += outputpulseaudiofactory.h \
           outputpulseaudio.h

SOURCES += outputpulseaudiofactory.cpp \
           outputpulseaudio.cpp


TARGET=$$PLUGINS_PREFIX/Output/pulseaudio
QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libpulseaudio.so

INCLUDEPATH += ../../../
QMAKE_LIBDIR += ../../../../lib

CONFIG += warn_on \
thread \
plugin \
link_pkgconfig

TEMPLATE = lib
LIBS += -lqmmp

PKGCONFIG += libpulse-simple



isEmpty (LIB_DIR){
LIB_DIR = /lib
}

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
