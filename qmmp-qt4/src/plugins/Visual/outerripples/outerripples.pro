include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = outerripples

HEADERS += outerripples.h \
           visualouterripplesfactory.h

SOURCES += outerripples.cpp \
           visualouterripplesfactory.cpp

unix{
    QMAKE_CLEAN = libouterripples.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
