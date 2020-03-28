include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridelectric

HEADERS += floridelectric.h \
           visualfloridelectricfactory.h
           
SOURCES += floridelectric.cpp \
           visualfloridelectricfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/libfloridelectric.so
    LIBS += -L/usr/lib -I/usr/include
}
