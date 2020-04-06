include($$PWD/../../plugins.pri)

HEADERS += decoderffmpegfactory.h \
           decoder_ffmpeg.h \
           ffmpegmetadatamodel.h \
           replaygainreader.h
    
SOURCES += decoderffmpegfactory.cpp \
           decoder_ffmpeg.cpp \
           ffmpegmetadatamodel.cpp \
           replaygainreader.cpp

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = ffmpeg

INCLUDEPATH += $$EXTRA_PREFIX/libav/include

unix {
    unix:android {
        TARGET = plugin_input_ffmpeg
        QMAKE_CLEAN = $$DESTDIR/libplugin_input_ffmpeg.so
    }else{
        QMAKE_CLEAN = $$DESTDIR/libffmpeg.so
    }
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec -lavformat -lavutil
}

win32 {
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec.dll -lavformat.dll -lavutil.dll
}

DEFINES += __STDC_CONSTANT_MACROS
