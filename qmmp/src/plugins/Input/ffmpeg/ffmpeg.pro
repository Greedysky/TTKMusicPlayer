include($$PWD/../../plugins.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decoderffmpegfactory.h \
           decoder_ffmpeg.h \
           ffmpegmetadatamodel.h \
           decoder_ffmpegcue.h \
           decoder_ffmpegm4b.h \
           replaygainreader.h \
           settingsdialog.h

SOURCES += decoderffmpegfactory.cpp \
           decoder_ffmpeg.cpp \
           ffmpegmetadatamodel.cpp \
           decoder_ffmpegcue.cpp \
           decoder_ffmpegm4b.cpp \
           replaygainreader.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

INCLUDEPATH += $$EXTRA_PREFIX/libffmpeg/include \
               $$EXTRA_PREFIX/libtaglib/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libffmpeg/lib -lavcodec -lavformat -lavutil -lswresample
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libtaglib/lib -ltag \
            -L$$EXTRA_PREFIX/libffmpeg/lib -lavcodec.dll -lavformat.dll -lavutil.dll -lswresample.dll
}

DEFINES += __STDC_CONSTANT_MACROS
