include($$PWD/../additional.pri)

HEADERS += decoderalacfactory.h \
           decoder_alac.h \
           alachelper.h \
           alac_decomp.h \
           alac_demux.h \
           alac_stdint_win.h \
           alac_stream.h

SOURCES += decoderalacfactory.cpp \
           decoder_alac.cpp \
           alachelper.cpp \
           alac.c \
           alac_demux.c \
           alac_stream.c

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = alac

unix:{
    QMAKE_CLEAN = $$DESTDIR/libalac.so
}
