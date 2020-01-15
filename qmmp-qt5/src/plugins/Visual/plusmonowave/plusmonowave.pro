include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/plusmonowave

include(../common/common.pri)

HEADERS += plusmonowave.h \
           visualplusmonowavefactory.h

SOURCES += plusmonowave.cpp \
           visualplusmonowavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libplusmonowave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
