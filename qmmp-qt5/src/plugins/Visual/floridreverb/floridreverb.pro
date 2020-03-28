include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridreverb

HEADERS += floridreverb.h \
           visualfloridreverbfactory.h
           
SOURCES += floridreverb.cpp \
           visualfloridreverbfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libfloridreverb.so
    LIBS += -L/usr/lib -I/usr/include
}
