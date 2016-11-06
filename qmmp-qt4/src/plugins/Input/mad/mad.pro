include(../../plugins.pri)

HEADERS += decodermadfactory.h \
           decoder_mad.h \
           tagextractor.h \
           mpegmetadatamodel.h \
           replaygainreader.h
    
SOURCES += decoder_mad.cpp \
           decodermadfactory.cpp \
           tagextractor.cpp \
           mpegmetadatamodel.cpp \
           replaygainreader.cpp
    
TARGET = $$PLUGINS_PREFIX/Input/mad

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libmad/include
               
CONFIG += warn_on \
          plugin \
          link_pkgconfig
    
TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libmad/lib -lmad \
            -lqmmp
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libmad.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
            -L$$EXTRA_PREFIX/libmad/lib -lmad \
            -lqmmp0
}
