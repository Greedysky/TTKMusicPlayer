include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderayflyfactory.h \
           decoder_ayfly.h \
           ayflyhelper.h \
           ayfly/AbstractAudio.h \
           ayfly/ay.h  \
           ayfly/ayfly.h  \
           ayfly/ayflyString.h  \
           ayfly/Filter3.h \
           ayfly/z80ex/z80ex.h
    
SOURCES += decoderayflyfactory.cpp \
           decoder_ayfly.cpp \
           ayflyhelper.cpp \
           ayfly/AbstractAudio.cpp \
           ayfly/ay.cpp  \
           ayfly/formats.cpp  \
           ayfly/speccy.cpp  \
           ayfly/lha.cpp  \
           ayfly/common.cpp  \
           ayfly/ayflyString.cpp  \
           ayfly/Filter3.cpp \
           ayfly/z80ex/z80ex.c

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$PWD/ayfly \
               $$PWD/ayfly/z80ex

DEFINES += DISABLE_AUDIO

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
