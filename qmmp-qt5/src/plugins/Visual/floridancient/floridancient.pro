include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Visual/floridancient

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
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libfloridancient.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
