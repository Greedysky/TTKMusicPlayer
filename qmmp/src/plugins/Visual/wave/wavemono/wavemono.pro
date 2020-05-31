include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += wavemono.h \
           visualwavemonofactory.h

SOURCES += wavemono.cpp \
           visualwavemonofactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
