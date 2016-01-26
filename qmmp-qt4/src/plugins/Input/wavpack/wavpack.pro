include(../../plugins.pri)
FORMS +=
HEADERS += decoderwavpackfactory.h \
    decoder_wavpack.h \
    cueparser.h \
    wavpackmetadatamodel.h \
    replaygainreader.h
SOURCES += decoder_wavpack.cpp \
    decoderwavpackfactory.cpp \
    cueparser.cpp \
    wavpackmetadatamodel.cpp \
    replaygainreader.cpp
TARGET = $$PLUGINS_PREFIX/Input/wavpack

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

    QMAKE_LIBDIR += ../../../../lib
    LIBS += -lqmmp
    PKGCONFIG += wavpack
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libwavpack.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin
    gcc{
        INCLUDEPATH += $$EXTRA_PREFIX/libwavpack/include

        LIBS += -L$$EXTRA_PREFIX/libwavpack/lib -lwavpack \
                -lqmmp0
    }
#    LIBS += -lqmmp0 -lwavpack
}
