include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = pluspointxrays

HEADERS += pluspointxrays.h \
           visualpluspointxraysfactory.h
           
SOURCES += pluspointxrays.cpp \
           visualpluspointxraysfactory.cpp

unix{
    QMAKE_CLEAN = libpluspointxrays.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
