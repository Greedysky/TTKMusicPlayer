include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Visual/floridancient
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libfloridancient.so

include(../common/common.pri)

HEADERS += floridancient.h \
           visualfloridancientfactory.h
           
SOURCES += floridancient.cpp \
           visualfloridancientfactory.cpp

win32:{
    HEADERS += ../../../../src/qmmp/visual.h
    INCLUDEPATH += ./
}

unix{
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
