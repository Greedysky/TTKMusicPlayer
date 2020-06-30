include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += wavevolume.h \
           visualwavevolumefactory.h
           
SOURCES += wavevolume.cpp \
           visualwavevolumefactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
