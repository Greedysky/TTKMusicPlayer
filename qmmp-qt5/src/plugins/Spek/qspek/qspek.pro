include(../../plugins.pri)

TARGET=$$PLUGINS_PREFIX/Spek/qspek
QMAKE_CLEAN =$$PLUGINS_PREFIX/Spek/libqspek.so


SOURCES += \
        spek-ruler.cc \
        spek-spectrogram.cc \
        spek-audio.cc \
        spek-fft.cc \
        spek-palette.cc \
        spek-pipeline.cc \
        visualqspekfactory.cpp

HEADERS  += \
        spek-audio.h \
        spek-fft.h \
        spek-palette.h \
        spek-pipeline.h \
        spek-ruler.h \
        spek-spectrogram.h \
        visualqspekfactory.h

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libav/include

CONFIG += warn_on \
            plugin

TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11

win32:{
    INCLUDEPATH += ./
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec.dll -lavformat.dll -lavutil.dll
}

unix{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    target.path = $$LIB_DIR/qmmp/Visual
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libav/lib -lavcodec -lavformat -lavutil
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
}
