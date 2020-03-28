include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = plusxrays

HEADERS += plusxrays.h \
           visualplusxraysfactory.h
           
SOURCES += plusxrays.cpp \
           visualplusxraysfactory.cpp

unix{
    QMAKE_CLEAN = libplusxrays.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
