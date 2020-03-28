include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridbass

HEADERS += floridbass.h \
           visualfloridbassfactory.h
           
SOURCES += floridbass.cpp \
           visualfloridbassfactory.cpp

unix{
    QMAKE_CLEAN = libfloridbass.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
