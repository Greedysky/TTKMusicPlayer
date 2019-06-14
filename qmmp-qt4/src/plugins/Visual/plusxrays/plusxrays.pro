include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/plusxrays
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libplusxrays.so

include(../common/common.pri)

HEADERS += plusxrays.h \
           visualplusxraysfactory.h
           
SOURCES += plusxrays.cpp \
           visualplusxraysfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
