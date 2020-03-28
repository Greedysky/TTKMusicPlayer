include(../../plugins.pri)

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

TARGET = $$PLUGINS_PREFIX/Input/alac

unix:{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libalac.so
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
}
