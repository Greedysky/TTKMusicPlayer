include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/outerrayswave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libouterrayswave.so

include(../common/common.pri)

HEADERS += outerrayswave.h \
           visualouterrayswavefactory.h
           
SOURCES += outerrayswave.cpp \
           visualouterrayswavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
