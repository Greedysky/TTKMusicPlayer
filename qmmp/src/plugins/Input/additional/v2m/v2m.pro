include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderv2mfactory.h \
           decoder_v2m.h \
           v2mhelper.h \
           tool/file.h \
           libv2.h \
           phonemtab.h \
           sounddef.h \
           synth.h \
           types.h \
           v2mconv.h \
           v2mplayer.h
    
SOURCES += decoderv2mfactory.cpp \
           decoder_v2m.cpp \
           v2mhelper.cpp \
           tool/file.cpp \
           sounddef.cpp \
           synth_core.cpp \
           v2mconv.cpp \
           v2mplayer.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$PWD

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
