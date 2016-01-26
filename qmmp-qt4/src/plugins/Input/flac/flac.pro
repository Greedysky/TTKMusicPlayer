include(../../plugins.pri)
FORMS +=
HEADERS += decoderflacfactory.h \
    decoder_flac.h \
    cueparser.h \
    flacmetadatamodel.h \
    replaygainreader.h
SOURCES += decoder_flac.cpp \
    decoderflacfactory.cpp \
    cueparser.cpp \
    flacmetadatamodel.cpp \
    replaygainreader.cpp
TARGET = $$PLUGINS_PREFIX/Input/flac

INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin \
    link_pkgconfig
TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib



unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    PKGCONFIG += taglib flac
    LIBS += -lqmmp
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libflac.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin
    gcc{
        INCLUDEPATH += $$EXTRA_PREFIX/libtaglib/include \
                       $$EXTRA_PREFIX/libflac/include
        LIBS += -L$$EXTRA_PREFIX/libflac/lib -llibFLAC \
                -L$$EXTRA_PREFIX/libogg/lib -logg \
                -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
                -lqmmp0 -lm
    }
#    LIBS += -lqmmp0 -lflac -logg -ltag.dll -lm
}
