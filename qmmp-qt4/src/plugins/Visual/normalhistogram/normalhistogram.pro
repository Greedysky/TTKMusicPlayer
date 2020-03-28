include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = normalhistogram

HEADERS += normalhistogram.h \
           visualnormalhistogramfactory.h

SOURCES += normalhistogram.cpp \
           visualnormalhistogramfactory.cpp

unix{
    QMAKE_CLEAN = libnormalhistogram.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
