include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridreverb

HEADERS += floridreverb.h \
           visualfloridreverbfactory.h
           
SOURCES += floridreverb.cpp \
           visualfloridreverbfactory.cpp

unix{
    QMAKE_CLEAN = libfloridreverb.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
