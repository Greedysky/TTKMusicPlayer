include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += plusfoldwave.h \
           visualplusfoldwavefactory.h
           
SOURCES += plusfoldwave.cpp \
           visualplusfoldwavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
