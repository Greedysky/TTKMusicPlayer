include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/floridreverb

include(../common/common.pri)

HEADERS += floridreverb.h \
           visualfloridreverbfactory.h
           
SOURCES += floridreverb.cpp \
           visualfloridreverbfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/florid.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libfloridreverb.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
