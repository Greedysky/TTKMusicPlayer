include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = plusvolumewave

HEADERS += plusvolumewave.h \
           visualplusvolumewavefactory.h
           
SOURCES += plusvolumewave.cpp \
           visualplusvolumewavefactory.cpp

unix{
    QMAKE_CLEAN = libplusvolumewave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
