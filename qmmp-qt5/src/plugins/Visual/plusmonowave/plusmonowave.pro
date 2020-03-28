include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = plusmonowave

HEADERS += plusmonowave.h \
           visualplusmonowavefactory.h

SOURCES += plusmonowave.cpp \
           visualplusmonowavefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libplusmonowave.so
}
