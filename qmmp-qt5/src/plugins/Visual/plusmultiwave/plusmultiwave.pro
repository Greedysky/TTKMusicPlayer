include(../../plugins.pri)
include(../common/common.pri)

TARGET = $$PLUGINS_PREFIX/Visual/plusmultiwave

HEADERS += plusmultiwave.h \
           visualplusmultiwavefactory.h

SOURCES += plusmultiwave.cpp \
           visualplusmultiwavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libplusmultiwave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
