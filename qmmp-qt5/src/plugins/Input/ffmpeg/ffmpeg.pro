include(../../plugins.pri)

HEADERS += decoderffmpegfactory.h \
           decoder_ffmpeg.h \
           ffmpegmetadatamodel.h \
           replaygainreader.h
    
SOURCES += decoder_ffmpeg.cpp \
           decoderffmpegfactory.cpp \
           ffmpegmetadatamodel.cpp \
           replaygainreader.cpp

TARGET = $$PLUGINS_PREFIX/Input/ffmpeg

INCLUDEPATH += $$EXTRA_PREFIX/libav/include

unix {
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_ffmpeg
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_ffmpeg.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libffmpeg.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec -lavformat -lavutil
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec.dll -lavformat.dll -lavutil.dll
}

DEFINES += __STDC_CONSTANT_MACROS
