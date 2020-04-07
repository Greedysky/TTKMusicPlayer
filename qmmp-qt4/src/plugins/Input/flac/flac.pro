include($$PWD/../../plugins.pri)

HEADERS += decoderflacfactory.h \
           decoder_flac.h \
           cueparser.h \
           flacmetadatamodel.h
    
SOURCES += decoderflacfactory.cpp \
           decoder_flac.cpp \
           cueparser.cpp \
           flacmetadatamodel.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libflac/include

unix {
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libflac/lib -lFLAC$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libogg/lib -logg$$STATIC_LIBRARY_SUFFIX
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libflac/lib -lFLAC \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -lm
}
