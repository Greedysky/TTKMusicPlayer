include(../../plugins.pri)

HEADERS += decodervorbisfactory.h \
           decoder_vorbis.h \
           vorbismetadatamodel.h \
           replaygainreader.h
    
SOURCES += decoder_vorbis.cpp \
           decodervorbisfactory.cpp \
           vorbismetadatamodel.cpp \
           replaygainreader.cpp
          
TARGET = $$PLUGINS_PREFIX/Input/vorbis

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libtaglib/include \
               $$EXTRA_PREFIX/libogg/include \
               $$EXTRA_PREFIX/libvorbis/include
                   
CONFIG += warn_on \
          plugin \
          link_pkgconfig
          
TEMPLATE = lib

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisfile -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag -lqmmp
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libvorbis.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libvorbis/lib -lvorbisfile -lvorbis \
            -L$$EXTRA_PREFIX/libogg/lib -logg \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll \
            -lqmmp0 -lm
    LD_FLAGS += -no-undefined
}
