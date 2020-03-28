include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = normalewave

HEADERS += normalewave.h \
           visualnormalewavefactory.h
           
SOURCES += normalewave.cpp \
           visualnormalewavefactory.cpp

unix{
    QMAKE_CLEAN = libnormalewave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
