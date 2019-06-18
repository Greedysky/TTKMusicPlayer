include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/floridbass
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfloridbass.so

include(../common/common.pri)

HEADERS += floridbass.h \
           visualfloridbassfactory.h
           
SOURCES += floridbass.cpp \
           visualfloridbassfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
