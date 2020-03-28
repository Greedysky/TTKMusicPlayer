include(../../plugins.pri)
include(../common/common.pri)

TARGET = $$PLUGINS_PREFIX/Visual/outerrayswave

HEADERS += outerrayswave.h \
           visualouterrayswavefactory.h
           
SOURCES += outerrayswave.cpp \
           visualouterrayswavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libouterrayswave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
