include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual

HEADERS += floridbass.h \
           visualfloridbassfactory.h

SOURCES += floridbass.cpp \
           visualfloridbassfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
