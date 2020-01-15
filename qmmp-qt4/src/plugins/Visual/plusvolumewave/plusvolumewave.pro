include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/plusvolumewave

include(../common/common.pri)

HEADERS += plusvolumewave.h \
           visualplusvolumewavefactory.h
           
SOURCES += plusvolumewave.cpp \
           visualplusvolumewavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libplusvolumewave.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
