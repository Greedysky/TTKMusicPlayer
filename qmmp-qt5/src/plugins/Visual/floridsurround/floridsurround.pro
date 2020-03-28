include(../../plugins.pri)
include(../common/common.pri)

TARGET = $$PLUGINS_PREFIX/Visual/floridsurround

HEADERS += floridsurround.h \
           visualfloridsurroundfactory.h
           
SOURCES += floridsurround.cpp \
           visualfloridsurroundfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/florid.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libfloridsurround.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
