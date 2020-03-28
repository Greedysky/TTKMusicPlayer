include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridancient

HEADERS += floridancient.h \
           visualfloridancientfactory.h
           
SOURCES += floridancient.cpp \
           visualfloridancientfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libfloridancient.so
    LIBS += -L/usr/lib -I/usr/include
}
