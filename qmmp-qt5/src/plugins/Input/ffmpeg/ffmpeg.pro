include(../../plugins.pri)
FORMS += settingsdialog.ui
HEADERS += decoderffmpegfactory.h \
    decoder_ffmpeg.h \
    settingsdialog.h \
    ffmpegmetadatamodel.h
SOURCES += decoder_ffmpeg.cpp \
    decoderffmpegfactory.cpp \
    settingsdialog.cpp \
    ffmpegmetadatamodel.cpp

INCLUDEPATH += ../../../



CONFIG += warn_on \
    plugin \
    link_pkgconfig
TEMPLATE = lib

TARGET = $$PLUGINS_PREFIX/Input/ffmpeg

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libffmpeg.so
    LIBS += -lqmmp
    QMAKE_LIBDIR += ../../../../lib
    PKGCONFIG += libavcodec libavformat libavutil
}


win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin

    gcc{
        INCLUDEPATH += $$EXTRA_PREFIX/libav/include
        LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec.dll -lavformat.dll -lavutil.dll \
                -lqmmp1
    }
#    LIBS += -lqmmp0 -lavcodec.dll -lavformat.dll -lavutil.dll
}

DEFINES += __STDC_CONSTANT_MACROS


