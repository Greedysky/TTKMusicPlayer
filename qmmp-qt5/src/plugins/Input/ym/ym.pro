include(../../plugins.pri)

HEADERS += decoderymfactory.h \
           decoder_ym.h \
           digidrum.h \
           LZH.H \
           Ym2149Ex.h \
           YmLoad.h \
           YmMusic.h \
           YmTypes.h

SOURCES += decoder_ym.cpp \
           decoderymfactory.cpp \
           digidrum.cpp \
           LzhLib.cpp \
           Ym2149Ex.cpp \
           YmLoad.cpp \
           YmMusic.cpp

TARGET = $$PLUGINS_PREFIX/Input/ym
QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libym.so
