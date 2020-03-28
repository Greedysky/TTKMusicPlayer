include($$PWD/../additional.pri)

HEADERS += decoderymfactory.h \
           decoder_ym.h \
           ym_digidrum.h \
           ym_lzh.h \
           ym_2149ex.h \
           ym_load.h \
           ym_music.h \
           ym_types.h

SOURCES += decoderymfactory.cpp \
           decoder_ym.cpp \
           ym_digidrum.cpp \
           ym_lzhlib.cpp \
           ym_2149ex.cpp \
           ym_load.cpp \
           ym_music.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = ym

unix:{
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libym.so
}
