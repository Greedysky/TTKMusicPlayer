include($$PWD/../../plugins.pri)
include($$PWD/../../Visual/common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Light
TARGET = $${TARGET}

SOURCES += lightenvelope.cpp \
           lightenvelopefactory.cpp

HEADERS += lightenvelope.h \
           lightenvelopefactory.h

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L/usr/lib -I/usr/include
}
