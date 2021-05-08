include($$PWD/../zed.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderfc14factory.h \
           decoder_fc14.h \
           fc14helper.h

SOURCES += decoderfc14factory.cpp \
           decoder_fc14.cpp \
           fc14helper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libfc14/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libfc14/lib -lfc14$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libfc14/lib -lfc14
}
