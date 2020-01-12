include(../../plugins.pri)

HEADERS += decoderymfactory.h \
           decoder_ym.h \
           ym_digidrum.h \
           ym_lzh.h \
           ym_2149ex.h \
           ym_load.h \
           ym_music.h \
           ym_types.h

SOURCES += decoder_ym.cpp \
           decoderymfactory.cpp \
           ym_digidrum.cpp \
           ym_lzhlib.cpp \
           ym_2149ex.cpp \
           ym_load.cpp \
           ym_music.cpp

TARGET = $$PLUGINS_PREFIX/Input/ym
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libym.so
