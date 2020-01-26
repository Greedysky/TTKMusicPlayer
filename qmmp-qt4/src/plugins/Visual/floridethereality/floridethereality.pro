include(../../plugins.pri)

QT += opengl
TARGET = $$PLUGINS_PREFIX/Visual/floridethereality

include(../common/common.pri)

HEADERS += ethereality.h \
           floridethereality.h \
           visualfloridetherealityfactory.h
           
SOURCES += ethereality.cpp \
           floridethereality.cpp \
           visualfloridetherealityfactory.cpp

RESOURCES += floridethereality.qrc

win32:{
    HEADERS += ../../../../src/qmmp/florid.h
    INCLUDEPATH += ./
    LIBS += -lglu32 -lopengl32
}

unix{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Visual/libfloridethereality.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include -lGLU
}
