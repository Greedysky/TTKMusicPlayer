include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/plusfoldwave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libplusfoldwave.so

include(../common/Visual.pri)

HEADERS += plusfoldwave.h \
           visualplusfoldwavefactory.h
           
SOURCES += plusfoldwave.cpp \
           visualplusfoldwavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
