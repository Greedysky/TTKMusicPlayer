include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridelectric

HEADERS += floridelectric.h \
           visualfloridelectricfactory.h
           
SOURCES += floridelectric.cpp \
           visualfloridelectricfactory.cpp

unix{
    QMAKE_CLEAN = libfloridelectric.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
