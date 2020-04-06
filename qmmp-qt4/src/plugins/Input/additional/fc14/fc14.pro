include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderfc14factory.h \
           decoder_fc14.h \
           fc14helper.h \
           Dump.h \
           FC.h \
           fc14audiodecoder.h \
           LamePaula.h \
           MyEndian.h \
           MyTypes.h \
           Paula.h \
           SmartPtr.h

SOURCES += decoderfc14factory.cpp \
           decoder_fc14.cpp \
           fc14helper.cpp \
           Dump.cpp \
           FC.cpp \
           FC_Data.cpp \
           fc14audiodecoder.cpp \
           LamePaulaMixer.cpp \
           LamePaulaVoice.cpp \
           Paula.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = fc14

unix:{
    QMAKE_CLEAN = $$DESTDIR/libfc14.so
}
