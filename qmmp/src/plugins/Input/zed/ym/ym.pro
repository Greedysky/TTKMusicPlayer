include($$PWD/../zed.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decoderymfactory.h \
           decoder_ym.h

SOURCES += decoderymfactory.cpp \
           decoder_ym.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libym/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libym/lib -lym$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libym/lib -lym
}
