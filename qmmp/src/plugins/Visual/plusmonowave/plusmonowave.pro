include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += plusmonowave.h \
           visualplusmonowavefactory.h

SOURCES += plusmonowave.cpp \
           visualplusmonowavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
