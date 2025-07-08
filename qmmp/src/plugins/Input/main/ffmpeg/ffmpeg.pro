include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderffmpegfactory.h \
           decoder_ffmpeg.h \
           ffmpegmetadatamodel.h \
           decoder_ffmpegcue.h \
           decoder_ffmpegm4b.h \
           replaygainreader.h

SOURCES += decoderffmpegfactory.cpp \
           decoder_ffmpeg.cpp \
           ffmpegmetadatamodel.cpp \
           decoder_ffmpegcue.cpp \
           decoder_ffmpegm4b.cpp \
           replaygainreader.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libffmpeg/include \
               $$EXTRA_PREFIX/libtaglib/include

DEFINES += __STDC_CONSTANT_MACROS

win32{
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libffmpeg/lib -lavcodec.dll -lavformat.dll -lavutil.dll -lswresample.dll
}

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libffmpeg/lib -lavcodec -lavformat -lavutil -lswresample
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -ltag -lavcodec -lavformat -lavutil -lswresample
}
