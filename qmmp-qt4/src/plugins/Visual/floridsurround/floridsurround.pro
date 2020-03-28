include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridsurround

HEADERS += floridsurround.h \
           visualfloridsurroundfactory.h
           
SOURCES += floridsurround.cpp \
           visualfloridsurroundfactory.cpp

unix{
    QMAKE_CLEAN = libfloridsurround.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
