include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += pluspointxrays.h \
           visualpluspointxraysfactory.h
           
SOURCES += pluspointxrays.cpp \
           visualpluspointxraysfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
