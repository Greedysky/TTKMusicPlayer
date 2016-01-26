include(../../plugins.pri)

HEADERS += outputjackfactory.h \
           outputjack.h \
           bio2jack.h

SOURCES += outputjackfactory.cpp \
           outputjack.cpp \
           bio2jack.c

TARGET=$$PLUGINS_PREFIX/Output/jack
QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libjack.so

contains(CONFIG, WITH_NEW_JACK){
    DEFINES += JACK_NEW_API
}

INCLUDEPATH += ../../../
QMAKE_LIBDIR += ../../../../lib
CONFIG += warn_on \
thread \
plugin \
link_pkgconfig
TEMPLATE = lib
LIBS += -lqmmp
PKGCONFIG += jack samplerate



isEmpty (LIB_DIR){
LIB_DIR = /lib
}

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
