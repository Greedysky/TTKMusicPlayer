include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/pluspointxrays

include(../common/common.pri)

HEADERS += pluspointxrays.h \
           visualpluspointxraysfactory.h
           
SOURCES += pluspointxrays.cpp \
           visualpluspointxraysfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libpluspointxrays.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
}
