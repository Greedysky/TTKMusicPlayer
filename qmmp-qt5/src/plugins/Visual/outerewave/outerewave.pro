include(../../plugins.pri)
include(../common/common.pri)

TARGET = $$PLUGINS_PREFIX/Visual/outerewave

HEADERS += outerewave.h \
           visualouterewavefactory.h
           
SOURCES += outerewave.cpp \
           visualouterewavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libouterewave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
