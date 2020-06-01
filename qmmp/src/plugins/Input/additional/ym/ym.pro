include($$PWD/../additional.pri)

HEADERS += decoderymfactory.h \
           decoder_ym.h

SOURCES += decoderymfactory.cpp \
           decoder_ym.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libym/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libym/lib -lym$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libym/lib -lym
}
