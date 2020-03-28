include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridautism

HEADERS += floridautism.h \
           visualfloridautismfactory.h
           
SOURCES += floridautism.cpp \
           visualfloridautismfactory.cpp

unix{
    QMAKE_CLEAN = libfloridautism.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
