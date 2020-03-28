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
    QMAKE_CLEAN = liblightenvelope.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
