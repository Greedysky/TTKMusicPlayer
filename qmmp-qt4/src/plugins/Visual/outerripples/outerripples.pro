include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/outerripples
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libouterripples.so

include(../common/Visual.pri)

HEADERS += outerripples.h \
           visualouterripplesfactory.h

SOURCES += outerripples.cpp \
           visualouterripplesfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
