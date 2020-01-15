include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/floridautism

include(../common/common.pri)

HEADERS += floridautism.h \
           visualfloridautismfactory.h
           
SOURCES += floridautism.cpp \
           visualfloridautismfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libfloridautism.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
