include($$PWD/../../plugins.pri)
include($$PWD/../../Visual/common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Light
TARGET = lightenvelope

SOURCES += \
        lightenvelope.cpp \
        visuallightenvelopefactory.cpp

HEADERS  += \
        lightenvelope.h \
        visuallightenvelopefactory.h

unix{
    QMAKE_CLEAN = $$DESTDIR/liblightenvelope.so
    LIBS += -L/usr/lib -I/usr/include
}
