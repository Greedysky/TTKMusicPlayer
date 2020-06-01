include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decodersc68factory.h \
           decoder_sc68.h \
           sc68helper.h \
           sc68metadatamodel.h
    
SOURCES += decodersc68factory.cpp \
           decoder_sc68.cpp \
           sc68helper.cpp \
           sc68metadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libsc68/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libsc68/lib -lsc68$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libsc68/lib -lsc68
}
