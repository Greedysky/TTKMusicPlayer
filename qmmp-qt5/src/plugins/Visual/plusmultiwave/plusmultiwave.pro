include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = plusmultiwave

HEADERS += plusmultiwave.h \
           visualplusmultiwavefactory.h

SOURCES += plusmultiwave.cpp \
           visualplusmultiwavefactory.cpp

unix{
    QMAKE_CLEAN = libplusmultiwave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
