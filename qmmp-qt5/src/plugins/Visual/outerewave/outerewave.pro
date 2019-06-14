include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/outerewave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libouterewave.so

include(../common/common.pri)

HEADERS += outerewave.h \
           visualouterewavefactory.h
           
SOURCES += outerewave.cpp \
           visualouterewavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
