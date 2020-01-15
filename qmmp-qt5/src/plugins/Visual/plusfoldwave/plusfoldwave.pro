include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/plusfoldwave

include(../common/common.pri)

HEADERS += plusfoldwave.h \
           visualplusfoldwavefactory.h
           
SOURCES += plusfoldwave.cpp \
           visualplusfoldwavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libplusfoldwave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
