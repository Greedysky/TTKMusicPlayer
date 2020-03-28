include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = outerewave

HEADERS += outerewave.h \
           visualouterewavefactory.h
           
SOURCES += outerewave.cpp \
           visualouterewavefactory.cpp

unix{
    QMAKE_CLEAN = libouterewave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
