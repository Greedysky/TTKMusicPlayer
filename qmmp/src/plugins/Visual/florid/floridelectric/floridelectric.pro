include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += floridelectric.h \
           visualfloridelectricfactory.h
           
SOURCES += floridelectric.cpp \
           visualfloridelectricfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L/usr/lib -I/usr/include
}
