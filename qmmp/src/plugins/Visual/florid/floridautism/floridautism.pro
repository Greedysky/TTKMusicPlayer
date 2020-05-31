include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += floridautism.h \
           visualfloridautismfactory.h
           
SOURCES += floridautism.cpp \
           visualfloridautismfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L/usr/lib -I/usr/include
}
