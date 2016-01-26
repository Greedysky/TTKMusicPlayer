include(../../plugins.pri)
FORMS +=
HEADERS += decoderopusfactory.h \
    decoder_opus.h \
    opusmetadatamodel.h \
    replaygainreader.h \
    opusproperties.h \
    opusfile.h \
    tdebug.h
SOURCES += decoder_opus.cpp \
    decoderopusfactory.cpp \
    opusmetadatamodel.cpp \
    replaygainreader.cpp \
    opusproperties.cpp \
    opusfile.cpp \
    tdebug.cpp
TARGET = $$PLUGINS_PREFIX/Input/opus

INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin \
    link_pkgconfig
TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc


unix {
    isEmpty (LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target

    PKGCONFIG += taglib opus opusfile
    LIBS += -lqmmp
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libopus.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmp0 -lopusfile -lopus -ltag.dll -lm
}
