include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/plusvolumewave
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libplusvolumewave.so

include(../common/Visual.pri)

HEADERS += plusvolumewave.h \
           visualplusvolumewavefactory.h
           
SOURCES += plusvolumewave.cpp \
           visualplusvolumewavefactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
