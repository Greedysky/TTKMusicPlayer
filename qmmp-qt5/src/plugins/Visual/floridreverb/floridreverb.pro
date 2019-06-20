include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/floridreverb
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfloridreverb.so

include(../common/common.pri)

HEADERS += floridreverb.h \
           visualfloridreverbfactory.h
           
SOURCES += floridreverb.cpp \
           visualfloridreverbfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
