include($$PWD/../../plugins.pri)
include($$PWD/../common/common.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = floridancient

HEADERS += floridancient.h \
           visualfloridancientfactory.h
           
SOURCES += floridancient.cpp \
           visualfloridancientfactory.cpp

unix{
    QMAKE_CLEAN = libfloridancient.so
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
}
