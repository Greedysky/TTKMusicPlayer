include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderasapfactory.h \
           decoder_asap.h \
           asaphelper.h \
           asapmetadatamodel.h \
           asap.h

SOURCES += decoderasapfactory.cpp \
           decoder_asap.cpp \
           asaphelper.cpp \
           asapmetadatamodel.cpp \
           asap.c

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = asap

unix:{
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libasap.so
}
