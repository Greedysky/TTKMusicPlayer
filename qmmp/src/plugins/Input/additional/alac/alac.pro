include($$PWD/../additional.pri)

HEADERS += decoderalacfactory.h \
           decoder_alac.h \
           alachelper.h

SOURCES += decoderalacfactory.cpp \
           decoder_alac.cpp \
           alachelper.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libalac/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libalac/lib -lalac$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libalac/lib -lalac
}
