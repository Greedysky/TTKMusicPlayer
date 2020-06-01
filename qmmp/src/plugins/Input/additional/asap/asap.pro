include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decoderasapfactory.h \
           decoder_asap.h \
           asaphelper.h \
           asapmetadatamodel.h

SOURCES += decoderasapfactory.cpp \
           decoder_asap.cpp \
           asaphelper.cpp \
           asapmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libasap/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libasap/lib -lasap$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libasap/lib -lasap
}
