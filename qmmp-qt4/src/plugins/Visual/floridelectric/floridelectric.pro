include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/floridelectric
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfloridelectric.so

include(../common/common.pri)

HEADERS += floridelectric.h \
           visualfloridelectricfactory.h
           
SOURCES += floridelectric.cpp \
           visualfloridelectricfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
