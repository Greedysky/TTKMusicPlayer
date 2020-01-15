include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/outerripples

include(../common/common.pri)

HEADERS += outerripples.h \
           visualouterripplesfactory.h

SOURCES += outerripples.cpp \
           visualouterripplesfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libouterripples.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
