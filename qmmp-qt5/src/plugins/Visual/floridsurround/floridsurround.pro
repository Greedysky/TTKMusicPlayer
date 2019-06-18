include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/floridsurround
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfloridsurround.so

include(../common/common.pri)

HEADERS += floridsurround.h \
           visualfloridsurroundfactory.h
           
SOURCES += floridsurround.cpp \
           visualfloridsurroundfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
