include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/plusmonowave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libplusmonowave.so

include(../common/Visual.pri)

HEADERS += plusmonowave.h \
           visualplusmonowavefactory.h

SOURCES += plusmonowave.cpp \
           visualplusmonowavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
