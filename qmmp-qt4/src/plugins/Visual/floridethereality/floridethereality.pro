include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridethereality

HEADERS += ethereality.h \
           floridethereality.h \
           visualfloridetherealityfactory.h
           
SOURCES += ethereality.cpp \
           floridethereality.cpp \
           visualfloridetherealityfactory.cpp

RESOURCES += floridethereality.qrc

win32:{
    LIBS += -lglu32 -lopengl32
}

unix{
    QMAKE_CLEAN = $$DESTDIR/libfloridethereality.so
    LIBS += -L/usr/lib -I/usr/include -lGLU
}
