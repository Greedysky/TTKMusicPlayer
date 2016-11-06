include(../../plugins.pri)

HEADERS += decoderffmpegfactory.h \
           decoder_ffmpeg.h \
           ffmpegmetadatamodel.h \
           replaygainreader.h
    
SOURCES += decoder_ffmpeg.cpp \
           decoderffmpegfactory.cpp \
           ffmpegmetadatamodel.cpp \
           replaygainreader.cpp

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libav/include

CONFIG += warn_on \
          plugin \
          link_pkgconfig
          
TEMPLATE = lib

TARGET = $$PLUGINS_PREFIX/Input/ffmpeg

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libffmpeg.so
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec -lavformat -lavutil -lqmmp
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec.dll -lavformat.dll -lavutil.dll \
            -lqmmp0
}

DEFINES += __STDC_CONSTANT_MACROS
