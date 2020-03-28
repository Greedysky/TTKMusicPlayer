include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = normalspacewave

HEADERS += normalspacewave.h \
           visualnormalspacewavefactory.h

SOURCES += normalspacewave.cpp \
           visualnormalspacewavefactory.cpp

unix{
    QMAKE_CLEAN = libnormalspacewave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
