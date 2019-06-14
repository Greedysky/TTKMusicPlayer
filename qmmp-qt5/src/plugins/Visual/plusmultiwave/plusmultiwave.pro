include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/plusmultiwave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libplusmultiwave.so

include(../common/common.pri)

HEADERS += plusmultiwave.h \
           visualplusmultiwavefactory.h

SOURCES += plusmultiwave.cpp \
           visualplusmultiwavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
